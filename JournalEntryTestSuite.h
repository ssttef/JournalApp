#ifndef JOURNALENTRYTESTSUITE_H
#define JOURNALENTRYTESTSUITE_H

#include <QObject>
#include <QtTest/QTest>
#include <memory>

class JournalEntry;
class JournalEntryController;
class JsonJournalEntryRepository;
class IJournalEntryRepository;
class FilterStrategy;
class AddCommand;
class RemoveCommand;
class UpdateCommand;


class JournalEntryTestSuite : public QObject
{
    Q_OBJECT

public slots:
    void init();
    void cleanup();

private slots:
    void testRepository_AddAndGetEntry();
    void testRepository_RemoveEntry();
    void testRepository_UpdateEntry();
    void testRepository_GetAllEntries();
    void testRepository_SaveAndLoadPersistence();
    void testController_AddEntry();
    void testController_RemoveEntry();
    void testController_UpdateEntry();
    void testController_GetAllEntries();
    void testController_UndoRedo_Add();
    void testController_UndoRedo_Remove();
    void testController_UndoRedo_Update();
    void testController_CanUndoRedoState();
    void testController_FilterByDate();
    void testController_FilterByTag();
    void testController_FilterByContent_CaseInsensitive();
    void testController_FilterByContent_CaseSensitive();
    void testController_FilterByAndStrategy();
    void testController_FilterByOrStrategy();


private:
    std::unique_ptr<JournalEntryController> m_controller;
    JsonJournalEntryRepository* m_repoInstance;

    const QString TEST_JSON_FILENAME = "test_journal_data.json";
};

#endif // JOURNALENTRYTESTSUITE_H