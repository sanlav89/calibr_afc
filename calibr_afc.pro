#-------------------------------------------------
#
# Project created by QtCreator 2019-10-24T11:29:09
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calibr_afc
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    panelconnect.cpp \
    calibrator2.cpp \
    plotcalibr.cpp

HEADERS  += mainwidget.h \
    panelconnect.h \
    disstypedef.h \
    calibrator2.h \
    plotcalibr.h \
    helpers.h

INCLUDEPATH += C:/qwt-6.1.3/include
LIBS += -LC:/qwt-6.1.3/lib -lqwt
