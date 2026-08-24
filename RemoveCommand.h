#ifndef REMOVECOMMAND_H
#define REMOVECOMMAND_H

#include "Command.h"
#include "JournalEntry.h"
#include <memory>

class JournalEntryController;

class RemoveCommand : public Command {
public:
    RemoveCommand(JournalEntryController* controller, const QUuid& idToRemove);
    void execute() override;
    void undo() override;

private:
    JournalEntryController* m_controller;
    QUuid m_idToRemove;
    std::unique_ptr<JournalEntry> m_removedEntry;
    bool m_wasRemoved;
};

#endif
