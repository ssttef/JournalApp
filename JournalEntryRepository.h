#ifndef JOURNALENTRYREPOSITORY_H
#define JOURNALENTRYREPOSITORY_H
#include "JournalEntry.h"
#include <QVector>
#include <QUuid>
#include <QString>
#include <memory>

class IJournalEntryRepository {
public:
    virtual ~IJournalEntryRepository() = default;

    virtual bool addEntry(const JournalEntry& entry) = 0;
    virtual bool removeEntry(const QUuid& id) = 0;
    virtual bool updateEntry(const JournalEntry& entry) = 0;
    virtual std::unique_ptr<JournalEntry> getEntryById(const QUuid& id) = 0;
    virtual QVector<JournalEntry> getAllEntries() = 0;
    virtual bool saveAllEntries(const QVector<JournalEntry>& entries) = 0;
    virtual bool loadAllEntries() = 0;
};

#endif // IJOURNALENTRYREPOSITORY_H
