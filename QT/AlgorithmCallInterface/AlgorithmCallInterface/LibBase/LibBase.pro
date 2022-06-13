QT -= gui
QT += serialport
QT += core
QT += network

TEMPLATE = lib
DEFINES += LIBBASE_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TCP/clienttcp.cpp \
    binaryutil.cpp \
    logger/rtbaselogger.cpp \
    logger/rtlogger.cpp \
    memory/buffer.cpp \
    serialport/serialport.cpp \
    usb/usbhid.cpp

HEADERS += \
    TCP/clienttcp.h \
    binaryutil.h \
    logger/rtbaselogger.h \
    memory/buffer.h \
    memory/slice.h \
    base_global.h \
    iview.h \
    logger/ilogger.h \
    logger/rtlogger.h \
    serialport/serialport.h \
    singleton.h \
    usb/usbhid.h

INCLUDEPATH += $$PWD/../3rdParty/include
INCLUDEPATH += $$PWD/../3rdParty/include/log4qt
INCLUDEPATH += $$PWD/../3rdParty/include/hidlib

#指定引用库的DLL目录
LIBS += -L$$PWD/../3rdParty/lib -llog4qt
LIBS += -L$$PWD/../3rdParty/lib -lhidapi
#LIBS += -lsetupapi
LIBS += -lws2_32

DESTDIR += $$PWD/../libs #指定生成目录
DLLDESTDIR += $$PWD/../bin/

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
