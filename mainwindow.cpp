#include "MainWindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog> // Not used in provided code but listed in .h
#include <QVBoxLayout>   // Explicitly needed
#include <QHBoxLayout>   // Explicitly needed
#include <QHeaderView>   // Explicitly needed
#include <QTableWidgetItem> // Explicitly needed
#include <QAction>          // Explicitly needed
#include <QMenuBar>         // Explicitly needed
#include <QApplication>
#include <QKeySequence>
#include <QDateTime>
#include <QLocale>

MainWindow::MainWindow(std::unique_ptr<JournalEntryController> controller, QWidget *parent)
    : QMainWindow(parent), m_controller(std::move(controller)) {
    setupUI();
    createActions();
    createMenus();
    refreshEntryList(); // Populate table on startup
    updateButtonsState(); // Set initial button states

    // Connect controller's signal to UI refresh slot
    connect(m_controller.get(), &JournalEntryController::entriesChanged, this, &MainWindow::refreshEntryList);
}

MainWindow::~MainWindow() {
    // Unique pointers automatically handle deletion of m_controller and m_currentFilter.
    // Qt widgets are parented, so they are deleted automatically when MainWindow is deleted.
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    m_entryTable = new QTableWidget(this);
    m_entryTable->setColumnCount(4); // Column 0: Title, Column 1: Timestamp, Column 2: Tags, Column 3: (stretch)
    m_entryTable->setHorizontalHeaderLabels({"Title", "Timestamp", "Tags", ""});
    m_entryTable->horizontalHeader()->setStretchLastSection(true);
    m_entryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_entryTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_entryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addWidget(m_entryTable);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton("Add Entry", this);
    m_editButton = new QPushButton("Edit Entry", this);
    m_removeButton = new QPushButton("Remove Entry", this);
    m_undoButton = new QPushButton("Undo", this);
    m_redoButton = new QPushButton("Redo", this);
    m_filterButton = new QPushButton("Apply Filter", this);
    m_clearFilterButton = new QPushButton("Clear Filter", this);

    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_undoButton);
    buttonLayout->addWidget(m_redoButton);
    buttonLayout->addWidget(m_filterButton);
    buttonLayout->addWidget(m_clearFilterButton);

    mainLayout->addLayout(buttonLayout);

    // Connections for buttons and table selection
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddEntryClicked);
    connect(m_editButton, &QPushButton::clicked, this, &MainWindow::onEditEntryClicked);
    connect(m_removeButton, &QPushButton::clicked, this, &MainWindow::onRemoveEntryClicked);
    connect(m_undoButton, &QPushButton::clicked, this, &MainWindow::onUndoClicked);
    connect(m_redoButton, &QPushButton::clicked, this, &MainWindow::onRedoClicked);
    connect(m_filterButton, &QPushButton::clicked, this, &MainWindow::onApplyFilterClicked);
    connect(m_clearFilterButton, &QPushButton::clicked, this, &MainWindow::onClearFilterClicked);
    connect(m_entryTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::onEntrySelectionChanged);

    // No need for updateButtonsState() here, it's called in constructor
}

void MainWindow::createActions() {
    m_addAction = new QAction("Add &Entry", this);
    m_addAction->setShortcuts(QKeySequence::New); // More semantic shortcut
    connect(m_addAction, &QAction::triggered, this, &MainWindow::onAddEntryClicked);

    m_editAction = new QAction("&Edit Entry", this);
    m_editAction->setShortcut(QKeySequence("Ctrl+E"));
    connect(m_editAction, &QAction::triggered, this, &MainWindow::onEditEntryClicked);

    m_removeAction = new QAction("&Remove Entry", this);
    m_removeAction->setShortcut(QKeySequence::Delete);
    connect(m_removeAction, &QAction::triggered, this, &MainWindow::onRemoveEntryClicked);

    m_undoAction = new QAction("&Undo", this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    connect(m_undoAction, &QAction::triggered, this, &MainWindow::onUndoClicked);

    m_redoAction = new QAction("&Redo", this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    connect(m_redoAction, &QAction::triggered, this, &MainWindow::onRedoClicked);

    m_filterAction = new QAction("&Apply Filter", this);
    connect(m_filterAction, &QAction::triggered, this, &MainWindow::onApplyFilterClicked);

    m_clearFilterAction = new QAction("Clear F&ilter", this);
    connect(m_clearFilterAction, &QAction::triggered, this, &MainWindow::onClearFilterClicked);

    m_exitAction = new QAction("E&xit", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    connect(m_exitAction, &QAction::triggered, this, &MainWindow::onExitClicked);
}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction(m_addAction);
    fileMenu->addAction(m_editAction);
    fileMenu->addAction(m_removeAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exitAction);

    QMenu *editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction(m_undoAction);
    editMenu->addAction(m_redoAction);
    editMenu->addSeparator();
    editMenu->addAction(m_filterAction);
    editMenu->addAction(m_clearFilterAction);
}

void MainWindow::refreshEntryList() {
    // CRITICAL FIX: Use clone() to pass a copy of the filter strategy
    std::unique_ptr<FilterStrategy> filterToPass;
    if (m_currentFilter) {
        filterToPass = m_currentFilter->clone(); // Requires clone() method in FilterStrategy hierarchy
    }
    QVector<JournalEntry> entries = m_controller->getFilteredEntries(std::move(filterToPass));

    m_entryTable->setRowCount(0); // Clear existing rows

    m_entryTable->setRowCount(entries.size());
    for (int i = 0; i < entries.size(); ++i) {
        const JournalEntry& entry = entries[i];
        m_entryTable->setItem(i, 0, new QTableWidgetItem(entry.getTitle()));
        m_entryTable->setItem(i, 1, new QTableWidgetItem(QLocale::system().toString(entry.getTimestamp(), QLocale::ShortFormat)));
        m_entryTable->setItem(i, 2, new QTableWidgetItem(entry.getTags().join(", ")));

        // Store the entry's UUID in the first column's item's UserRole data
        m_entryTable->item(i, 0)->setData(Qt::UserRole, entry.getId().toString());
    }
    m_entryTable->resizeColumnsToContents(); // Adjust column widths to fit content
    updateButtonsState(); // Update button states (e.g., enable/disable Edit/Remove)
}

void MainWindow::onAddEntryClicked() {
    JournalEntryForm form(this);
    if (form.exec() == QDialog::Accepted) {
        // Create new entry with a fresh UUID
        JournalEntry newEntry(QUuid::createUuid(), form.getTitle(), form.getContent(), form.getTimestamp(), form.getTags());
        if (!m_controller->addEntry(newEntry)) {
            QMessageBox::warning(this, "Error", "Failed to add journal entry.");
        }
        // refreshEntryList() is called automatically via entriesChanged signal.
    }
}

void MainWindow::onEditEntryClicked() {
    if (m_selectedEntryId.isNull()) {
        QMessageBox::information(this, "No Selection", "Please select an entry to edit.");
        return;
    }

    // Retrieve the current version of the entry from the controller
    std::unique_ptr<JournalEntry> entryToEdit = m_controller->getEntryByIdInternal(m_selectedEntryId);
    if (!entryToEdit) {
        QMessageBox::warning(this, "Error", "Selected entry not found or has been removed.");
        refreshEntryList(); // Refresh to ensure UI reflects actual state
        return;
    }

    JournalEntryForm form(this);
    form.setEntry(*entryToEdit); // Populate form with existing data
    if (form.exec() == QDialog::Accepted) {
        // Create an updated JournalEntry, preserving the original ID
        JournalEntry updatedEntry(entryToEdit->getId(), form.getTitle(), form.getContent(), form.getTimestamp(), form.getTags());
        if (!m_controller->updateEntry(updatedEntry)) {
            QMessageBox::warning(this, "Error", "Failed to update journal entry.");
        }
        // refreshEntryList() is called automatically via entriesChanged signal.
    }
}

void MainWindow::onRemoveEntryClicked() {
    if (m_selectedEntryId.isNull()) {
        QMessageBox::information(this, "No Selection", "Please select an entry to remove.");
        return;
    }

    if (QMessageBox::question(this, "Confirm Removal", "Are you sure you want to remove this entry?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        if (!m_controller->removeEntry(m_selectedEntryId)) {
            QMessageBox::warning(this, "Error", "Failed to remove journal entry.");
        }
        // refreshEntryList() is called automatically via entriesChanged signal.
    }
}

void MainWindow::onEntrySelectionChanged() {
    QList<QTableWidgetItem*> selectedItems = m_entryTable->selectedItems();
    if (!selectedItems.isEmpty()) {
        // Get the ID from the UserRole data of the first selected item (which should be in column 0)
        m_selectedEntryId = QUuid(selectedItems.first()->data(Qt::UserRole).toString());
    } else {
        m_selectedEntryId = QUuid(); // No selection, clear the ID
    }
    updateButtonsState(); // Update buttons based on new selection state
}

void MainWindow::onUndoClicked() {
    if (!m_controller->undo()) {
        QMessageBox::information(this, "Undo", "Nothing to undo.");
    }
    // refreshEntryList() is called automatically via entriesChanged signal.
}

void MainWindow::onRedoClicked() {
    if (!m_controller->redo()) {
        QMessageBox::information(this, "Redo", "Nothing to redo.");
    }
    // refreshEntryList() is called automatically via entriesChanged signal.
}

void MainWindow::onApplyFilterClicked() {
    FilterDialog filterDialog(this);
    if (filterDialog.exec() == QDialog::Accepted) {
        // Take ownership of the filter strategy returned by the dialog
        m_currentFilter = filterDialog.getFilterStrategy();
        refreshEntryList(); // Refresh the display with the new filter applied
    }
}

void MainWindow::onClearFilterClicked() {
    m_currentFilter.reset(); // Clear the unique_ptr, effectively removing the filter
    refreshEntryList();      // Refresh the display to show all entries
}

void MainWindow::onExitClicked() {
    QApplication::quit();
}

void MainWindow::updateButtonsState() {
    bool hasSelection = !m_selectedEntryId.isNull();

    // Enable/disable 'Edit' and 'Remove' buttons/actions based on whether an entry is selected
    m_editButton->setEnabled(hasSelection);
    m_removeButton->setEnabled(hasSelection);
    m_editAction->setEnabled(hasSelection);
    m_removeAction->setEnabled(hasSelection);

    // Enable/disable 'Undo' and 'Redo' buttons/actions based on controller's capabilities
    m_undoButton->setEnabled(m_controller->canUndo());
    m_redoButton->setEnabled(m_controller->canRedo());
    m_undoAction->setEnabled(m_controller->canUndo());
    m_redoAction->setEnabled(m_controller->canRedo());
}

// Removed redundant displayEntries function as its logic is integrated into refreshEntryList.
// void MainWindow::displayEntries(const QVector<JournalEntry>& entries) { /* ... */ }