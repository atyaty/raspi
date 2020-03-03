#-------------------------------------------------
#
# Project created by QtCreator 2019-05-08T12:19:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ADS1256_widgets
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


SOURCES += main.cpp\
        mainwindow.cpp \
    ../atlib/atSPI_ads1256.cpp \
    ../atlib/atSPI_dev.cpp \
    ../atlib/atDataBuffer.cpp \
    ../atlib/atFunctions.cpp \
    ../atlib/atFile.cpp \
    ../atlib/atSPI.cpp \
    ../atlib/atKanal.cpp

HEADERS  += mainwindow.h \
    ../atlib/atSPI_ads1256.h \
    ../atlib/atSPI_dev.h \
    ../atlib/atDataBuffer.h \
    ../atlib/atFunctions.h \
    ../atlib/atFile.h \
    ../atlib/atErrors.h \
    ../atlib/atSPI.h \
    ../atlib/atKanal.h

FORMS    += mainwindow.ui
INCLUDEPATH +=  ../atlib
LIBS += -L/usr/local/lib -lbcm2835
