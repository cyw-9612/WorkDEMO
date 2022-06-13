QT -= gui
QT += serialport
QT += core
QT += network

TEMPLATE = lib
DEFINES += LIBCOMMUNICATE_LIBRARY

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
    channel/comchannel.cpp \
    channel/tcpchannel.cpp \
    filter/fixedheadandendreceivefliter.cpp \
    filter/fixedlengthreceivefliter.cpp \
    channeloption/channeloption.cpp \
    channeloption/comchanneloption.cpp \
    channeloption/netchanneloption.cpp \
    channeloption/usbchanneloption.cpp \
    filter/fixheadandlengthreceivefilter.cpp \
    protocal/awa5912protocal.cpp \
    protocal/awa6292protocal.cpp \
    protocal/baseprotocal.cpp \
    rundevice.cpp
    libcommunicate.cpp \

HEADERS += \
    LibCommunicate_global.h \
    channel/comchannel.h \
    channel/ichannel.h \
    channel/tcpchannel.h \
    channeloption/channeloption.h \
    channeloption/comchanneloption.h \
    channeloption/netchanneloption.h \
    channeloption/usbchanneloption.h \
    channeloption/channeloption.h \
    communicate.h \
    filter/fixedheadandendreceivefliter.h \
    filter/fixedlengthreceivefliter.h \
    filter/fixheadandlengthreceivefilter.h \
    protocal/awa5912protocal.h \
    protocal/awa6292protocal.h \
    protocal/baseprotocal.h \
    protocal/iprotocal.h \
    filter/ireceivefilter.h \
    idevice.h \
    protocal/packParam.h \
    rundevice.h

INCLUDEPATH += $$PWD/../LibBase

LIBS += -L$$PWD/../3rdParty/lib -llog4qt
LIBS += -L$$PWD/../libs -llibbase

#指定生成目录
DESTDIR += $$PWD/../libs
DLLDESTDIR += $$PWD/../bin/

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
