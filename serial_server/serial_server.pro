TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    atGetch.cpp \
    ../atlib/atSerial.cpp \
    ../atlib/atDataBuffer.cpp



HEADERS += \
    atGetch.h \
    ../atlib/atSerial.h \
    ../atlib/atDataBuffer.h \
    ../atlib/at_X3.h \
    ../atlib/at_X3Error.h

INCLUDEPATH +=  ../atlib
