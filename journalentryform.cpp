#include "JournalEntryForm.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QdateTime>
#include <QstringList>

JournalEntryForm::JournalEntryForm(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Journal Entry");

    QFormLayout *formLayout = new QFormLayout(this);

    m_titleEdit = new QLineEdit(this);
    m_contentEdit = new QTextEdit(this);
    m_timestampEdit = new QDateTimeEdit(this);
    m_timestampEdit->setCalendarPopup(true);
    m_timestampEdit->setDateTime(QDateTime::currentDateTime());
    m_tagsEdit = new QLineEdit(this);

    formLayout->addRow("Title:", m_titleEdit);
    formLayout->addRow("Content:", m_contentEdit);
    formLayout->addRow("Timestamp:", m_timestampEdit);
    formLayout->addRow("Tags (comma-separated):", m_tagsEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_okButton = new QPushButton("OK", this);
    m_cancelButton = new QPushButton("Cancel", this);

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);

    formLayout->addRow(buttonLayout);

    connect(m_okButton, &QPushButton::clicked, this, &JournalEntryForm::accept);
    connect(m_cancelButton, &QPushButton::clicked, this, &JournalEntryForm::reject);

    resize(400, 300);
}

QString JournalEntryForm::getTitle() const {
    return m_titleEdit->text();
}

QString JournalEntryForm::getContent() const {
    return m_contentEdit->toPlainText();
}

QDateTime JournalEntryForm::getTimestamp() const {
    return m_timestampEdit->dateTime();
}

QVector<QString> JournalEntryForm::getTags() const {
    QStringList tagList = m_tagsEdit->text().split(",", Qt::SkipEmptyParts);
    QVector<QString> tags;
    for (const QString& tag : tagList) {
        tags.append(tag.trimmed());
    }
    return tags;
}

void JournalEntryForm::setEntry(const JournalEntry& entry) {
    m_titleEdit->setText(entry.getTitle());
    m_contentEdit->setText(entry.getContent());
    m_timestampEdit->setDateTime(entry.getTimestamp());
    m_tagsEdit->setText(entry.getTags().join(", "));
}
