#include "videoqml.h"

#include <gst/app/gstappsink.h>
#include <clutter-gst-3.0/clutter-gst/clutter-gst.h>
#include <QGst/Init>
#include <QGst/Pipeline>
#include <QGst/Utils/ApplicationSink>
#include <QQmlApplicationEngine>
#include <QGst/Message>
#include <QGst/Query>
#include <QGst/Event>
#include <QGst/Bus>
#include <QGlib/Connect>

VideoQML::VideoQML(QWidget *parent):
    QObject(parent)
{
    if(mPipeline)
    {
        mPipeline->setState(QGst::StateNull);
    }
}

VideoQML::~VideoQML()
{

}

void VideoQML::setFileName(const QString &fileName)
{
    if (!mPipeline)
    {
        mPipeline = QGst::ElementFactory::make("playbin").dynamicCast<QGst::Pipeline>();
        if (mPipeline)
        {
            QGst::ElementPtr src = QGst::ElementFactory::make("videotestsrc");
            QGst::ElementPtr appsink = QGst::ElementFactory::make("appsink", "appsink");//приемник
            mPipeline->setProperty("video-sink", mVideoSink);
            mPipeline->add(src);
            mPipeline->add(appsink);
            src->link(appsink);
            QGst::BusPtr bus = mPipeline->bus();
            bus->addSignalWatch();
            QGlib::connect(bus, "message", this, &VideoQML::onBusMessage);
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

void VideoQML::setVideoSink(const QGst::ElementPtr &sink)
{
    mVideoSink = sink;
}

QGst::State VideoQML::state() const
{
    return mPipeline ? mPipeline->currentState() : QGst::StateNull;
}

void VideoQML::play()
{
    if(mPipeline)
    {
        mPipeline->setState(QGst::StatePlaying);
    }
}

void VideoQML::pause()
{
    if (mPipeline)
    {
        mPipeline->setState(QGst::StatePaused);
    }
}

void VideoQML::stop()
{
    if (mPipeline)
    {
        mPipeline->setState(QGst::StateNull);
        Q_EMIT stateChanged();
    }
}

void VideoQML::onBusMessage(const QGst::MessagePtr &message)
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
    default:
        break;
    }
}
