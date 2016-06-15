#include "Grain.hpp"
#include <cmath>
#include <QDebug>
#include <sstream>

namespace Granulation {
namespace Synthesis {

//Grain::Grain() {}

Grain::Grain(std::shared_ptr<Envelope> e, std::shared_ptr<Source> s) :
    m_source{s},
    m_envelope{e}
{

}

Grain::Grain(const Grain & grain) :
    m_active{grain.m_active},
    m_readBackwards{grain.m_readBackwards},
    m_index{grain.m_index},
    m_source{grain.m_source},
    m_envelope{grain.m_envelope},
    m_completed{grain.m_completed}
    //m_duration{grain.m_duration}
{

}

void Grain::operator =(const Grain& grain) {
    m_active = grain.m_active;
    m_readBackwards = grain.m_readBackwards;
    m_completed = grain.m_completed;
    //m_duration = grain.m_duration;
    m_index = grain.m_index;
    m_source = grain.m_source;
    m_envelope = grain.m_envelope;
}

bool Grain::completed() const {
    return m_completed;
}

void Grain::activate(int duration) {
    //qDebug() << "grain is being activated for duration" << duration;
    if (duration != 0) {
        m_completed = false;
        m_active = true;
        m_index = 0;
        m_channelindex = 0;
        //m_duration = std::abs(duration);

        if (duration > 0) {
            m_readBackwards = false;
        }
        else {
            m_readBackwards = true;
            //qDebug() << "grain will be read backwards";
        }
    }
}

float Grain::synthetize() {
    int nc = m_source->channels();
    int srcs = m_source->size();
    if (m_index * nc + m_channelindex >= srcs || m_index < 0) {
        m_index = 0;
        m_channelindex = 0;
    }

    int idx = m_index * nc + m_channelindex;
    if (m_readBackwards)
        idx = srcs - 1 - idx;
    m_channelindex = (m_channelindex + 1) % nc;
    if (m_channelindex == 0)
        ++m_index;
    //qDebug() << idx << m_index;
    return m_source->data(idx) * m_envelope->data(m_index);
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
    //ss << ", duration: " << m_duration << "ms";
    ss << ", is read " << (m_readBackwards ? "backwards" : "forward");
    ss << ", has " << (m_completed ? "completed" : "not completed yet");
    std::string str = ss.str();
    int length = str.size();
    char cstr[length + 1];
    str.copy(cstr, length, 0);
    cstr[length] = 0;
    return std::string(cstr);
}

}
}
