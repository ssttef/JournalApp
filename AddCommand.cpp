#include "AddCommand.h"
#include "JournalEntryController.h"

AddCommand::AddCommand(JournalEntryController* controller, const JournalEntry& entryToAdd)
    : m_controller(controller), m_entryToAdd(entryToAdd), m_wasAdded(false) {}

void AddCommand::execute() {
    if (m_controller) {
        m_wasAdded = m_controller->addEntryInternal(m_entryToAdd, false);
    }
}

void AddCommand::undo() {
    if (m_controller && m_wasAdded) {
        m_controller->removeEntryInternal(m_entryToAdd.getId(), false);
    }
}