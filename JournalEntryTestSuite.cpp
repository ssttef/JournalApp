#include "JournalEntryTestSuite.h"
#include "JournalEntryController.h"
#include "JsonJournalEntryRepository.h"
#include "JournalEntry.h"
#include "FilterStrategy.h"
#include "AddCommand.h"
#include "RemoveCommand.h"
#include "UpdateCommand.h"



#include <QFile>
#include <QUuid>
#include <QDateTime>
#include <QDebug>
void JournalEntryTestSuite::init() {
    if (QFile::exists(TEST_JSON_FILENAME)) {
        QFile::remove(TEST_JSON_FILENAME);
    }
    m_repoInstance = new JsonJournalEntryRepository(TEST_JSON_FILENAME);
    m_controller = std::make_unique<JournalEntryController>(
        std::unique_ptr<IJournalEntryRepository>(m_repoInstance)
    );
}

void JournalEntryTestSuite::cleanup() {
    m_controller.reset();
    if (QFile::exists(TEST_JSON_FILENAME)) {
        QFile::remove(TEST_JSON_FILENAME);
    }
}

void JournalEntryTestSuite::testRepository_AddAndGetEntry() {
    JsonJournalEntryRepository repo(TEST_JSON_FILENAME);
    JournalEntry entry(QUuid::createUuid(), "Repo Add Title", "Repo Add Content", QDateTime::currentDateTime(), {"tag1", "tag2"});

    QVERIFY(repo.addEntry(entry));
    QCOMPARE(repo.getAllEntries().size(), 1);

    std::unique_ptr<JournalEntry> retrieved = repo.getEntryById(entry.getId());
    QVERIFY(retrieved != nullptr);
    QCOMPARE(retrieved->getTitle(), entry.getTitle());
    QCOMPARE(retrieved->getContent(), entry.getContent());
    QCOMPARE(retrieved->getTags(), entry.getTags());

    QVERIFY(!repo.addEntry(entry));
    QCOMPARE(repo.getAllEntries().size(), 1);
}

void JournalEntryTestSuite::testRepository_RemoveEntry() {
    JsonJournalEntryRepository repo(TEST_JSON_FILENAME);
    JournalEntry entry1(QUuid::createUuid(), "Remove Test 1", "Content 1", QDateTime::currentDateTime());
    JournalEntry entry2(QUuid::createUuid(), "Remove Test 2", "Content 2", QDateTime::currentDateTime());
    repo.addEntry(entry1);
    repo.addEntry(entry2);
    QCOMPARE(repo.getAllEntries().size(), 2);

    QVERIFY(repo.removeEntry(entry1.getId()));
    QCOMPARE(repo.getAllEntries().size(), 1);
    QVERIFY(repo.getEntryById(entry1.getId()) == nullptr);
    QVERIFY(repo.getEntryById(entry2.getId()) != nullptr);

    QVERIFY(!repo.removeEntry(QUuid::createUuid()));
    QCOMPARE(repo.getAllEntries().size(), 1);
}

void JournalEntryTestSuite::testRepository_UpdateEntry() {
    JsonJournalEntryRepository repo(TEST_JSON_FILENAME);
    JournalEntry originalEntry(QUuid::createUuid(), "Old Title", "Old Content", QDateTime::currentDateTime(), {"old"});
    repo.addEntry(originalEntry);
    QCOMPARE(repo.getAllEntries().size(), 1);

    JournalEntry updatedEntry = originalEntry;
    updatedEntry.setTitle("New Title");
    updatedEntry.setContent("New Content");
    updatedEntry.setTags({"new", "updated"});

    QVERIFY(repo.updateEntry(updatedEntry));
    QCOMPARE(repo.getAllEntries().size(), 1);
    std::unique_ptr<JournalEntry> retrieved = repo.getEntryById(originalEntry.getId());
    QVERIFY(retrieved != nullptr);
    QCOMPARE(retrieved->getTitle(), "New Title");
    QCOMPARE(retrieved->getContent(), "New Content");
    QCOMPARE(retrieved->getTags(), QVector<QString>({"new", "updated"}));

    JournalEntry nonExistent(QUuid::createUuid(), "X", "Y", QDateTime::currentDateTime());
    QVERIFY(!repo.updateEntry(nonExistent));
}

void JournalEntryTestSuite::testRepository_GetAllEntries() {
    JsonJournalEntryRepository repo(TEST_JSON_FILENAME);
    QCOMPARE(repo.getAllEntries().size(), 0);

    JournalEntry entry1(QUuid::createUuid(), "A", "C", QDateTime::currentDateTime());
    JournalEntry entry2(QUuid::createUuid(), "B", "D", QDateTime::currentDateTime());
    repo.addEntry(entry1);
    repo.addEntry(entry2);

    QVector<JournalEntry> allEntries = repo.getAllEntries();
    QCOMPARE(allEntries.size(), 2);
    QVERIFY(allEntries.contains(entry1));
    QVERIFY(allEntries.contains(entry2));
}

void JournalEntryTestSuite::testRepository_SaveAndLoadPersistence() {
    JournalEntry entry1(QUuid::createUuid(), "JSON Persist Title 1", "JSON Persist Content 1", QDateTime::currentDateTime().addDays(-1), {"day1"});
    JournalEntry entry2(QUuid::createUuid(), "JSON Persist Title 2", "JSON Persist Content 2", QDateTime::currentDateTime(), {"day2"});
    {
        JsonJournalEntryRepository repo1(TEST_JSON_FILENAME);
        repo1.addEntry(entry1);
        repo1.addEntry(entry2);
        QCOMPARE(repo1.getAllEntries().size(), 2);
        QVERIFY(repo1.saveAllEntries(repo1.getAllEntries()));
    }

    JsonJournalEntryRepository repo2(TEST_JSON_FILENAME);
    QVERIFY(repo2.loadAllEntries());

    QVector<JournalEntry> loadedEntries = repo2.getAllEntries();
    QCOMPARE(loadedEntries.size(), 2);

    bool foundOriginalEntry1 = false;
    bool foundOriginalEntry2 = false;

    for (const JournalEntry& loadedEntry : loadedEntries) {
        if (loadedEntry.getId() == entry1.getId() &&
            loadedEntry.getTitle() == entry1.getTitle() &&
            loadedEntry.getContent() == entry1.getContent()) {
            foundOriginalEntry1 = true;
        }
        if (loadedEntry.getId() == entry2.getId() &&
            loadedEntry.getTitle() == entry2.getTitle() &&
            loadedEntry.getContent() == entry2.getContent()) {
            foundOriginalEntry2 = true;
        }
    }
    QVERIFY(foundOriginalEntry1);
    QVERIFY(foundOriginalEntry2);
}


void JournalEntryTestSuite::testController_AddEntry() {
    JournalEntry newEntry(QUuid::createUuid(), "Ctrl Add Test", "Content", QDateTime::currentDateTime(), {"test"});

    QVERIFY(m_controller->addEntry(newEntry));
    QCOMPARE(m_controller->getAllEntries().size(), 1);
    QCOMPARE(m_controller->getAllEntries().first().getTitle(), newEntry.getTitle());
    QCOMPARE(m_controller->getEntryByIdInternal(newEntry.getId())->getId(), newEntry.getId());
}

void JournalEntryTestSuite::testController_RemoveEntry() {
    JournalEntry entry1(QUuid::createUuid(), "Ctrl Remove Test 1", "Content1", QDateTime::currentDateTime());
    JournalEntry entry2(QUuid::createUuid(), "Ctrl Remove Test 2", "Content2", QDateTime::currentDateTime());
    m_controller->addEntry(entry1);
    m_controller->addEntry(entry2);
    QCOMPARE(m_controller->getAllEntries().size(), 2);

    QVERIFY(m_controller->removeEntry(entry1.getId()));
    QCOMPARE(m_controller->getAllEntries().size(), 1);
    QVERIFY(m_controller->getAllEntries().first().getId() == entry2.getId());
}

void JournalEntryTestSuite::testController_UpdateEntry() {
    JournalEntry originalEntry(QUuid::createUuid(), "Ctrl Original", "Content", QDateTime::currentDateTime(), {"old"});
    m_controller->addEntry(originalEntry);
    QCOMPARE(m_controller->getAllEntries().size(), 1);

    JournalEntry updatedEntry = originalEntry;
    updatedEntry.setTitle("New Title");
    updatedEntry.setContent("New Content");
    updatedEntry.setTags({"new", "updated"});

    QVERIFY(m_controller->updateEntry(updatedEntry));
    QCOMPARE(m_controller->getAllEntries().size(), 1);
    std::unique_ptr<JournalEntry> retrieved = m_controller->getEntryByIdInternal(originalEntry.getId());
    QVERIFY(retrieved != nullptr);
    QCOMPARE(retrieved->getTitle(), "Ctrl Updated");
    QCOMPARE(retrieved->getContent(), "New Content");
    QCOMPARE(retrieved->getTags(), QVector<QString>({"new", "updated"}));
}

void JournalEntryTestSuite::testController_GetAllEntries() {
    QCOMPARE(m_controller->getAllEntries().size(), 0);

    JournalEntry entry1(QUuid::createUuid(), "Entry A", "Content A", QDateTime::currentDateTime());
    JournalEntry entry2(QUuid::createUuid(), "Entry B", "Content B", QDateTime::currentDateTime());
    m_controller->addEntry(entry1);
    m_controller->addEntry(entry2);

    QCOMPARE(m_controller->getAllEntries().size(), 2);
    QVERIFY(m_controller->getAllEntries().contains(entry1));
    QVERIFY(m_controller->getAllEntries().contains(entry2));
}

void JournalEntryTestSuite::testController_UndoRedo_Add() {
    JournalEntry newEntry(QUuid::createUuid(), "Add for Undo", "Content", QDateTime::currentDateTime(), {"tag"});

    QVERIFY(m_controller->addEntry(newEntry));
    QCOMPARE(m_controller->getAllEntries().size(), 1);
    QVERIFY(m_controller->canUndo());
    QVERIFY(!m_controller->canRedo());

    QVERIFY(m_controller->undo());
    QCOMPARE(m_controller->getAllEntries().size(), 0);
    QVERIFY(!m_controller->canUndo());
    QVERIFY(m_controller->canRedo());

    QVERIFY(m_controller->redo());
    QCOMPARE(m_controller->getAllEntries().size(), 1);
    QVERIFY(m_controller->getAllEntries().first().getId() == newEntry.getId());
    QVERIFY(m_controller->canUndo());
    QVERIFY(!m_controller->canRedo());
}

void JournalEntryTestSuite::testController_UndoRedo_Remove() {
    JournalEntry entry1(QUuid::createUuid(), "Undo Remove Test 1", "Content1", QDateTime::currentDateTime());
    JournalEntry entry2(QUuid::createUuid(), "Undo Remove Test 2", "Content2", QDateTime::currentDateTime());
    m_controller->addEntry(entry1);
    m_controller->addEntry(entry2);
    QCOMPARE(m_controller->getAllEntries().size(), 2);

    QVERIFY(m_controller->removeEntry(entry1.getId()));
    QCOMPARE(m_controller->getAllEntries().size(), 1);
    QVERIFY(m_controller->canUndo());

    QVERIFY(m_controller->undo());
    QCOMPARE(m_controller->getAllEntries().size(), 2);
    QVERIFY(m_controller->getAllEntries().contains(entry1));
    QVERIFY(m_controller->canRedo());

    QVERIFY(m_controller->redo());
    QCOMPARE(m_controller->getAllEntries().size(), 1);
    QVERIFY(!m_controller->getAllEntries().contains(entry1));
}

void JournalEntryTestSuite::testController_UndoRedo_Update() {
    JournalEntry originalEntry(QUuid::createUuid(), "Undo Update Original", "Content", QDateTime::currentDateTime(), {"tag"});
    m_controller->addEntry(originalEntry);
    QCOMPARE(m_controller->getAllEntries().size(), 1);

    JournalEntry updatedEntry = originalEntry;
    updatedEntry.setTitle("Undo Update New Title");
    updatedEntry.setContent("New Content");

    QVERIFY(m_controller->updateEntry(updatedEntry));
    QCOMPARE(m_controller->getAllEntries().first().getTitle(), "Undo Update New Title");
    QVERIFY(m_controller->canUndo());

    QVERIFY(m_controller->undo());
    QCOMPARE(m_controller->getAllEntries().first().getTitle(), "Undo Update Original");
    QVERIFY(m_controller->canRedo());

    QVERIFY(m_controller->redo());
    QCOMPARE(m_controller->getAllEntries().first().getTitle(), "Undo Update New Title");
    QVERIFY(m_controller->canUndo());
}

void JournalEntryTestSuite::testController_CanUndoRedoState() {
    QVERIFY(!m_controller->canUndo());
    QVERIFY(!m_controller->canRedo());

    JournalEntry entry(QUuid::createUuid(), "Test CanUndoRedo", "Content", QDateTime::currentDateTime());
    m_controller->addEntry(entry);
    QVERIFY(m_controller->canUndo());
    QVERIFY(!m_controller->canRedo());

    m_controller->undo();
    QVERIFY(!m_controller->canUndo());
    QVERIFY(m_controller->canRedo());

    m_controller->redo();
    QVERIFY(m_controller->canUndo());
    QVERIFY(!m_controller->canRedo());

    JournalEntry newEntry(QUuid::createUuid(), "Another Entry", "Content", QDateTime::currentDateTime());
    m_controller->addEntry(newEntry);
    QVERIFY(!m_controller->canRedo());
}

void JournalEntryTestSuite::testController_FilterByDate() {
    JournalEntry entry1(QUuid::createUuid(), "Date Test E1", "Content E1", QDateTime(QDate(2023, 1, 1), QTime(12, 0, 0)));
    JournalEntry entry2(QUuid::createUuid(), "Date Test E2", "Content E2", QDateTime(QDate(2023, 1, 15), QTime(12, 0, 0)));
    JournalEntry entry3(QUuid::createUuid(), "Date Test E3", "Content E3", QDateTime(QDate(2023, 2, 1), QTime(12, 0, 0)));

    m_controller->addEntry(entry1);
    m_controller->addEntry(entry2);
    m_controller->addEntry(entry3);
    QCOMPARE(m_controller->getAllEntries().count(), 3);
    std::unique_ptr<FilterStrategy> filter = std::make_unique<DateFilterStrategy>(
        QDateTime(QDate(2023, 1, 10), QTime(0, 0, 0)),
        QDateTime(QDate(2023, 1, 20), QTime(23, 59, 59))
        );
    QVector<JournalEntry> filtered = m_controller->getFilteredEntries(std::move(filter));
    QCOMPARE(filtered.size(), 1);
    QVERIFY(filtered.contains(entry2));
}

void JournalEntryTestSuite::testController_FilterByTag() {
    JournalEntry entry1(QUuid::createUuid(), "Tag Test E1", "C1", QDateTime::currentDateTime(), {"work", "meeting"});
    JournalEntry entry2(QUuid::createUuid(), "Tag Test E2", "C2", QDateTime::currentDateTime(), {"personal", "thoughts"});
    JournalEntry entry3(QUuid::createUuid(), "Tag Test E3", "C3", QDateTime::currentDateTime(), {"work", "project"});

    m_controller->addEntry(entry1);
    m_controller->addEntry(entry2);
    m_controller->addEntry(entry3);
    QCOMPARE(m_controller->getAllEntries().count(), 3);
    std::unique_ptr<FilterStrategy> filter = std::make_unique<TagFilterStrategy>("work");
    QVector<JournalEntry> filtered = m_controller->getFilteredEntries(std::move(filter));
    QCOMPARE(filtered.size(), 2);
    QVERIFY(filtered.contains(entry1));
    QVERIFY(filtered.contains(entry3));
}

void JournalEntryTestSuite::testController_FilterByContent_CaseInsensitive() {
    JournalEntry entry1(QUuid::createUuid(), "Content Test E1", "Discussed features.", QDateTime::currentDateTime());
    JournalEntry entry2(QUuid::createUuid(), "Content Test E2", "Thinking about Features.", QDateTime::currentDateTime());
    JournalEntry entry3(QUuid::createUuid(), "Content Test E3", "No match here.", QDateTime::currentDateTime());

    m_controller->addEntry(entry1);
    m_controller->addEntry(entry2);
    m_controller->addEntry(entry3);
    QCOMPARE(m_controller->getAllEntries().count(), 3);
    std::unique_ptr<FilterStrategy> filter = std::make_unique<ContentFilterStrategy>("features", false); // Case-insensitive
    QVector<JournalEntry> filtered = m_controller->getFilteredEntries(std::move(filter));
    QCOMPARE(filtered.size(), 2);
    QVERIFY(filtered.contains(entry1));
    QVERIFY(filtered.contains(entry2));
}

void JournalEntryTestSuite::testController_FilterByContent_CaseSensitive() {
    JournalEntry entry1(QUuid::createUuid(), "Content Test S1", "Discussed Features.", QDateTime::currentDateTime());
    JournalEntry entry2(QUuid::createUuid(), "Content Test S2", "Thinking about features.", QDateTime::currentDateTime());
    JournalEntry entry3(QUuid::createUuid(), "Content Test S3", "FEATURES IS KEY.", QDateTime::currentDateTime());

    m_controller->addEntry(entry1);
    m_controller->addEntry(entry2);
    m_controller->addEntry(entry3);
    QCOMPARE(m_controller->getAllEntries().count(), 3);

    std::unique_ptr<FilterStrategy> filter = std::make_unique<ContentFilterStrategy>("Features", true); // Case-sensitive
    QVector<JournalEntry> filtered = m_controller->getFilteredEntries(std::move(filter));
    QCOMPARE(filtered.size(), 1);
    QVERIFY(filtered.contains(entry1));
}

void JournalEntryTestSuite::testController_FilterByAndStrategy() {
    JournalEntry entry1(QUuid::createUuid(), "Meeting", "Project Alpha features", QDateTime(QDate(2023, 1, 10), QTime(10,0,0)), {"work", "alpha"});
    JournalEntry entry2(QUuid::createUuid(), "Brainstorm", "Alpha next steps", QDateTime(QDate(2023, 1, 15), QTime(11,0,0)), {"work", "brainstorm"});
    JournalEntry entry3(QUuid::createUuid(), "Personal", "Thoughts on life", QDateTime(QDate(2023, 1, 12), QTime(9,0,0)), {"personal"});

    m_controller->addEntry(entry1);
    m_controller->addEntry(entry2);
    m_controller->addEntry(entry3);
    QCOMPARE(m_controller->getAllEntries().count(), 3);

    std::unique_ptr<FilterStrategy> dateFilter = std::make_unique<DateFilterStrategy>(
        QDateTime(QDate(2023, 1, 5), QTime(0,0,0)), QDateTime(QDate(2023, 1, 12), QTime(23,59,59)));
    std::unique_ptr<FilterStrategy> tagFilter = std::make_unique<TagFilterStrategy>("work");

    std::unique_ptr<FilterStrategy> andFilter = std::make_unique<AndFilterStrategy>(
        std::move(dateFilter), std::move(tagFilter));

    QVector<JournalEntry> filtered = m_controller->getFilteredEntries(std::move(andFilter));
    QCOMPARE(filtered.size(), 1);
    QVERIFY(filtered.contains(entry1));
}

void JournalEntryTestSuite::testController_FilterByOrStrategy() {
    JournalEntry entry1(QUuid::createUuid(), "Meeting", "Project Alpha features", QDateTime(QDate(2023, 1, 10), QTime(10,0,0)), {"work", "alpha"});
    JournalEntry entry2(QUuid::createUuid(), "Brainstorm", "Alpha next steps", QDateTime(QDate(2023, 2, 15), QTime(11,0,0)), {"personal", "brainstorm"});
    JournalEntry entry3(QUuid::createUuid(), "Personal", "Thoughts on life", QDateTime(QDate(2023, 3, 12), QTime(9,0,0)), {"work"});

    m_controller->addEntry(entry1);
    m_controller->addEntry(entry2);
    m_controller->addEntry(entry3);
    QCOMPARE(m_controller->getAllEntries().count(), 3);

    std::unique_ptr<FilterStrategy> tagFilter1 = std::make_unique<TagFilterStrategy>("personal");
    std::unique_ptr<FilterStrategy> tagFilter2 = std::make_unique<TagFilterStrategy>("alpha");

    std::unique_ptr<FilterStrategy> orFilter = std::make_unique<OrFilterStrategy>(
        std::move(tagFilter1), std::move(tagFilter2));

    QVector<JournalEntry> filtered = m_controller->getFilteredEntries(std::move(orFilter));
    QCOMPARE(filtered.size(), 2);
    QVERIFY(filtered.contains(entry1));
    QVERIFY(filtered.contains(entry2));
}

QTEST_APPLESS_MAIN(JournalEntryTestSuite)