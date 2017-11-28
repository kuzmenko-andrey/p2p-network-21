#-------------------------------------------------
#
# Project created by QtCreator 2017-11-27T16:24:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

INCLUDEPATH += /usr/local/include/boost
LIBS += -L"/usr/local/lib" \-lboost_system \-lboost_regex \-lboost_filesystem

CONFIG+= c++11 console
QMAKE_CXXFLAGS += -std=gnu++11


SOURCES += main.cpp\
        mainwindow.cpp \
    client_func.cpp \
    filesyst_func.cpp

HEADERS  += mainwindow.h \
    client_func.h \
    filesyst_func.h

FORMS    += mainwindow.ui
