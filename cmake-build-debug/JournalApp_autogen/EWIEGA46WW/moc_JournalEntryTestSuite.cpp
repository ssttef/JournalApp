/****************************************************************************
** Meta object code from reading C++ file 'JournalEntryTestSuite.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../JournalEntryTestSuite.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'JournalEntryTestSuite.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN21JournalEntryTestSuiteE_t {};
} // unnamed namespace

template <> constexpr inline auto JournalEntryTestSuite::qt_create_metaobjectdata<qt_meta_tag_ZN21JournalEntryTestSuiteE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "JournalEntryTestSuite",
        "init",
        "",
        "cleanup",
        "testRepository_AddAndGetEntry",
        "testRepository_RemoveEntry",
        "testRepository_UpdateEntry",
        "testRepository_GetAllEntries",
        "testRepository_SaveAndLoadPersistence",
        "testController_AddEntry",
        "testController_RemoveEntry",
        "testController_UpdateEntry",
        "testController_GetAllEntries",
        "testController_UndoRedo_Add",
        "testController_UndoRedo_Remove",
        "testController_UndoRedo_Update",
        "testController_CanUndoRedoState",
        "testController_FilterByDate",
        "testController_FilterByTag",
        "testController_FilterByContent_CaseInsensitive",
        "testController_FilterByContent_CaseSensitive",
        "testController_FilterByAndStrategy",
        "testController_FilterByOrStrategy"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'init'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'cleanup'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'testRepository_AddAndGetEntry'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testRepository_RemoveEntry'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testRepository_UpdateEntry'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testRepository_GetAllEntries'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testRepository_SaveAndLoadPersistence'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_AddEntry'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_RemoveEntry'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_UpdateEntry'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_GetAllEntries'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_UndoRedo_Add'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_UndoRedo_Remove'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_UndoRedo_Update'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_CanUndoRedoState'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_FilterByDate'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_FilterByTag'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_FilterByContent_CaseInsensitive'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_FilterByContent_CaseSensitive'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_FilterByAndStrategy'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'testController_FilterByOrStrategy'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<JournalEntryTestSuite, qt_meta_tag_ZN21JournalEntryTestSuiteE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject JournalEntryTestSuite::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21JournalEntryTestSuiteE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21JournalEntryTestSuiteE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN21JournalEntryTestSuiteE_t>.metaTypes,
    nullptr
} };

void JournalEntryTestSuite::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<JournalEntryTestSuite *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->init(); break;
        case 1: _t->cleanup(); break;
        case 2: _t->testRepository_AddAndGetEntry(); break;
        case 3: _t->testRepository_RemoveEntry(); break;
        case 4: _t->testRepository_UpdateEntry(); break;
        case 5: _t->testRepository_GetAllEntries(); break;
        case 6: _t->testRepository_SaveAndLoadPersistence(); break;
        case 7: _t->testController_AddEntry(); break;
        case 8: _t->testController_RemoveEntry(); break;
        case 9: _t->testController_UpdateEntry(); break;
        case 10: _t->testController_GetAllEntries(); break;
        case 11: _t->testController_UndoRedo_Add(); break;
        case 12: _t->testController_UndoRedo_Remove(); break;
        case 13: _t->testController_UndoRedo_Update(); break;
        case 14: _t->testController_CanUndoRedoState(); break;
        case 15: _t->testController_FilterByDate(); break;
        case 16: _t->testController_FilterByTag(); break;
        case 17: _t->testController_FilterByContent_CaseInsensitive(); break;
        case 18: _t->testController_FilterByContent_CaseSensitive(); break;
        case 19: _t->testController_FilterByAndStrategy(); break;
        case 20: _t->testController_FilterByOrStrategy(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *JournalEntryTestSuite::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *JournalEntryTestSuite::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21JournalEntryTestSuiteE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int JournalEntryTestSuite::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 21;
    }
    return _id;
}
QT_WARNING_POP
