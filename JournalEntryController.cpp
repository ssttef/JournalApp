#include "JournalEntryController.h"
#include "AddCommand.h"
#include "RemoveCommand.h"
#include "UpdateCommand.h"
#include <QDebug>

#include <vector>
#include <algorithm>

JournalEntryController::JournalEntryController(std::unique_ptr<IJournalEntryRepository> repository)
    : m_repository(std::move(repository)) {
    if (!m_repository->loadAllEntries()) {
        qWarning() << "JournalEntryController: Failed to load initial entries from repository.";
    }
}

JournalEntryController::~JournalEntryController() {
    if (!m_repository->saveAllEntries(m_repository->getAllEntries())) {
        qWarning() << "JournalEntryController: Failed to save entries on controller destruction.";
    }
}

bool JournalEntryController::addEntry(const JournalEntry& entry) {
    std::unique_ptr<Command> command = std::make_unique<AddCommand>(this, entry);
    executeCommand(std::move(command), true);
    return true;
}

bool JournalEntryController::removeEntry(const QUuid& id) {
    std::unique_ptr<Command> command = std::make_unique<RemoveCommand>(this, id);
    executeCommand(std::move(command), true);
    return true;
}

bool JournalEntryController::updateEntry(const JournalEntry& entry) {
    std::unique_ptr<Command> command = std::make_unique<UpdateCommand>(this, entry);
    executeCommand(std::move(command), true);
    return true;
}

QVector<JournalEntry> JournalEntryController::getAllEntries() {
    return m_repository->getAllEntries();
}

QVector<JournalEntry> JournalEntryController::getFilteredEntries(std::unique_ptr<FilterStrategy> filter) {
    QVector<JournalEntry> allEntries = m_repository->getAllEntries();
    QVector<JournalEntry> filteredEntries;

    if (filter) {
        for (const auto& entry : allEntries) {
            if (filter->matches(entry)) {
                filteredEntries.append(entry);
            }
        }
    } else {
        filteredEntries = allEntries;
    }
    return filteredEntries;
}


bool JournalEntryController::undo() {
    if (!m_undoStack.empty()) {
        std::unique_ptr<Command> command = std::move(m_undoStack.back());
        m_undoStack.pop_back();

        command->undo();

        m_redoStack.push_back(std::move(command));
        emit entriesChanged();
        return true;
    }
    return false;
}

bool JournalEntryController::redo() {
    if (!m_redoStack.empty()) {
        std::unique_ptr<Command> command = std::move(m_redoStack.back());
        m_redoStack.pop_back();

        command->execute();
        m_undoStack.push_back(std::move(command));
        emit entriesChanged();
        return true;
    }
    return false;
}

bool JournalEntryController::canUndo() const {
    return !m_undoStack.empty();
}

bool JournalEntryController::canRedo() const {
    return !m_redoStack.empty();
}


bool JournalEntryController::addEntryInternal(const JournalEntry& entry, bool recordCommand) {
    bool success = m_repository->addEntry(entry);
    if (success && recordCommand) {
        m_redoStack.clear();
        emit entriesChanged();
    }
    return success;
}

bool JournalEntryController::removeEntryInternal(const QUuid& id, bool recordCommand) {
    bool success = m_repository->removeEntry(id);
    if (success && recordCommand) {
        m_redoStack.clear();
        emit entriesChanged();
    }
    return success;
}

bool JournalEntryController::updateEntryInternal(const JournalEntry& entry, bool recordCommand) {
    bool success = m_repository->updateEntry(entry);
    if (success && recordCommand) {
        m_redoStack.clear();
        emit entriesChanged();
    }
    return success;
}

std::unique_ptr<JournalEntry> JournalEntryController::getEntryByIdInternal(const QUuid& id) {
    return m_repository->getEntryById(id);
}

void JournalEntryController::executeCommand(std::unique_ptr<Command> command, bool record) {
    command->execute();
    if (record) {
        m_undoStack.push_back(std::move(command));
        m_redoStack.clear();
    }
    emit entriesChanged();
}