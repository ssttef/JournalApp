# JournalApp

A desktop journal application built with C++20 and Qt6, featuring undo/redo functionality, dynamic filtering, and JSON data persistence. The project is designed with a strong focus on clean architecture and modern software design patterns, making it highly maintainable and extensible.

##  Features

* **Entry Management:** Create, edit, and remove journal entries with titles, rich content, tags, and timestamps.
* **Undo/Redo Stack:** Fully functional undo and redo capabilities for all modifications (add, edit, remove), implemented using the Command pattern.
* **Dynamic Filtering:** Filter your journal entries by date ranges, specific tags, or content keywords (with case-sensitivity options). Features logical AND/OR filter combinations.
* **Data Persistence:** Automatically loads and saves your journal entries locally to a JSON file.
* **Clean UI:** A responsive, intuitive desktop interface built with Qt Widgets.

##  Tech Stack

* **Language:** C++20
* **Framework:** Qt 6 (Core, Gui, Widgets, Test)
* **Build System:** CMake
* **Data Storage:** JSON (Qt JSON module)

##  Architecture & Design Patterns

This project was built to demonstrate professional software engineering principles:

* **Model-View-Controller (MVC):** Strict decoupling of the UI (`MainWindow`) from the application logic (`JournalEntryController`).
* **Repository Pattern:** Abstract data access (`IJournalEntryRepository`) with a concrete JSON implementation (`JsonJournalEntryRepository`). This allows for easy swapping to a database (e.g., SQLite) in the future without changing the business logic.
* **Command Pattern:** Encapsulates operations (`AddCommand`, `RemoveCommand`, `UpdateCommand`) as objects, allowing them to be pushed onto a stack to provide robust Undo/Redo functionality.
* **Strategy Pattern:** Defines a family of filtering algorithms (`FilterStrategy`, `DateFilterStrategy`, `TagFilterStrategy`, etc.) making the filtering system highly extensible.

##  Build Instructions

### Prerequisites
* CMake (3.31 or higher)
* A C++20 compatible compiler (e.g., GCC, Clang, MSVC, MinGW)
* Qt 6 installed (with Core, Gui, Widgets, and Test modules)

### Compilation Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/ssttef/JournalApp.git
   cd JournalApp
   ```
2. Create a build directory and run CMake:
   ```bash
   mkdir build
   cd build
   cmake -DCMAKE_PREFIX_PATH="<path-to-your-qt6-installation>" ..
   ```
3. Build the project:
   ```bash
   cmake --build .
   ```
4. Run the executable:
   ```bash
   ./JournalApp
   ```

##  Testing

The logic and data layers are heavily tested using the `QtTest` framework to ensure reliability and prevent regressions.

To run the test suite:
1. Navigate to your build directory.
2. Run the test executable:
   ```bash
   ctest --output-on-failure
   ```
   *Alternatively, directly run `./JournalAppTests`*

