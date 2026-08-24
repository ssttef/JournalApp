#include "MainWindow.h"
#include <QApplication>
#include <memory>
#include "JournalEntryController.h"
#include "JsonJournalEntryRepository.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::unique_ptr<IJournalEntryRepository> repository =
        std::make_unique<JsonJournalEntryRepository>("entries.json");
    std::unique_ptr<JournalEntryController> controller =
        std::make_unique<JournalEntryController>(std::move(repository));
    MainWindow w(std::move(controller));
    w.show();

    return a.exec();
}
