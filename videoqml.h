#ifndef VIDEOQML_H
#define VIDEOQML_H

#include <QGst/Pipeline>
#include <QGst/Ui/VideoWidget>
#include <QGst/ElementFactory>

class VideoQML : public QObject
{
    Q_OBJECT

public:
    explicit VideoQML(QWidget *parent = 0);
    ~VideoQML();

    void setFileName(const QString &fileName);
    void setVideoSink(const QGst::ElementPtr &sink);
    QGst::State state() const;

public Q_SLOTS:
    void play();
    void pause();
    void stop();

Q_SIGNALS:
    void stateChanged();

protected:

private:
    void onBusMessage(const QGst::MessagePtr &message);
    QGst::PipelinePtr mPipeline;
    QGst::ElementPtr mVideoSink;
};

#endif // VIDEOQML_H
