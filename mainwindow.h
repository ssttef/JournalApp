#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QDateTimeEdit>
#include <QFormLayout>
#include <QMenuBar>
#include <QAction>
#include <QTableWidget>
#include <QHeaderView>

#include "JournalEntryController.h"
#include "JournalEntryForm.h"
#include "FilterDialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(std::unique_ptr<JournalEntryController> controller, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void refreshEntryList();
    void onAddEntryClicked();
    void onEditEntryClicked();
    void onRemoveEntryClicked();
    void onEntrySelectionChanged();
    void onUndoClicked();
    void onRedoClicked();
    void onApplyFilterClicked();
    void onClearFilterClicked();
    void onExitClicked();

private:
    std::unique_ptr<JournalEntryController> m_controller;
    QUuid m_selectedEntryId;

    QTableWidget* m_entryTable;
    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_removeButton;
    QPushButton* m_undoButton;
    QPushButton* m_redoButton;
    QPushButton* m_filterButton;
    QPushButton* m_clearFilterButton;

    QAction* m_addAction;
    QAction* m_editAction;
    QAction* m_removeAction;
    QAction* m_undoAction;
    QAction* m_redoAction;
    QAction* m_filterAction;
    QAction* m_clearFilterAction;
    QAction* m_exitAction;

    void setupUI();
    void createActions();
    void createMenus();
    void updateButtonsState();
    void displayEntries(const QVector<JournalEntry>& entries);

    std::unique_ptr<FilterStrategy> m_currentFilter;
};

#endif
