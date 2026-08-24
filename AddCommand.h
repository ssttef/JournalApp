#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include "Command.h"
#include "JournalEntry.h"
#include <memory>

class JournalEntryController;

class AddCommand : public Command {
public:
    AddCommand(JournalEntryController* controller, const JournalEntry& entryToAdd);
    void execute() override;
    void undo() override;

private:
    JournalEntryController* m_controller;
    JournalEntry m_entryToAdd;
    bool m_wasAdded;
};

#endif
