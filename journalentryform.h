#ifndef JOURNALENTRYFORM_H
#define JOURNALENTRYFORM_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateTimeEdit>
#include <QVector>
#include <QString>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>

#include "JournalEntry.h"

class JournalEntryForm : public QDialog {
    Q_OBJECT
public:
    explicit JournalEntryForm(QWidget *parent = nullptr);

    QString getTitle() const;
    QString getContent() const;
    QDateTime getTimestamp() const;
    QVector<QString> getTags() const;

    void setEntry(const JournalEntry& entry);

private:
    QLineEdit* m_titleEdit;
    QTextEdit* m_contentEdit;
    QDateTimeEdit* m_timestampEdit;
    QLineEdit* m_tagsEdit;
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
};

#endif
