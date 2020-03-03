TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    ../atlib/atDataBuffer.cpp \
    ../atlib/atFunctions.cpp \
    ../atlib/at_File.cpp \
    ../atlib/at_Konfiguracja.cpp \
    ../atlib/atStoper.cpp \
    ../atlib/atFileLog.cpp \
    ../atlib/atSocketServer.cpp

HEADERS += \
    ../atlib/atDataBuffer.h \
    ../atlib/at_X3.h \
    ../atlib/at_X3Error.h \
     ../atlib/atFunctions.h \
    ../atlib/at_File.h \
    ../atlib/atErrors.h \
    ../atlib/at_Konfiguracja.h \
    ../atlib/atStoper.h \
    ../atlib/atFileLog.h \
    ../atlib/atSocketServer.h

INCLUDEPATH +=  ../atlib


