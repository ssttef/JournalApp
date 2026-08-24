#ifndef FILTERSTRATEGY_H
#define FILTERSTRATEGY_H

#include "JournalEntry.h"
#include <QVector>
#include <QDateTime>
#include <QString>
#include <memory>

class FilterStrategy {
public:
    virtual ~FilterStrategy() = default;
    virtual bool matches(const JournalEntry& entry) const = 0;
    virtual std::unique_ptr<FilterStrategy> clone() const = 0;
};

class DateFilterStrategy : public FilterStrategy {
public:
    DateFilterStrategy(const QDateTime& startDate, const QDateTime& endDate);
    bool matches(const JournalEntry& entry) const override;
    std::unique_ptr<FilterStrategy> clone() const override {
        return std::make_unique<DateFilterStrategy>(m_startDate, m_endDate);
    }

private:
    QDateTime m_startDate;
    QDateTime m_endDate;
};

class TagFilterStrategy : public FilterStrategy {
public:
    TagFilterStrategy(const QString& tag);
    bool matches(const JournalEntry& entry) const override;
    std::unique_ptr<FilterStrategy> clone() const override {
        return std::make_unique<TagFilterStrategy>(m_tag);
    }


private:
    QString m_tag;
};

class ContentFilterStrategy : public FilterStrategy {
public:
    ContentFilterStrategy(const QString& keyword, bool caseSensitive = false);
    bool matches(const JournalEntry& entry) const override;
    std::unique_ptr<FilterStrategy> clone() const override {
        return std::make_unique<ContentFilterStrategy>(m_keyword, m_caseSensitivity == Qt::CaseSensitive);
    }

private:
    QString m_keyword;
    Qt::CaseSensitivity m_caseSensitivity;
};
class AndFilterStrategy : public FilterStrategy {
public:
    AndFilterStrategy(std::unique_ptr<FilterStrategy> s1, std::unique_ptr<FilterStrategy> s2);
    bool matches(const JournalEntry& entry) const override;
    std::unique_ptr<FilterStrategy> clone() const override {
        return std::make_unique<AndFilterStrategy>(m_strategy1->clone(), m_strategy2->clone());
    }

private:
    std::unique_ptr<FilterStrategy> m_strategy1;
    std::unique_ptr<FilterStrategy> m_strategy2;
};

class OrFilterStrategy : public FilterStrategy {
public:
    OrFilterStrategy(std::unique_ptr<FilterStrategy> s1, std::unique_ptr<FilterStrategy> s2);
    bool matches(const JournalEntry& entry) const override;
    std::unique_ptr<FilterStrategy> clone() const override {
        return std::make_unique<OrFilterStrategy>(m_strategy1->clone(), m_strategy2->clone());
    }

private:
    std::unique_ptr<FilterStrategy> m_strategy1;
    std::unique_ptr<FilterStrategy> m_strategy2;
};

#endif
