QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET =  ./FFmpeg_rtsp

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    xdecode.cpp \
    xvideothread.cpp \
    xvideowidget.cpp

HEADERS += \
    mainwindow.h \
    xdecode.h \
    xvideothread.h \
    xvideowidget.h

FORMS += \
    mainwindow.ui
DESTDIR +=$$PWD/bin
INCLUDEPATH +=$$PWD/include
LIBS += $$PWD/lib/avcodec.lib \
        $$PWD/lib/avdevice.lib \
        $$PWD/lib/avfilter.lib \
        $$PWD/lib/avformat.lib \
        $$PWD/lib/avutil.lib \
        $$PWD/lib/postproc.lib \
        $$PWD/lib/swresample.lib \
        $$PWD/lib/swscale.lib

RESOURCES += \
    Res.qrc
