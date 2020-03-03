TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    ../atlib/atSocketClient.cpp \
    ../atlib/atDataBuffer.cpp

HEADERS += \
    ../atlib/atDataBuffer.h \
    ../atlib/atSocketClient.h \
    ../atlib/cfg.h \
    ../atlib/at_X3.h \
    ../atlib/at_X3Error.h
INCLUDEPATH +=  ../atlib
