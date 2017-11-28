QT += core
QT -= gui



TARGET = Server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/local/include/boost
LIBS += -L"/usr/local/lib" \-lboost_system \-lboost_regex \-lboost_filesystem

CONFIG+= c++11 console

SOURCES += main.cpp \
    server_funct.cpp

HEADERS += \
    server_funct.h

