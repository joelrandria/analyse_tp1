#-------------------------------------------------
#
# Project created by QtCreator 2014-09-29T10:13:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tp1
TEMPLATE = app


SOURCES += main.cpp\
    Workspace.cpp \
    Tp1Application.cpp \
    WorkspaceWindow.cpp

HEADERS  += \
    Workspace.h \
    Tp1Application.h \
    WorkspaceWindow.h

FORMS    += \
    WorkspaceWindow.ui

LIBS += `pkg-config opencv --libs`
