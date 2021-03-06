QT += widgets
QT += core gui
QT += multimedia
QT += sql
QT += charts multimedia

TEMPLATE = lib
DEFINES += LIBCONTROL_LIBRARY

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

CONFIG+=resources_big

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
    SysInfo/cpuwidget.cpp \
    SysInfo/memorywidget.cpp \
    SysInfo/sysinfo.cpp \
#    SysInfo/sysinfolinuximpl.cpp \
    SysInfo/sysinfominiwidgat.cpp \
    SysInfo/sysinfowidget.cpp \
    SysInfo/sysinfowindowsimpl.cpp \
    baseview.cpp \
    childview.cpp \
    libcontrol.cpp \
    myAESTool/myrsa.cpp \
    myAESTool/qaesencryption.cpp \
    myAESTool/usetimemark.cpp \
    myComputerInfoDialog/currentthreaddialog.cpp \
    myComputerInfoDialog/mycomputerinfodialog.cpp \
    myComputerInfoDialog/threadcheck.cpp \
    myDataBase/cdatabase.cpp \
    myDataBase/cfile.cpp \
    myFunctionListwidget/myfunctiondisplaydialog.cpp \
    myFunctionListwidget/myfunctionlistwidget.cpp \
    myFunctionListwidget/taskchoisedialog.cpp \
    myMessageDialog/minimessagedialog.cpp \
    myMessageDialog/mymessagedialog.cpp \
    myProgressBox/myprogressbox.cpp \
    myProgressBox/picprogress.cpp \
    myToolTip/mytooltip.cpp \
    mydoublevalidator.cpp \
    mytool.cpp \
    popupview.cpp \
    rtmasklayer.cpp \
    rtmessagebox.cpp

HEADERS += \
    LibControl_global.h \
    SysInfo/cpuwidget.h \
    SysInfo/memorywidget.h \
    SysInfo/sysinfo.h \
#    SysInfo/sysinfolinuximpl.h \
    SysInfo/sysinfominiwidgat.h \
    SysInfo/sysinfowidget.h \
    SysInfo/sysinfowindowsimpl.h \
    baseview.h \
    childview.h \
    libcontrol.h \
    myAESTool/myrsa.h \
    myAESTool/qaesencryption.h \
    myAESTool/usetimemark.h \
    myComputerInfoDialog/currentthreaddialog.h \
    myComputerInfoDialog/mycomputerinfodialog.h \
    myComputerInfoDialog/threadcheck.h \
    myDataBase/cdatabase.h \
    myDataBase/cfile.h \
    myFunctionListwidget/myfunctiondisplaydialog.h \
    myFunctionListwidget/myfunctionlistwidget.h \
    myFunctionListwidget/taskchoisedialog.h \
    myMessageDialog/minimessagedialog.h \
    myMessageDialog/mymessagedialog.h \
    myProgressBox/myprogressbox.h \
    myProgressBox/picprogress.h \
    myToolTip/mytooltip.h \
    mydoublevalidator.h \
    mytool.h \
    popupview.h \
    rtmasklayer.h \
    rtmessagebox.h

INCLUDEPATH += $$PWD/../LibBase
INCLUDEPATH += $$PWD/../3rdParty/include/

LIBS += -L$$PWD/../3rdParty/lib -llog4qt
LIBS += -L$$PWD/../libs -llibbase
LIBS += -L$$PWD/../3rdParty/lib -lfmodex
LIBS += -L$$PWD/../3rdParty/lib -llibssl -llibcrypto

DESTDIR += $$PWD/../libs #??????????????????
DLLDESTDIR += $$PWD/../bin/

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    SysInfo/sysinfominiwidgat.ui \
    myComputerInfoDialog/currentthreaddialog.ui \
    myComputerInfoDialog/mycomputerinfodialog.ui \
    myFunctionListwidget/myfunctiondisplaydialog.ui \
    myFunctionListwidget/myfunctionlistwidget.ui \
    myFunctionListwidget/taskchoisedialog.ui \
    myMessageDialog/minimessagedialog.ui \
    myMessageDialog/mymessagedialog.ui \
    myProgressBox/myprogressbox.ui \
    myToolTip/mytooltip.ui \

RESOURCES += \
    ../images.qrc

SUBDIRS += \
    musicPlayer/myMusicPlayer.pro
