#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T08:39:14
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ApplicationBDD
TEMPLATE = app


SOURCES += main.cpp\
    mainwindowprincipal.cpp \
    dialogconnect.cpp

HEADERS  += \
    mainwindowprincipal.h \
    dialogconnect.h

FORMS    += \
    mainwindowprincipal.ui \
    dialogconnect.ui

TRANSLATIONS += application_BDD_fr_FR.ts\
                application_BDD_en.ts
