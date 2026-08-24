#ifndef JSONJOURNALENTRYREPOSITORY_H
#define JSONJOURNALENTRYREPOSITORY_H

#include "JournalEntryRepository.h"
#include <QString>
#include <QVector>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class JsonJournalEntryRepository : public IJournalEntryRepository {
public:
    explicit JsonJournalEntryRepository(const QString& filename);
    ~JsonJournalEntryRepository() override;

    bool addEntry(const JournalEntry& entry) override;
    bool removeEntry(const QUuid& id) override;
    bool updateEntry(const JournalEntry& entry) override;
    std::unique_ptr<JournalEntry> getEntryById(const QUuid& id) override;
    QVector<JournalEntry> getAllEntries() override;
    bool saveAllEntries(const QVector<JournalEntry>& entries) override;
    bool loadAllEntries() override;

private:
    QString m_filename;
    QVector<JournalEntry> m_entries;
    QJsonObject entryToJson(const JournalEntry& entry) const;
    JournalEntry jsonToEntry(const QJsonObject& jsonObject) const;
};

#endif// JSONJOURNALENTRYREPOSITORY_H
