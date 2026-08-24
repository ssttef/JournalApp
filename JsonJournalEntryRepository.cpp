#include "JsonJournalEntryRepository.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <stdexcept>
#include <QDateTime>
#include <QUuid>

JsonJournalEntryRepository::JsonJournalEntryRepository(const QString& filename)
    : m_filename(filename) {
    loadAllEntries();
}

JsonJournalEntryRepository::~JsonJournalEntryRepository() {
    saveAllEntries(m_entries);
}

bool JsonJournalEntryRepository::addEntry(const JournalEntry& entry) {
    for (const auto& existingEntry : m_entries) {
        if (existingEntry.getId() == entry.getId()) {
            qWarning() << "Attempted to add duplicate entry with ID:" << entry.getId();
            return false;
        }
    }
    m_entries.append(entry);
    return true;
}

bool JsonJournalEntryRepository::removeEntry(const QUuid& id) {
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries[i].getId() == id) {
            m_entries.removeAt(i);
            return true;
        }
    }
    qWarning() << "Attempted to remove non-existent entry with ID:" << id;
    return false;
}

bool JsonJournalEntryRepository::updateEntry(const JournalEntry& entry) {
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries[i].getId() == entry.getId()) {
            m_entries[i] = entry; // Overwrite
            return true;
        }
    }
    qWarning() << "Attempted to update non-existent entry with ID:" << entry.getId();
    return false;
}

std::unique_ptr<JournalEntry> JsonJournalEntryRepository::getEntryById(const QUuid& id) {
    for (const auto& entry : m_entries) {
        if (entry.getId() == id) {
            return std::make_unique<JournalEntry>(entry); // Return a copy
        }
    }
    return nullptr;
}

QVector<JournalEntry> JsonJournalEntryRepository::getAllEntries() {
    return m_entries;
}

bool JsonJournalEntryRepository::saveAllEntries(const QVector<JournalEntry>& entries) {
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qCritical() << "Could not open JSON file for writing:" << m_filename << file.errorString();
        return false;
    }

    QJsonArray jsonArray;
    for (const auto& entry : entries) {
        jsonArray.append(entryToJson(entry));
    }

    QJsonDocument doc(jsonArray);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool JsonJournalEntryRepository::loadAllEntries() {
    m_entries.clear();
    QFile file(m_filename);
    if (!file.exists()) {
        qInfo() << "JSON file does not exist, creating new:" << m_filename;
        return true;
    }
    if (!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not open JSON file for reading:" << m_filename << file.errorString();
        return false;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qCritical() << "Failed to parse JSON file:" << m_filename << parseError.errorString();
        return false;
    }

    if (!doc.isArray()) {
        qCritical() << "JSON document is not an array:" << m_filename;
        return false;
    }

    QJsonArray jsonArray = doc.array();
    for (const QJsonValue& value : jsonArray) {
        if (value.isObject()) {
            try {
                m_entries.append(jsonToEntry(value.toObject()));
            } catch (const std::exception& e) {
                qWarning() << "Error parsing JSON object:" << value.toObject() << "Error:" << e.what();
            }
        }
    }
    return true;
}

QJsonObject JsonJournalEntryRepository::entryToJson(const JournalEntry& entry) const {
    QJsonObject jsonObject;
    jsonObject["id"] = entry.getId().toString();
    jsonObject["title"] = entry.getTitle();
    jsonObject["content"] = entry.getContent();
    jsonObject["timestamp"] = entry.getTimestamp().toString(Qt::ISODate);

    QJsonArray tagsArray;
    for (const QString& tag : entry.getTags()) {
        tagsArray.append(tag);
    }
    jsonObject["tags"] = tagsArray;

    return jsonObject;
}

JournalEntry JsonJournalEntryRepository::jsonToEntry(const QJsonObject& jsonObject) const {
    QUuid id = QUuid(jsonObject["id"].toString());
    QString title = jsonObject["title"].toString();
    QString content = jsonObject["content"].toString();
    QDateTime timestamp = QDateTime::fromString(jsonObject["timestamp"].toString(), Qt::ISODate);

    QVector<QString> tags;
    QJsonArray tagsArray = jsonObject["tags"].toArray();
    for (const QJsonValue& value : tagsArray) {
        tags.append(value.toString());
    }

    return JournalEntry(id, title, content, timestamp, tags);
}
