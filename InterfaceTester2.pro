#-------------------------------------------------
#
# Project created by QtCreator 2018-03-10T18:25:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InterfaceTester2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    frame.cpp \
    frameOut.cpp \
    framesettingswindow.cpp \
    frameIn.cpp \
    usbdevicecontroller.cpp \
    usbsettingswindow.cpp \
    ethr.cpp \
    ethrsettingswindow.cpp

HEADERS += \
        mainwindow.h \
    frame.h \
    frameIn.h \
    frameOut.h \
    framesettingswindow.h \
    usbdevicecontroller.h \
    usbsettingswindow.h \
    ethr.h \
    ethrsettingswindow.h

FORMS += \
     mainwindow.ui \
     framesettingswindow.ui \
    usbsettingswindow.ui \
    ethrsettingswindow.ui

QT += network


unix|win32: LIBS += -L$$PWD/libusb/static/ -llibusb-1.0

INCLUDEPATH += $$PWD/libusb/include
DEPENDPATH += $$PWD/libusb/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/libusb/static/libusb-1.0.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/libusb/static/libusb-1.0.a

