#-------------------------------------------------
#
# Project created by QtCreator 2013-05-14T22:37:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = whatFU
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    detect.h

FORMS    += mainwindow.ui


INCLUDEPATH += /usr/include/opencv
INCLUDEPATH += /usr/include/opencv2

LIBS +=/usr/lib64/libopencv_calib3d.so \
/usr/lib64/libopencv_contrib.so \
/usr/lib64/libopencv_core.so \
/usr/lib64/libopencv_features2d.so \
/usr/lib64/libopencv_flann.so \
/usr/lib64/libopencv_highgui.so \
/usr/lib64/libopencv_imgproc.so \
/usr/lib64/libopencv_legacy.so \
/usr/lib64/libopencv_ml.so \
/usr/lib64/libopencv_objdetect.so \
/usr/lib64/libopencv_photo.so \
/usr/lib64/libopencv_stitching.so \
/usr/lib64/libopencv_superres.so \
/usr/lib64/libopencv_ts.so \
/usr/lib64/libopencv_video.so \
/usr/lib64/libopencv_videostab.so
