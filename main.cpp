#include "mainwindow.h"
#include "videoqml.h"
#include <QApplication>
#include <QGst/Init>
#include <QGst/Quick/VideoItem>
#include <QGst/Quick/VideoSurface>
#include <clutter-gst/clutter-gst.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qmlRegisterType<QGst::Quick::VideoItem>("VideoItem", 1, 0, "VideoItem");
    clutter_init(&argc, &argv);
    QGst::init(&argc, &argv);
    MainWindow w;
    w.show();

    return a.exec();
}
