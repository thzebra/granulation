#include "Granulator.hpp"
#include <QDebug>

namespace Granulation {
namespace Synthesis {

Granulator::Granulator() :
    m_scheduler{Scheduler ()}
{}

void Granulator::setEssence(Envelope* env, Source* src) {
    m_envelope = env;
    m_source = src;
}

void Granulator::setStrategy(SequenceStrategy *strategy) {
    m_scheduler.setStrategy(strategy);
}

float Granulator::synthetize() {
    return m_scheduler.synthetize();
}

void Granulator::generate(int n) {
    qDebug() << "granulator is generating" << n << "new grains";
    for (int i = 0; i < n; ++i) {
        m_scheduler.addGrain(m_envelope, m_source);
    }
}

int Granulator::sampleRate() {
    if (m_source)
        return m_source->sampleRate();
    return 44100;
}

void Granulator::updateTime(double streamTime) {
    m_scheduler.updateTime(streamTime);
}

}
}
