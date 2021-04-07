/****************************************************************************
** Meta object code from reading C++ file 'core.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../myBrushFirmTest02/core/core.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'core.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Core_t {
    QByteArrayData data[15];
    char stringdata0[154];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Core_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Core_t qt_meta_stringdata_Core = {
    {
QT_MOC_LITERAL(0, 0, 4), // "Core"
QT_MOC_LITERAL(1, 5, 18), // "onDownloadProgress"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 9), // "is_finish"
QT_MOC_LITERAL(4, 35, 12), // "is_successed"
QT_MOC_LITERAL(5, 48, 7), // "percent"
QT_MOC_LITERAL(6, 56, 4), // "note"
QT_MOC_LITERAL(7, 61, 15), // "OnProtocolError"
QT_MOC_LITERAL(8, 77, 10), // "error_type"
QT_MOC_LITERAL(9, 88, 12), // "error_string"
QT_MOC_LITERAL(10, 101, 17), // "OnDispatchPackage"
QT_MOC_LITERAL(11, 119, 7), // "quint8*"
QT_MOC_LITERAL(12, 127, 3), // "pkg"
QT_MOC_LITERAL(13, 131, 3), // "len"
QT_MOC_LITERAL(14, 135, 18) // "OnCommInitCallback"

    },
    "Core\0onDownloadProgress\0\0is_finish\0"
    "is_successed\0percent\0note\0OnProtocolError\0"
    "error_type\0error_string\0OnDispatchPackage\0"
    "quint8*\0pkg\0len\0OnCommInitCallback"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Core[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   43,    2, 0x08 /* Private */,
      10,    2,   48,    2, 0x08 /* Private */,
      14,    0,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Bool, QMetaType::Int, QMetaType::QString,    3,    4,    5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    8,    9,
    QMetaType::Void, 0x80000000 | 11, QMetaType::Int,   12,   13,
    QMetaType::Void,

       0        // eod
};

void Core::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Core *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onDownloadProgress((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->OnProtocolError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->OnDispatchPackage((*reinterpret_cast< quint8*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->OnCommInitCallback(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Core::*)(bool , bool , int , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Core::onDownloadProgress)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Core::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_Core.data,
    qt_meta_data_Core,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Core::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Core::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Core.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Core::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Core::onDownloadProgress(bool _t1, bool _t2, int _t3, QString _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
