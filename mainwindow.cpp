#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>
#include <QQmlEngine>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    videoWidget = new VideoWidget(this);
    videoQML = new VideoQML(this);
    surface = new QGst::Quick::VideoSurface(this);
    layout = new QHBoxLayout(ui->frame);
    videoQML->setVideoSink(surface->videoSink());
    ui->frame->layout()->addWidget(videoWidget);
    context = ui->quickWidget->engine()->rootContext();
    context->setContextProperty("videoQML", videoQML);
    context->setContextProperty("videoSurface", surface);
    setConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
    QSettings settings;
    QString path = settings.value("last_folder",
                                  QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
    QString fileName = QFileDialog::getOpenFileName(this, "Выберете файл",
                                                    path, "*.avi *.mkv *.mpeg");
    if(fileName != "")
    {
        QFileInfo fi(fileName);
        settings.setValue("last_folder", fi.absolutePath());
        if(!ui->tabWidget->currentIndex())
            videoWidget->setFileName(QString("file://").append(fileName));
        else
            videoQML->setFileName(QString("file://").append(fileName));
    }
}

void MainWindow::on_position_sliderMoved(int position)
{
    unsigned length = -videoWidget->length().msecsTo(QTime(0,0));
    if (length != 0 && position > 0)
    {
        QTime pos(0,0);
        pos = pos.addMSecs(length * (position / 1000.0));
        videoWidget->setPosition(pos);
    }
}

void MainWindow::onPositionChanged()
{
    QTime length(0,0);
    QTime curpos(0,0);
    if (videoWidget->state() != QGst::StateReady &&
        videoWidget->state() != QGst::StateNull)
    {
        length = videoWidget->length();
        curpos = videoWidget->position();
    }
    if (length != QTime(0,0))
    {
        ui->position->setValue(curpos.msecsTo(QTime(0,0)) * 1000 / length.msecsTo(QTime(0,0)));
    }
    else
    {
        ui->position->setValue(0);
    }
}

void MainWindow::onStateChanged()
{
    QGst::State newState = videoWidget->state();
    if(newState == QGst::StateNull)
    {
        onPositionChanged();
    }
}

void MainWindow::setConnections()
{
    connect(ui->action_2, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->start, &QPushButton::clicked, videoWidget, &VideoWidget::play);
    connect(ui->pause, &QPushButton::clicked, videoWidget, &VideoWidget::pause);
    connect(ui->stop, &QPushButton::clicked, videoWidget, &VideoWidget::stop);
    connect(videoWidget, &VideoWidget::positionChanged, this, &MainWindow::onPositionChanged);
    connect(videoWidget, &VideoWidget::stateChanged, this, &MainWindow::onStateChanged);
}
