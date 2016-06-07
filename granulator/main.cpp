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

using namespace Granulation;

QString typeToQString(const RtAudioError::Type t);
void setupStream(int);
RtAudio* api;

int main(int argc, char *argv[])
{
    api = new RtAudio ();

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

    QApplication a(argc, argv);
    Panel::GranulatorInterface w;

    unsigned int ndev = api->getDeviceCount();
    for (unsigned int i = 0; i < ndev; ++i) {
        w.m_devices->addItem(QString(api->getDeviceInfo(i).name.c_str()));
    }
    setupStream(0);

    w.granulator = g;

    QWidget::connect(w.m_button, &QPushButton::clicked, &w, &Panel::GranulatorInterface::updateLabel);
    QWidget::connect(w.m_devices, static_cast<void (QComboBox::*)(int)> (&QComboBox::currentIndexChanged), setupStream);

    w.show();

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

void setupStream(int device) {
    if (api) {
        qDebug() << "Setting up stream for device" << device << "of name" << api->getDeviceInfo((unsigned int) device).name.c_str();
    }
}
