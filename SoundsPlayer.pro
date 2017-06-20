#-------------------------------------------------
#
# Project created by QtCreator 2016-03-28T14:00:33
# @author : suibinc
# @create-time : 2016-04-06
# @web-site : www.suibinc.com
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia\
        multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoundsPlayer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    soundsplayer.cpp

HEADERS  += mainwindow.h \
    soundsplayer.h

FORMS    += mainwindow.ui \
    soundsplayer.ui

RESOURCES += \
    icons.qrc
