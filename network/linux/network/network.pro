#-------------------------------------------------
#
# Project created by QtCreator 2016-11-14T19:54:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = network
TEMPLATE = app


SOURCES += main.cpp \
    ../csocket.cpp \
    netmodel.cpp \
    select.cpp

HEADERS  += \
    ../csocket.h \
    netmodel.h \
    select.h
