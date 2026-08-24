#ifndef JOURNALENTRY_H
#define JOURNALENTRY_H
#include <QString>
#include <QDateTime>
#include <QUuid>
#include <QVector>

class JournalEntry {
public:
    JournalEntry(const QUuid& id = QUuid::createUuid(),
                 const QString& title = "",
                 const QString& content = "",
                 const QDateTime& timestamp = QDateTime::currentDateTime(),
                 const QVector<QString>& tags = {});

    QUuid getId() const;
    QString getTitle() const;
    QString getContent() const;
    QDateTime getTimestamp() const;
    QVector<QString> getTags() const;

    void setTitle(const QString& title);
    void setContent(const QString& content);
    void setTimestamp(const QDateTime& timestamp);
    void setTags(const QVector<QString>& tags);

    // For comparison and uniqueness
    bool operator==(const JournalEntry& other) const;

private:
    QUuid m_id;
    QString m_title;
    QString m_content;
    QDateTime m_timestamp;
    QVector<QString> m_tags;
};


#endif

