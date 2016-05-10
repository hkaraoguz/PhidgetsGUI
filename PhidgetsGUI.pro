#-------------------------------------------------
#
# Project created by QtCreator 2016-05-02T14:03:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhidgetsGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    phidgetsmotorcontrol.cpp \
    phidgetsdevicemanager.cpp \
    phidgetsinterfacekit.cpp \
    irsensorgraphicsscene.cpp \
    motorcontroldialog.cpp \
    digitaliodialog.cpp

HEADERS  += mainwindow.h \
    phidgetsmotorcontrol.h \
    phidgetsdevicemanager.h \
    phidgetsinterfacekit.h \
    irsensorgraphicsscene.h \
    motorcontroldialog.h \
    digitaliodialog.h

FORMS    += mainwindow.ui \
    motorcontroldialog.ui \
    digitaliodialog.ui

LIBS += -lphidget21
