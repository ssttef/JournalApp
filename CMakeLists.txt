cmake_minimum_required(VERSION 3.31)
project(JournalApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Test
        REQUIRED)
add_executable(JournalApp
        main.cpp
        AddCommand.h
        AddCommand.cpp
        Command.h
        filterdialog.cpp
        filterdialog.h
        filterdialog.ui
        FilterStrategy.cpp
        FilterStrategy.h
        JournalEntry.h
        JournalEntry.cpp
        JournalEntryController.cpp
        JournalEntryController.h
        journalentryform.cpp
        journalentryform.h
        journalentryform.ui
        JournalEntryRepository.h
        JsonJournalEntryRepository.h
        JsonJournalEntryRepository.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        RemoveCommand.cpp
        RemoveCommand.h
        UpdateCommand.h
        UpdateCommand.cpp
)

target_include_directories(JournalApp PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/JournalApp_autogen/include
)

target_link_libraries(JournalApp
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

add_executable(JournalAppTests
        JournalEntryTestSuite.cpp
        JournalEntryTestSuite.h
        AddCommand.cpp
        FilterStrategy.cpp
        JournalEntry.cpp
        JournalEntryController.cpp
        JsonJournalEntryRepository.cpp
        RemoveCommand.cpp
        UpdateCommand.cpp
)

target_include_directories(JournalAppTests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/JournalAppTests_autogen/include
        ${Qt6_DIR}/../../include/QtTest
)

target_link_libraries(JournalAppTests
        Qt6::Core
        Qt6::Test
)

add_test(NAME run_JournalAppTests COMMAND JournalAppTests)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()