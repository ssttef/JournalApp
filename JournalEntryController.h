#ifndef JOURNALENTRYCONTROLLER_H
#define JOURNALENTRYCONTROLLER_H

#include <QObject>
#include <QUuid>
#include <QVector>
#include <memory>
#include <vector>
#include "JournalEntryRepository.h"
#include "JournalEntry.h"
#include "Command.h"
#include "FilterStrategy.h"

class JournalEntryController : public QObject {
    Q_OBJECT

public:
    explicit JournalEntryController(std::unique_ptr<IJournalEntryRepository> repository);
    ~JournalEntryController();
    bool addEntry(const JournalEntry& entry);
    bool removeEntry(const QUuid& id);
    bool updateEntry(const JournalEntry& entry);
    QVector<JournalEntry> getAllEntries();
    QVector<JournalEntry> getFilteredEntries(std::unique_ptr<FilterStrategy> filter);

    bool undo();
    bool redo();
    bool canUndo() const;
    bool canRedo() const;

    bool addEntryInternal(const JournalEntry& entry, bool recordCommand = true);
    bool removeEntryInternal(const QUuid& id, bool recordCommand = true);
    bool updateEntryInternal(const JournalEntry& entry, bool recordCommand = true);
    std::unique_ptr<JournalEntry> getEntryByIdInternal(const QUuid& id);

    void executeCommand(std::unique_ptr<Command> command, bool record = true);

    signals:
        void entriesChanged();

private:
    std::unique_ptr<IJournalEntryRepository> m_repository;

    std::vector<std::unique_ptr<Command>> m_undoStack;
    std::vector<std::unique_ptr<Command>> m_redoStack;
};

#endif // JOURNALENTRYCONTROLLER_H