TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    fileserver.cpp \
    fileclient.cpp \
    msgitem.cpp \
    handlestream.cpp \
    client.cpp \
    server.cpp

HEADERS += \
    fileserver.h \
    fileclient.h \
    msgitem.h \
    handlestream.h

DISTFILES += \
    makefile
