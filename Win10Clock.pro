#-------------------------------------------------
#
# Project created by QtCreator 2016-06-10T19:23:29
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Win10Clock
TEMPLATE = app
CONFIG += static

SOURCES += src/main.cpp \
           src/mainWindow.cpp \
           src/hotkey.cpp

HEADERS += src/mainWindow.h \
           src/hotkey.h \
           src/dwm.h

FORMS += ui/mainWindow.ui
