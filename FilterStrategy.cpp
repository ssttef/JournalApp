#include "FilterStrategy.h"


DateFilterStrategy::DateFilterStrategy(const QDateTime& startDate, const QDateTime& endDate)
    : m_startDate(startDate), m_endDate(endDate) {}

bool DateFilterStrategy::matches(const JournalEntry& entry) const {
    return entry.getTimestamp() >= m_startDate && entry.getTimestamp() <= m_endDate;
}


TagFilterStrategy::TagFilterStrategy(const QString& tag)
    : m_tag(tag) {}

bool TagFilterStrategy::matches(const JournalEntry& entry) const {
    for (const QString& entryTag : entry.getTags()) {
        if (entryTag.compare(m_tag, Qt::CaseInsensitive) == 0) {
            return true;
        }
    }
    return false;
}

ContentFilterStrategy::ContentFilterStrategy(const QString& keyword, bool caseSensitive)
    : m_keyword(keyword), m_caseSensitivity(caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive) {}

bool ContentFilterStrategy::matches(const JournalEntry& entry) const {
    return entry.getTitle().contains(m_keyword, m_caseSensitivity) ||
           entry.getContent().contains(m_keyword, m_caseSensitivity);
}


AndFilterStrategy::AndFilterStrategy(std::unique_ptr<FilterStrategy> s1, std::unique_ptr<FilterStrategy> s2)
    : m_strategy1(std::move(s1)), m_strategy2(std::move(s2)) {}

bool AndFilterStrategy::matches(const JournalEntry& entry) const {
    return m_strategy1->matches(entry) && m_strategy2->matches(entry);
}


OrFilterStrategy::OrFilterStrategy(std::unique_ptr<FilterStrategy> s1, std::unique_ptr<FilterStrategy> s2)
    : m_strategy1(std::move(s1)), m_strategy2(std::move(s2)) {}

bool OrFilterStrategy::matches(const JournalEntry& entry) const {
    return m_strategy1->matches(entry) || m_strategy2->matches(entry);
}
