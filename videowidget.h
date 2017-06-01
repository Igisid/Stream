#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QTimer>

#include <QGst/Pipeline>
#include <QGst/Ui/VideoWidget>

#include <clutter-gst/clutter-gst.h>

class VideoWidget : public QGst::Ui::VideoWidget
{
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

    void setFileName(const QString &fileName);
    QTime position() const;
    void setPosition(const QTime &pos);
    QTime length() const;
    QGst::State state() const;

public Q_SLOTS:
    void play();
    void pause();
    void stop();
    void onSizeChange();

Q_SIGNALS:
    void positionChanged();
    void stateChanged();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    void onBusMessage(const QGst::MessagePtr &message);
    void handlePipelineStateChange(const QGst::StateChangedMessagePtr &scm);
    QGst::PipelinePtr mPipeline;
    QTimer mPositionTimer;
    ClutterActor *stage;
    ClutterActor *texture;
};

#endif // VIDEOWIDGET_H
