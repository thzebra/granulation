#include "Grain.hpp"
#include <math.h>
#include <QDebug>

namespace Granulation {
namespace Synthesis {

Grain::Grain() :
    m_active{false},
    m_readBackwards{false},
    m_index(0)
{}

Grain::Grain(Envelope *e, Source *s) :
    m_source{s},
    m_envelope{e}
{}

bool Grain::completed() const {
    return isActive() &&  (m_index >= m_source->size() / m_speedRatio);
}

void Grain::activate(int duration) {
    qDebug() << "grain is being activated for duration" << duration;
    if (duration != 0) {
        m_active = true;
        int d = std::abs(duration);
        if (duration > 0) {
            m_readBackwards = false;
            m_index = 0;
        }
        else {
            m_index = m_source->size() - 1;
            m_readBackwards = true;
            qDebug() << "grain will be read backwards";
        }
        if (m_source->size() > 0) {
            float samplerate = (1000 * m_source->size()) / d;
            m_speedRatio = samplerate / m_source->sampleRate();
        }
    }
}

float Grain::synthetize() {
    qDebug() << "grain is synthetizing";
    float res = 0.f;
    if (m_speedRatio > 1) {
        int nSamples = (int)(m_speedRatio + 0.5);
        if (m_readBackwards) {
            float synthSample = 0.f;
            int imin = std::max(0, m_index - nSamples);
            for (int i = m_index; i > imin; --i) {
                synthSample += m_envelope->data(i) * m_source->data(i);
            }
            --m_index;
            res = synthSample / nSamples;
        }
        else {
            float synthSample = 0.f;
            int imax = std::min(nSamples, (int)m_source->size());
            for (int i = m_index; i < imax; ++i) {
                synthSample += m_envelope->data(i) * m_source->data(i);
            }
            ++m_index;
            res = synthSample / imax;
        }
    }
    else {
        int i;
        if (!m_readBackwards) {
            i = m_speedRatio * m_index;
            ++m_index;
        }
        else {
            int i = m_source->size() - m_speedRatio * m_index;
            --m_index;
        }
        res = m_source->data(i) * m_envelope->data(i);
    }
    qDebug() << "result is" << res;
    return res;
}

bool Grain::isActive() const {
    return m_active;
}

}
}
