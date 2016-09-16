QT += core
QT -= gui

CONFIG += c++11

TARGET = matilda-helper
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    consoleclass.cpp

HEADERS += \
    consoleclass.h
