QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

DESTDIR += $$PWD/bin
TARGET   = cameraControlModule

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}

SOURCES +=  \
    MainWindow.cpp \
    VideoThread.cpp \
    VideoWidget.cpp \
    main.cpp

HEADERS +=  \
    MainWindow.h \
    VideoThread.h \
    VideoWidget.h

INCLUDEPATH += $$PWD/include

# 导入OpenCV库（使用opencv_world统一模块）
LIBS += -L$$PWD/bin -lopencv_world460

RESOURCES += \
    image.qrc

FORMS += \
    MainWindow.ui
