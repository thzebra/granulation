#include "GranulatorInterface.hpp"
#include <QApplication>
#include <rtaudio/RtAudio.h>
#include "RandomSourceData.hpp"
#include "RandomWindowSource.hpp"
#include "CustomWindowSource.hpp"
#include "RegularSequenceStrategy.hpp"
#include "Scheduler.hpp"
#include "SinusoidalEnvelope.hpp"
#include <QDebug>
#include <strstream>
#include <string>
#include <QMouseEvent>
#include "FileSourceData.hpp"
#include "Essence.hpp"
#include <chrono>
#include <sndfile.hh>

using namespace Granulation;
using namespace Synthesis;

QString typeToQString(const RtAudioError::Type t);
void setupStream(int dev, Panel::GranulatorInterface* w, RtAudio* api, RtAudioCallback callback, unsigned int* ptr = nullptr);
int output(void *outputBuffer, void *inputBuffer, unsigned int nFrames,
            double streamTime, RtAudioStreamStatus status, void *userData);
QString format2QString(const RtAudioFormat& f);
int outindex = 0;

int main(int argc, char *argv[])
{
    RtAudio* api = new RtAudio (RtAudio::Api::LINUX_ALSA);
    unsigned int bufferframes = 0;

    QApplication application(argc, argv);
    Panel::GranulatorInterface* w = new Panel::GranulatorInterface ();

    unsigned int ndev = api->getDeviceCount();
    for (unsigned int i = 0; i < ndev; ++i) {
        w->m_devices->addItem(QString(api->getDeviceInfo(i).name.c_str()));
    }
    w->granulator = new Granulator<SinusoidalEnvelope, CustomWindowSource> (w->sourceData(), w->m_grainDuration->value());
    RegularSequenceStrategy strat = RegularSequenceStrategy (w->m_density->value(), w->m_grainDuration->value());
    w->granulator->setStrategy(&strat);
    w->granulator->setMaxGrains(w->m_grainCount->value());

    setupStream(0, w, api, output, &bufferframes);

    QWidget::connect(w->m_devices,
                     static_cast<void (QComboBox::*)(int)> (&QComboBox::currentIndexChanged),
                     [=] (int idx) mutable {setupStream(idx, w, api, output, nullptr);});

    QWidget::connect(w->m_drawingArea,
                     &Panel::DrawingArea::mouseMoveEvent,
                     w,
                     static_cast<void (Panel::GranulatorInterface::*)(QMouseEvent*)> (&Panel::GranulatorInterface::addPoint),
                     Qt::QueuedConnection);

    w->show();

    QWidget::connect(&application, &QApplication::aboutToQuit, [=] () { delete api; });

        return application.exec();
}

QString typeToQString(const RtAudioError::Type t) {
    QString qstr;
    switch (t) {
    case (RtAudioError::Type::DEBUG_WARNING):
        qstr = QString("debug warning");
        break;
    case (RtAudioError::Type::DRIVER_ERROR):
        qstr = QString("driver error");
        break;
    case (RtAudioError::Type::INVALID_DEVICE):
        qstr = QString("invalid device");
        break;
    case (RtAudioError::Type::INVALID_PARAMETER):
        qstr = QString("invalid parameter");
        break;
    case (RtAudioError::Type::INVALID_USE):
        qstr = QString("invalid use");
        break;
    case (RtAudioError::Type::MEMORY_ERROR):
        qstr = QString("memory error");
        break;
    case (RtAudioError::Type::NO_DEVICES_FOUND):
        qstr = QString("no devices found");
        break;
    case (RtAudioError::Type::SYSTEM_ERROR):
        qstr = QString("system error");
        break;
    case (RtAudioError::Type::THREAD_ERROR):
        qstr = QString("thread error");
        break;
    case (RtAudioError::Type::WARNING):
        qstr = QString("warning");
        break;
    default:
        qstr = QString("unspecified");
    }
    return qstr;
}

void setupStream(int device, Panel::GranulatorInterface* w, RtAudio* api, RtAudioCallback callback, unsigned int* ptr) {
    if (api) {
        if (api->isStreamOpen())
            api->closeStream();

        auto g = w->granulator;

        auto devinfo = api->getDeviceInfo((unsigned int) device);
        //qDebug() << "Setting up stream for device" << device << "of name" << devinfo.name.c_str();
        RtAudioFormat format = RTAUDIO_FLOAT32;

        RtAudioStreamFlags flags = RTAUDIO_MINIMIZE_LATENCY
                | RTAUDIO_HOG_DEVICE
                | RTAUDIO_SCHEDULE_REALTIME;
        RtAudio::StreamOptions options;
        options.flags = flags;

        try {
            RtAudio::StreamParameters par;
            par.deviceId = device;
            par.nChannels = std::min(devinfo.outputChannels, (unsigned int)g->channels());

            unsigned int* bfptr;
            if (ptr)
                bfptr = ptr;
            else
                bfptr = &(g->bufferFrames);

            api->openStream(&par, nullptr, (unsigned int)format, g->sampleRate(), bfptr, callback, (void*)w, &options);
            api->startStream();
            qDebug() << options.numberOfBuffers;
        }
        catch (RtAudioError re) {
            qDebug() << "Error type" << typeToQString(re.getType()) << ":" << re.getMessage().c_str();
        }
        catch (std::exception e) {
            qDebug() << "Error:" << e.what();
        }
        catch (...) {
            qDebug() << "Unknown error occured";
        }
    }
}

int output(void *outputBuffer, void *inputBuffer, unsigned int nFrames,
           double streamTime, RtAudioStreamStatus status, void *userData) {

    Panel::GranulatorInterface* w = (Panel::GranulatorInterface*) userData;
    auto g = w->granulator;

//    using clck = std::chrono::high_resolution_clock;
    if (g) {
        float* out = (float*) outputBuffer;

        g->updateTime(streamTime);
        unsigned int nchannels = g->channels();
        int imax = nchannels * nFrames;

        for (int i = 0; i < imax; ++i)
            out[i] = 0;

        if (w->m_mute->isChecked()) {
            return 0;
        }

        g->synthetize(gsl::span<float>{out, imax});

        w->m_graindisplayview->grainDisplay().setGrains(g->grains());

        if (w->m_capturebutton->isChecked()) {
            w->write(out, nFrames);
        }

        if (w->m_generateGrains->isChecked() && g->grainCount() < g->maxGrains()) {
            g->generate(1);
        }
    }
    return 0;
}

QString format2QString(const RtAudioFormat& f) {
    switch (f) {
    case RTAUDIO_SINT8:
        return QString("8-bit signed integer");
    case RTAUDIO_SINT16:
        return QString("16-bit signed integer");
    case RTAUDIO_SINT24:
        return QString("24-bit signed integer");
    case RTAUDIO_SINT32:
        return QString("32-bit signed integer");
    case RTAUDIO_FLOAT32:
        return QString("Normalized between plus/minus 1.0 (32 bits)");
    case RTAUDIO_FLOAT64:
        return QString("Normalized between plus/minus 1.0 (64 bits)");
    default:
        return QString("Unknown format");
    }
}
