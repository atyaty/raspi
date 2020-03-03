TARGET=../install/serial_client_win
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cpp \
    ../atlib/at_File.cpp \
    ../atlib/at_Konfiguracja.cpp \
    ../atlib/atDataBuffer.cpp \
    ../atlib/atError.cpp \
    ../atlib/atFileLog.cpp \
    ../atlib/atFunctions.cpp \
    serial_client_win.cpp

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += gtkmm-3.0



HEADERS += \
    ../atlib/at_File.h \
    ../atlib/at_Konfiguracja.h \
    ../atlib/atDataBuffer.h \
    ../atlib/atError.h \
    ../atlib/atErrors.h \
    ../atlib/atFileLog.h \
    ../atlib/atFunctions.h \
    serial_client_win.h
