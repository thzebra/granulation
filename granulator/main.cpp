#include "GranulatorInterface.hpp"
#include <QApplication>
#include <rtaudio/RtAudio.h>
#include "RandomSourceData.hpp"
#include "RandomWindowSource.hpp"
#include "RegularSequenceStrategy.hpp"
#include "Scheduler.hpp"
#include "SinusoidalEnvelope.hpp"
#include <QDebug>
#include <strstream>
#include <string>
#include <QMouseEvent>

using namespace Granulation;

QString typeToQString(const RtAudioError::Type t);
void setupStream(int dev, Synthesis::Granulator* g, RtAudio* api, RtAudioCallback callback);
int output(void *outputBuffer, void *inputBuffer, unsigned int nFrames,
            double streamTime, RtAudioStreamStatus status, void *userData);
QString format2QString(const RtAudioFormat& f);

int main(int argc, char *argv[])
{
    RtAudio* api = new RtAudio ();

    int sampleRate = 44100;
    int grainDuration = 30; // duration in ms
    int sourceLength = grainDuration * sampleRate / 1000; // number of samples in the source
    int density = 100; // grains per second

    Synthesis::RegularSequenceStrategy* strat = new Synthesis::RegularSequenceStrategy(density, grainDuration);
    Synthesis::RandomSourceData* data = new Synthesis::RandomSourceData(100000);
    Synthesis::RandomWindowSource* src = new Synthesis::RandomWindowSource(sourceLength, data);
    Synthesis::SinusoidalEnvelope* env = new Synthesis::SinusoidalEnvelope(sourceLength);
    Synthesis::Granulator* g = new Synthesis::Granulator ();

    g->setEssence(env, src);
    g->setStrategy(strat);

    int callback (void *outputBuffer, void *inputBuffer,
                                    unsigned int nFrames,
                                    double streamTime,
                                    RtAudioStreamStatus status,
                                    void *userData );

    QApplication a(argc, argv);
    Panel::GranulatorInterface* w = new Panel::GranulatorInterface ();

    unsigned int ndev = api->getDeviceCount();
    for (unsigned int i = 0; i < ndev; ++i) {
        w->m_devices->addItem(QString(api->getDeviceInfo(i).name.c_str()));
    }
    setupStream(0, g, api, output);

    w->granulator = g;

    QWidget::connect(w->m_button, &QPushButton::clicked, w, &Panel::GranulatorInterface::updateLabel);
    QWidget::connect(w->m_devices, static_cast<void (QComboBox::*)(int)> (&QComboBox::currentIndexChanged), [=] (int idx) {setupStream(idx, g, api, output);});
    QWidget::connect(w->m_drawingArea, &Panel::DrawingArea::mouseMoveEvent, w, static_cast<void (Panel::GranulatorInterface::*)(QMouseEvent*)> (&Panel::GranulatorInterface::addPoint));

    w->show();

    return a.exec();
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

void setupStream(int device, Synthesis::Granulator* g, RtAudio* api, RtAudioCallback callback) {
    if (api) {
        if (api->isStreamOpen())
            api->closeStream();

        auto devinfo = api->getDeviceInfo((unsigned int) device);
        qDebug() << "Setting up stream for device" << device << "of name" << devinfo.name.c_str();
        int16_t fmts = devinfo.nativeFormats;
        int16_t format = 32;
        qDebug() << fmts - format;

        for (; fmts - format < 0 && format >= 1; format /= 2) {
            qDebug() << fmts << format << fmts - format;
        }
        qDebug() << "using format" << format2QString(format);

        RtAudioStreamFlags flags = RTAUDIO_MINIMIZE_LATENCY
                | RTAUDIO_HOG_DEVICE
                | RTAUDIO_SCHEDULE_REALTIME;
        RtAudio::StreamOptions options;
        options.flags = flags;

        try {
            RtAudio::StreamParameters par;
            par.deviceId = device;
            par.nChannels = std::max(devinfo.outputChannels, (unsigned int)1);

            api->openStream(&par, nullptr, (unsigned int)format, g->sampleRate(), &g->bufferFrames, callback, (void*)g, &options);
            qDebug() << "Stream was successfully set up for device" << devinfo.name.c_str() << "with" << par.nChannels << "channels, format" << format << "and" << g->bufferFrames << "buffer frames";
            api->startStream();
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
    if (status == RTAUDIO_OUTPUT_UNDERFLOW)
        qDebug() << "output underflow detected";

    Synthesis::Granulator* g = (Synthesis::Granulator*)userData;

    if (g) {
        float* out = (float*) outputBuffer;
        for (unsigned int i = 0; i < nFrames; ++i) {
            for (int j = 0; j < 2; ++j) {
                out[2 * i + j] = g->synthetize();
            }
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
