#include "JournalEntry.h"
JournalEntry::JournalEntry(const QUuid& id, const QString& title,
                           const QString& content, const QDateTime& timestamp,
                           const QVector<QString>& tags)
    : m_id(id),
    m_title(title),
    m_content(content),
    m_timestamp(timestamp),
    m_tags(tags) {}

QUuid JournalEntry::getId() const {
    return m_id;
}

QString JournalEntry::getTitle() const {
    return m_title;
}

QString JournalEntry::getContent() const {
    return m_content;
}

QDateTime JournalEntry::getTimestamp() const {
    return m_timestamp;
}

QVector<QString> JournalEntry::getTags() const {
    return m_tags;
}

void JournalEntry::setTitle(const QString& title) {
    m_title = title;
}

void JournalEntry::setContent(const QString& content) {
    m_content = content;
}

void JournalEntry::setTimestamp(const QDateTime& timestamp) {
    m_timestamp = timestamp;
}

void JournalEntry::setTags(const QVector<QString>& tags) {
    m_tags = tags;
}

bool JournalEntry::operator==(const JournalEntry& other) const {
    return m_id == other.m_id;
}
