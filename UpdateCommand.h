#ifndef UPDATECOMMAND_H
#define UPDATECOMMAND_H

#include "Command.h"
#include "JournalEntry.h"
#include <memory>

class JournalEntryController;
class UpdateCommand : public Command {
public:
    UpdateCommand(JournalEntryController* controller, const JournalEntry& newEntry);
    void execute() override;
    void undo() override;

private:
    JournalEntryController* m_controller;
    JournalEntry m_newEntry;
    std::unique_ptr<JournalEntry> m_oldEntry;
    bool m_wasUpdated;
};

#endif
