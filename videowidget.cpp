#include "videowidget.h"

#include <QDebug>

#include <QGst/ElementFactory>
#include <QGst/Message>
#include <QGst/Query>
#include <QGst/Event>
#include <QGst/Bus>
#include <QGlib/Connect>

VideoWidget::VideoWidget(QWidget *parent) :
    QGst::Ui::VideoWidget(parent)
{
    if(mPipeline)
    {
        mPipeline->setState(QGst::StateNull);
        stopPipelineWatch();
    }
    connect(&mPositionTimer, &QTimer::timeout, this, &VideoWidget::positionChanged);
}

VideoWidget::~VideoWidget()
{

}

void VideoWidget::setFileName(const QString &fileName)
{
    if (!mPipeline)
    {
        mPipeline = QGst::ElementFactory::make("playbin").dynamicCast<QGst::Pipeline>();
        if (mPipeline)
        {
            QGst::ElementPtr appsink = QGst::ElementFactory::make("appsink", "appsink");

            stage = CLUTTER_ACTOR(g_object_new(CLUTTER_TYPE_STAGE, NULL));

            texture = CLUTTER_ACTOR(g_object_new(CLUTTER_TYPE_TEXTURE, NULL));

            GstElement *sink = gst_element_factory_make("clutterautovideosink", NULL);

            g_object_set(sink, "texture", texture, NULL);

            QGst::ElementPtr clutterSink = QGst::ElementPtr::wrap(sink);

            mPipeline->add(clutterSink);
            mPipeline->add(appsink);
            clutterSink->link(appsink);
            watchPipeline(mPipeline);
            QGst::BusPtr bus = mPipeline->bus();
            bus->addSignalWatch();
            QGlib::connect(bus, "message", this, &VideoWidget::onBusMessage);
        }
        else
        {
            qCritical() << "Failed to create the pipeline";
        }
    }
    if (mPipeline)
    {
        mPipeline->setProperty("uri", fileName);
    }
    play();
}

QTime VideoWidget::position() const
{
    if(mPipeline)
    {

        QGst::PositionQueryPtr query = QGst::PositionQuery::create(QGst::FormatTime);
        mPipeline->query(query);
        return QGst::ClockTime(query->position()).toTime();
    }
    else
    {
        return QTime(0,0);
    }
}

void VideoWidget::setPosition(const QTime &pos)
{
    QGst::SeekEventPtr evt = QGst::SeekEvent::create(1.0, QGst::FormatTime, QGst::SeekFlagFlush,
                                                     QGst::SeekTypeSet, QGst::ClockTime::fromTime(pos),
                                                     QGst::SeekTypeNone, QGst::ClockTime::None
                                                     );
    mPipeline->sendEvent(evt);
}

QTime VideoWidget::length() const
{
    if (mPipeline)
    {
        QGst::DurationQueryPtr query = QGst::DurationQuery::create(QGst::FormatTime);
        mPipeline->query(query);
        return QGst::ClockTime(query->duration()).toTime();
    }
    else
    {
        return QTime(0,0);
    }
}

QGst::State VideoWidget::state() const
{
    return mPipeline ? mPipeline->currentState() : QGst::StateNull;
}

void VideoWidget::play()
{
    if(mPipeline)
    {
        mPipeline->setState(QGst::StatePlaying);
        clutter_group_add(CLUTTER_GROUP(stage), texture);
        clutter_actor_show(stage);
        clutter_main();
    }
}

void VideoWidget::pause()
{
    if (mPipeline)
    {
        mPipeline->setState(QGst::StatePaused);
    }
}

void VideoWidget::stop()
{
    if (mPipeline)
    {
        mPipeline->setState(QGst::StateNull);
        Q_EMIT stateChanged();
    }
}

void VideoWidget::onSizeChange()
{

}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if(!this->isFullScreen())
    {
        this->setWindowFlags(this->windowFlags() | Qt::WindowType_Mask | Qt::WindowCloseButtonHint);
        this->showFullScreen();
    }
    else
    {
        this->setWindowFlags(this->windowFlags() & ~Qt::WindowType_Mask & ~Qt::WindowCloseButtonHint);
        this->showNormal();
    }
}

void VideoWidget::onBusMessage(const QGst::MessagePtr &message)
{
    switch (message->type())
    {
    case QGst::MessageEos:
        stop();
        break;
    case QGst::MessageError:
        qCritical() << message.staticCast<QGst::ErrorMessage>()->error();
        stop();
        break;
    case QGst::MessageStateChanged:
        if (message->source() == mPipeline)
            handlePipelineStateChange(message.staticCast<QGst::StateChangedMessage>());
        break;
    default:
        break;
    }
}

void VideoWidget::handlePipelineStateChange(const QGst::StateChangedMessagePtr &scm)
{
    switch (scm->newState())
    {
    case QGst::StatePlaying:
        mPositionTimer.start(100);
        break;
    case QGst::StatePaused:
        if(scm->oldState() == QGst::StatePlaying) {
            mPositionTimer.stop();
        }
        break;
    default:
        break;
    }
    Q_EMIT stateChanged();
}
