#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QCheckBox>

#include "FilterStrategy.h"

class FilterDialog : public QDialog {
    Q_OBJECT
public:
    explicit FilterDialog(QWidget *parent = nullptr);

    std::unique_ptr<FilterStrategy> getFilterStrategy() const;

private slots:
    void onFilterTypeChanged();

private:
    QRadioButton* m_noFilterRadio;
    QRadioButton* m_dateFilterRadio;
    QRadioButton* m_tagFilterRadio;
    QRadioButton* m_contentFilterRadio;

    QDateTimeEdit* m_startDateEdit;
    QDateTimeEdit* m_endDateEdit;
    QLineEdit* m_tagEdit;
    QLineEdit* m_contentKeywordEdit;
    QCheckBox* m_contentCaseSensitiveCheck;

    QPushButton* m_okButton;
    QPushButton* m_cancelButton;

    void setupUI();
    void updateFilterWidgetsState();
};

#endif
