#include "Grain.hpp"
#include <cmath>
#include <QDebug>
#include <sstream>

namespace Granulation {
namespace Synthesis {

//Grain::Grain() {}

Grain::Grain(Envelope& e, Source& s) :
    m_active{false},
    m_readBackwards{false},
    m_index(0),
    m_source{s},
    m_envelope{e},
    m_completed{false}
{}

Grain::Grain(const Grain & grain) :
    m_active{grain.m_active},
    m_readBackwards{grain.m_readBackwards},
    m_index{grain.m_index},
    m_source{grain.m_source},
    m_envelope{grain.m_envelope},
    m_completed{grain.m_completed},
    m_duration{grain.m_duration}
{}

void Grain::operator =(const Grain& grain) {
    m_active = grain.m_active;
    m_readBackwards = grain.m_readBackwards;
    m_completed = grain.m_completed;
    m_duration = grain.m_duration;
    m_sampleRate = grain.m_sampleRate;
    m_index = grain.m_index;
    m_source = grain.m_source;
    m_envelope = grain.m_envelope;
}

bool Grain::completed() const {
    return m_completed;
}

void Grain::activate(int duration) {
    qDebug() << "grain is being activated for duration" << duration;
    if (duration != 0) {
        m_completed = false;
        m_active = true;
        m_index = 0;
        qDebug() << "reset m_index to 0";
        m_duration = std::abs(duration);

        if (duration > 0) {
            m_readBackwards = false;
        }
        else {
            m_readBackwards = true;
            qDebug() << "grain will be read backwards";
        }
        m_sampleRate = 1000 * m_source.size() / m_duration;
        qDebug() << m_source.size() << m_duration;
        qDebug() << "grain has a normal duration of" << (1000 * m_source.size() / m_source.sampleRate()) << "ms"
                 << "so a duration of" << m_duration << "ms will be achieved with a sample rate of" << m_sampleRate;
    }
    qDebug() << "updated grain is" << grainToString().c_str();
}

float Grain::synthetize() {
    if (m_index >= m_duration * m_sampleRate / 1000 || m_index < 0) {
        m_completed = true;
        return 0.f;
    }
    qDebug() << m_index << (m_duration * m_sampleRate / 1000) << m_duration << m_sampleRate;
    int idx = m_index * m_source.sampleRate() / m_sampleRate;
    qDebug() << "synthetizing sample n°" << m_index << "with sample rate" << m_sampleRate << "taking sample n°" << idx;
    if (m_readBackwards)
        idx = m_source.size() - 1 - idx;
    ++m_index;
    return m_source.data(idx) * m_envelope.data(idx);
}

bool Grain::isActive() const {
    return m_active;
}

bool Grain::isReadBackwards() const {
    return m_readBackwards;
}

std::string Grain::grainToString() const {
    std::stringstream ss;
    if (m_active)
        ss << "Active, ";
    else
        ss << "Inactive, ";
    ss << "index = " << m_index;
    ss << ", sample rate: " << m_sampleRate << "Hz";
    ss << ", duration: " << m_duration << "ms";
    ss << ", is read " << (m_readBackwards ? "backwards" : "forward");
    ss << ", has " << (m_completed ? "completed" : "not completed yet");
    std::string str = ss.str();
    int length = str.size();
    char cstr[length + 1];
    str.copy(cstr, length, 0);
    cstr[length] = 0;
    return std::string(cstr);
}

Grain::~Grain() {}

}
}
