#-------------------------------------------------
#
# Project created by QtCreator 2013-05-02T17:53:46
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtQ1
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    pakfile.cpp \
    bspfile.cpp \
    glwidget.cpp \
    basictypes.cpp \
    matrix.cpp \
    gametime.cpp

HEADERS  += mainwindow.h \
    pakfile.h \
    bspfile.h \
    glwidget.h \
    basictypes.h \
    matrix.h \
    gametime.h

FORMS    += mainwindow.ui
