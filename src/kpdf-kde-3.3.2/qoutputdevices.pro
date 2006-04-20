TEMPLATE = lib

TARGET = qoutputdevices

#debug information
QMAKE_CXXFLAGS += -g

#include headers from kernel and used by kernel
INCLUDEPATH += ../xpdf/ ../xpdf/xpdf ../xpdf/goo ../xpdf/splash

#QOutputDevices source files
HEADERS += QOutputDevPixmap.h
SOURCES += QOutputDevPixmap.cpp
HEADERS += QOutputDev.h
SOURCES += QOutputDev.cpp

OBJECTS += ../xpdf/xpdf/SplashOutputDev.o
