#-------------------------------------------------
#
# Project created by QtCreator 2015-04-19T08:38:26
#
#-------------------------------------------------

QT       += core gui opengl
LIBS += -lsetupapi -lopengl32 -LC:\Qwt-6.1.2\lib\ -lqwt -LC:\qwtplot3d\lib\ -lqwtplot3d -LC:\mathgl-2.3.2-mingw.i686\lib\ -llibmgl-qt5 -LC:\GnuWin32\lib\ -llibgsl -LC:\BLAS\32\ -lblas -llapack
RC_ICONS += app_icon.ico restore.ico exit.ico
CONFIG += c++11

INCLUDEPATH += C:\Qwt-6.1.2\src C:\qwtplot3d\src  C:\qwtplot3d\include C:\mathgl-2.3.2-mingw.i686\include C:\GnuWin32\include C:\armadillo-5.100.1\include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LocomotionService
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    myoglwidget.cpp \
    hid.c \
    calibwidget.cpp \
    sensorplot.cpp

HEADERS  += mainwindow.h \
    myoglwidget.h \
    hidapi.h \
    calibwidget.h \
    sensorplot.h

FORMS    += mainwindow.ui
