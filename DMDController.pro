# -------------------------------------------------
# Project created by QtCreator 2017-06-30T09:22:14
# -------------------------------------------------
QT += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_DEPRECATED_WARNINGS
TARGET = DMDController
TEMPLATE = app
SOURCES += main.cpp \
    DMDController.cpp \
    dmd_file.cc \
    dmd.cc \
    dmd_udp.cc \
    DMDInterface.cc
HEADERS += DMDController.h \
    dmd_file.h \
    dmd_udp.h \
    dmd.h \
    DMDController.h \
    DMDInterface.h
FORMS += DMDController.ui
