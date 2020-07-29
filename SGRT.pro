#-------------------------------------------------
#
# Project created by QtCreator 2020-05-01T18:48:48
#
#-------------------------------------------------

QT += core gui multimedia concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SGRT
TEMPLATE = app


INCLUDEPATH += /usr/local/include/opencv4
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_video -lopencv_videoio -lopencv_objdetect -lopencv_face -lopencv_calib3d



DEFINES += OPENCV_DATA_DIR=\\\"/opt/opencv/data/haarcascades/\\\"
DEFINES += OPENCV_LBP_DIR=\\\"/opt/opencv/data/lbpcascades/\\\"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp mainwindow.cpp


HEADERS += mainwindow.h


