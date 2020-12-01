TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += serial.cpp \
        main.cpp \
    detect.cpp \
    distance.cpp


INCLUDEPATH += /usr/local/include \
                /usr/local \
               /usr/local/include/opencv \
               /usr/local/include/opencv4 \
               /usr/local/include/opencv2


LIBS    += /usr/local/lib/libopencv_*.so

unix: LTBS += -lphread
unix: LTBS += -lv4l2

HEADERS += \
    detect.h \
    distance.h \
    serial.h

DISTFILES += \
    cameraParams.xml
