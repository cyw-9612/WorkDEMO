/****************************************************************************
** Meta object code from reading C++ file 'myfirstplugin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../PluginWidget/myfirstplugin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'myfirstplugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyFirstPlugin_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyFirstPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyFirstPlugin_t qt_meta_stringdata_MyFirstPlugin = {
    {
QT_MOC_LITERAL(0, 0, 13) // "MyFirstPlugin"

    },
    "MyFirstPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyFirstPlugin[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void MyFirstPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject MyFirstPlugin::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_MyFirstPlugin.data,
    qt_meta_data_MyFirstPlugin,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MyFirstPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyFirstPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyFirstPlugin.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "AbstractInterface"))
        return static_cast< AbstractInterface*>(this);
    if (!strcmp(_clname, "Welcome to use this API"))
        return static_cast< AbstractInterface*>(this);
    return QObject::qt_metacast(_clname);
}

int MyFirstPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}

QT_PLUGIN_METADATA_SECTION
static constexpr unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x77,  'W',  'e',  'l',  'c',  'o',  'm', 
    'e',  ' ',  't',  'o',  ' ',  'u',  's',  'e', 
    ' ',  't',  'h',  'i',  's',  ' ',  'A',  'P', 
    'I', 
    // "className"
    0x03,  0x6d,  'M',  'y',  'F',  'i',  'r',  's', 
    't',  'P',  'l',  'u',  'g',  'i',  'n', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(MyFirstPlugin, MyFirstPlugin)

QT_WARNING_POP
QT_END_MOC_NAMESPACE
