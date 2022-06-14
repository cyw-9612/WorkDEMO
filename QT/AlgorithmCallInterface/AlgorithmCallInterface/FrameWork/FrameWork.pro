QT       += core gui
QT += sql
QT += serialport
QT += network
QT += concurrent
QT += xlsx
QT += widgets
QT += charts multimedia
CONFIG+=resources_big

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    main.cpp \
    winmain.cpp

HEADERS += \
    winmain.h

FORMS += \
    winmain.ui

INCLUDEPATH += $$PWD/../3rdParty/include/log4qt
INCLUDEPATH += $$PWD/../3rdParty/include/libWavFileProcess
INCLUDEPATH += $$PWD/../3rdParty/include/
INCLUDEPATH += $$PWD/../LibBase
INCLUDEPATH += $$PWD/../LibControl
INCLUDEPATH += $$PWD/../LibCommunicate

LIBS += -L$$PWD/../libs -llibbase
LIBS += -L$$PWD/../3rdParty/lib/ -llog4qt
LIBS += -L$$PWD/../libs -llibControl
LIBS += -L$$PWD/../libs -llibCommunicate

# PRE_TARGETDEPS += $$PWD/../3rdParty/lib/liblog4qt.a
DESTDIR += $$PWD/../bin/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

PRE_TARGETDEPS += $$PWD/../libs/liblibBase.a
PRE_TARGETDEPS += $$PWD/../libs/liblibControl.a

RESOURCES += \
    ../images.qrc

OTHER_FILES += app.rc
RC_FILE += app.rc
