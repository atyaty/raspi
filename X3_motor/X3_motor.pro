TARGET=../install/x3_motor
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    X3_motor.cpp \
    ../atlib/atI2C_dev.cpp \
    ../atlib/atI2C_X3.cpp \
    ../atlib/atI2C.cpp \
    ../atlib/at_File.cpp \
    ../atlib/at_Konfiguracja.cpp \
    ../atlib/atDataBuffer.cpp \
    ../atlib/atError.cpp \
    ../atlib/atFileLog.cpp \
    ../atlib/atFunctions.cpp

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += gtkmm-3.0



HEADERS += \
    X3_motor.h \
    ../atlib/atI2C_dev.h \
    ../atlib/atI2C_X3.h \
    ../atlib/atI2C.h \
    ../atlib/at_File.h \
    ../atlib/at_Konfiguracja.h \
    ../atlib/atDataBuffer.h \
    ../atlib/atError.h \
    ../atlib/atErrors.h \
    ../atlib/atFileLog.h \
    ../atlib/atFunctions.h


INCLUDEPATH +=  ../atlib


LIBS += -L/usr/local/lib -lbcm2835
