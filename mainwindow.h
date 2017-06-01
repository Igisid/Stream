#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "videowidget.h"
#include "videoqml.h"

#include <QMainWindow>
#include <QHBoxLayout>
#include <QQmlContext>
#include <QGst/Quick/VideoSurface>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    void on_action_triggered();
    void on_position_sliderMoved(int position);
    void onPositionChanged();
    void onStateChanged();

private:
    Ui::MainWindow *ui;
    QHBoxLayout *layout;
    VideoWidget *videoWidget;
    VideoQML *videoQML;
    QQmlContext *context;
    QGst::Quick::VideoSurface *surface;
    void setConnections();
};

#endif // MAINWINDOW_H
