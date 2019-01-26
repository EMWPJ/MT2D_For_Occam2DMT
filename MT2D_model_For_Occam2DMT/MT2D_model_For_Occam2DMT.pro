#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T12:23:45
# A small software for Occam 2D MT
# Copyright belongs to Wang Peijie
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MT2D_model_For_Occam2DMT
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    paint.cpp \
    functions.cpp \
    frequency.cpp \
    intercal.cpp \
    colorscale.cpp \
    unit.cpp \
    expand.cpp \
    newmodel.cpp \
    site.cpp \
    projectmanager.cpp \
    occamproject.cpp \
    occam2dmtmodel.cpp

HEADERS  += \
    paint.h \
    functions.h \
    intercal.h \
    frequency.h \
    colorscale.h \
    unit.h \
    expand.h \
    newmodel.h \
    site.h \
    projectmanager.h \
    occamproject.h \
    occam2dmtmodel.h

FORMS    += \
    frequency.ui \
    colorscale.ui \
    paint.ui \
    expand.ui \
    newmodel.ui \
    site.ui \
    projectmanager.ui \
    occamproject.ui \
    occam2dmtmodel.ui
