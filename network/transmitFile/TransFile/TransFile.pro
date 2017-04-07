TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fileserver.cpp \
    fileclient.cpp

HEADERS += \
    fileserver.h \
    fileclient.h
