#-------------------------------------------------
#
# Project created by QtCreator 2018-10-22T10:23:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = serial_client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH +=  ../atlib

SOURCES += main.cpp\
           mainwindow.cpp \
    ../atlib/atDataBuffer.cpp \
    ../atlib/atI2C.cpp \
    ../atlib/atI2C_dev.cpp \
    ../atlib/atI2C_pi.cpp \
    ../atlib/atSocketClient.cpp \
    ../atlib/atKonfiguracja.cpp \
    ../atlib/atFile.cpp \
    ../atlib/atFunctions.cpp

HEADERS  += mainwindow.h \
    ../atlib/atDataBuffer.h \
    ../atlib/at_X3.h \
    ../atlib/at_X3Error.h \
    ../atlib/atI2C.h \
    ../atlib/atI2C_dev.h \
    ../atlib/atI2C_pi.h \
    ../atlib/atSocketClient.h \
    ../atlib/atKonfiguracja.h \
    ../atlib/atFile.h \
    ../atlib/atFunctions.h

FORMS    += mainwindow.ui


