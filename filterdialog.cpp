#include "FilterDialog.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QDateTime>


FilterDialog::FilterDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Apply Filter");
    setupUI();
    updateFilterWidgetsState();
    connect(m_noFilterRadio, &QRadioButton::toggled, this, &FilterDialog::onFilterTypeChanged);
    connect(m_dateFilterRadio, &QRadioButton::toggled, this, &FilterDialog::onFilterTypeChanged);
    connect(m_tagFilterRadio, &QRadioButton::toggled, this, &FilterDialog::onFilterTypeChanged);
    connect(m_contentFilterRadio, &QRadioButton::toggled, this, &FilterDialog::onFilterTypeChanged);
}

void FilterDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *filterTypeGroup = new QGroupBox("Filter Type", this);
    QVBoxLayout *filterTypeLayout = new QVBoxLayout(filterTypeGroup);
    m_noFilterRadio = new QRadioButton("No Filter", this);
    m_dateFilterRadio = new QRadioButton("Filter by Date Range", this);
    m_tagFilterRadio = new QRadioButton("Filter by Tag", this);
    m_contentFilterRadio = new QRadioButton("Filter by Content Keyword", this);
    m_noFilterRadio->setChecked(true);
    filterTypeLayout->addWidget(m_noFilterRadio);
    filterTypeLayout->addWidget(m_dateFilterRadio);
    filterTypeLayout->addWidget(m_tagFilterRadio);
    filterTypeLayout->addWidget(m_contentFilterRadio);
    mainLayout->addWidget(filterTypeGroup);

    QGroupBox *filterDetailsGroup = new QGroupBox("Filter Details", this);
    QFormLayout *filterDetailsLayout = new QFormLayout(filterDetailsGroup);
    m_startDateEdit = new QDateTimeEdit(this);
    m_startDateEdit->setCalendarPopup(true);
    m_startDateEdit->setDateTime(QDateTime::currentDateTime().addMonths(-1));
    m_endDateEdit = new QDateTimeEdit(this);
    m_endDateEdit->setCalendarPopup(true);
    m_endDateEdit->setDateTime(QDateTime::currentDateTime());
    m_tagEdit = new QLineEdit(this);
    m_contentKeywordEdit = new QLineEdit(this);
    m_contentCaseSensitiveCheck = new QCheckBox("Case Sensitive", this);

    filterDetailsLayout->addRow("Start Date:", m_startDateEdit);
    filterDetailsLayout->addRow("End Date:", m_endDateEdit);
    filterDetailsLayout->addRow("Tag:", m_tagEdit);
    filterDetailsLayout->addRow("Keyword:", m_contentKeywordEdit);
    filterDetailsLayout->addRow("", m_contentCaseSensitiveCheck);
    mainLayout->addWidget(filterDetailsGroup);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_okButton = new QPushButton("Apply", this);
    m_cancelButton = new QPushButton("Cancel", this);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(m_okButton, &QPushButton::clicked, this, &FilterDialog::accept);
    connect(m_cancelButton, &QPushButton::clicked, this, &FilterDialog::reject);
}

void FilterDialog::onFilterTypeChanged() {
    updateFilterWidgetsState();
}

void FilterDialog::updateFilterWidgetsState() {
    m_startDateEdit->setEnabled(m_dateFilterRadio->isChecked());
    m_endDateEdit->setEnabled(m_dateFilterRadio->isChecked());
    m_tagEdit->setEnabled(m_tagFilterRadio->isChecked());
    m_contentKeywordEdit->setEnabled(m_contentFilterRadio->isChecked());
    m_contentCaseSensitiveCheck->setEnabled(m_contentFilterRadio->isChecked());
}

std::unique_ptr<FilterStrategy> FilterDialog::getFilterStrategy() const {
    if (m_noFilterRadio->isChecked()) {
        return nullptr;
    } else if (m_dateFilterRadio->isChecked()) {
        return std::make_unique<DateFilterStrategy>(m_startDateEdit->dateTime(), m_endDateEdit->dateTime());
    } else if (m_tagFilterRadio->isChecked()) {
        return std::make_unique<TagFilterStrategy>(m_tagEdit->text());
    } else if (m_contentFilterRadio->isChecked()) {
        return std::make_unique<ContentFilterStrategy>(m_contentKeywordEdit->text(), m_contentCaseSensitiveCheck->isChecked());
    }
    return nullptr;
}
