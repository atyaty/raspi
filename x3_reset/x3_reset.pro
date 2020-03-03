TARGET=../install/x3_reset
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    ../atlib/atFunctions.cpp \
    ../atlib/at_File.cpp \
    ../atlib/at_Konfiguracja.cpp \
    ../atlib/atStoper.cpp \
    ../atlib/atFileLog.cpp \
    ../atlib/atGPIO.cpp \
    ../atlib/atError.cpp

HEADERS += \
    ../atlib/atFunctions.h \
    ../atlib/at_File.h \
    ../atlib/atErrors.h \
    ../atlib/at_Konfiguracja.h \
    ../atlib/atStoper.h \
    ../atlib/atFileLog.h \
    ../atlib/atGPIO.h \
    ../atlib/atError.h

INCLUDEPATH +=  ../atlib

LIBS += -L/usr/local/lib -lbcm2835

