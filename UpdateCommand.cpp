#include "UpdateCommand.h"
#include "JournalEntryController.h"

UpdateCommand::UpdateCommand(JournalEntryController* controller, const JournalEntry& newEntry)
    : m_controller(controller), m_newEntry(newEntry), m_wasUpdated(false) {}

void UpdateCommand::execute() {
    if (m_controller) {
        m_oldEntry = m_controller->getEntryByIdInternal(m_newEntry.getId());
        if (m_oldEntry) {
            m_wasUpdated = m_controller->updateEntryInternal(m_newEntry, false);
        } else {
            m_wasUpdated = false;
        }
    }
}

void UpdateCommand::undo() {
    if (m_controller && m_wasUpdated && m_oldEntry) {
        m_controller->updateEntryInternal(*m_oldEntry, false);
    }
}
