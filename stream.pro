QT       += core gui qml quickwidgets quick
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stream
TEMPLATE = app

DEFINES += QT_NO_KEYWORDS

CONFIG += link_pkgconfig

PKGCONFIG += gstreamer-1.0
PKGCONFIG += clutter-1.0
PKGCONFIG += clutter-gst-3.0
PKGCONFIG += Qt5GStreamer-1.0
PKGCONFIG += Qt5GLib-2.0
PKGCONFIG += Qt5GStreamerUi-1.0

LIBS += -L/usr/lib -lQt5GStreamerQuick-1.0
INCLUDEPATH += /usr/include/Qt5GStreamer

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    videowidget.cpp \
    videoqml.cpp

HEADERS  += mainwindow.h \
    videowidget.h \
    videoqml.h

FORMS    += mainwindow.ui

DISTFILES += \
    QmlPlayer.qml
