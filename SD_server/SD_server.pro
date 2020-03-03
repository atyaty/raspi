TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    ../atlib/atDataBuffer.cpp \
    ../atlib/atSPI_ads1256.cpp \
    ../atlib/atSPI_dev.cpp \
    ../atlib/atSPI.cpp \
    ../atlib/atFunctions.cpp \
    ../atlib/at_File.cpp \
    ../atlib/atI2C_dev.cpp \
    ../atlib/atI2C_X3.cpp \
    ../atlib/atI2C.cpp \
    ../atlib/atSocketServer.cpp \
    ../atlib/atStoper.cpp \
    ../atlib/atError.cpp \
    ../atlib/at_Konfiguracja.cpp

HEADERS += \
    ../atlib/atDataBuffer.h \
    ../atlib/at_X3.h \
    ../atlib/atSPI_ads1256.h \
    ../atlib/atSPI_dev.h \
    ../atlib/atSPI.h \
    ../atlib/atFunctions.h \
    ../atlib/at_File.h \
    ../atlib/atErrors.h \
    ../atlib/atI2C_dev.h \
    ../atlib/atI2C_X3.h \
    ../atlib/atI2C.h \
    ../atlib/atSocketServer.h \
    ../atlib/at_Konfiguracja.h \
    ../atlib/atStoper.h \
    ../atlib/atError.h

INCLUDEPATH +=  ../atlib

LIBS += -L/usr/local/lib -lbcm2835

