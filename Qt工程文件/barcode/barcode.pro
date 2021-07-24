#-------------------------------------------------
#
# Project created by QtCreator 2013-05-14T22:37:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = barcode_recog
TEMPLATE = app
CONFIG += c++11  # OpenCV 4 requires this

SOURCES += main.cpp \
        mainwindow.cpp \
        barcode_recog.cpp

HEADERS  += mainwindow.h \
    barcode_recog.h

FORMS    += mainwindow.ui

# opencv including directory
OPENCV_INCLUDE_DIR = /usr/include/opencv4

# opencv libraries
OPENCV_LIBS = /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so \
/usr/lib/x86_64-linux-gnu/libopencv_imgcodecs.so \
/usr/lib/x86_64-linux-gnu/libopencv_core.so

INCLUDEPATH += $${OPENCV_INCLUDE_DIR}
LIBS += $${OPENCV_LIBS}
