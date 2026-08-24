#include "RemoveCommand.h"
#include "JournalEntryController.h"
RemoveCommand::RemoveCommand(JournalEntryController* controller, const QUuid& idToRemove)
    : m_controller(controller), m_idToRemove(idToRemove), m_wasRemoved(false) {}

void RemoveCommand::execute() {
    if (m_controller) {
        m_removedEntry = m_controller->getEntryByIdInternal(m_idToRemove);
        if (m_removedEntry) {
            m_wasRemoved = m_controller->removeEntryInternal(m_idToRemove, false);
        } else {
            m_wasRemoved = false;
        }
    }
}

void RemoveCommand::undo() {
    if (m_controller && m_wasRemoved && m_removedEntry) {
        m_controller->addEntryInternal(*m_removedEntry, false);
    }
}
