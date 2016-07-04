#include "ADSREnvelope.hpp"
#include "cmath"

namespace Granulation {
namespace Synthesis {

ADSREnvelope::ADSREnvelope() {}
ADSREnvelope::ADSREnvelope(int64_t length) : Envelope(length) {
    fill();
}

void ADSREnvelope::fill() {
    if (size() == 0)
        return;

    double srms = m_sampleRate / 1000.f;
    int natt = m_attack * srms;
    int ndec = m_decay * srms;
    int nrel = m_release * srms;

    if (natt + ndec + nrel > size()) {
        if (natt + nrel > size()) {
            ndec = 0;
            natt = size() / 2;
            nrel = natt;
        }
        else {
            ndec = size() - (natt + nrel);
        }
    }

    for (int i = 0; i < natt; ++i) {
        m_data[i] = i / double(natt);
    }

    float volsustain = 0.65;

    for (int i = 0; i < ndec; ++i) {
        m_data[i + natt] = i * (volsustain - 1.) + 1.;
    }

    for (int i = ndec; i < size() - nrel; ++i) {
        m_data[i] = volsustain;
    }

    int s = size();
    int i0 = s - nrel;

    for (int i = i0; i < s; ++i) {
        m_data[i] = volsustain * ((i0 - i) / (s - i0) + 1);
    }
}

void ADSREnvelope::setADSR(int attms, int decms, int susms, int relms, int samplerate) {
    m_attack = std::max(attms, 0);
    m_decay = std::max(decms, 0);
    m_release = std::max(relms, 0);

    if (m_sampleRate > 0)
        m_sampleRate = samplerate;

    int length = (attms + decms + susms + relms) * (m_sampleRate / 1000.f);

    recompute(length);
}

void ADSREnvelope::recompute(int64_t length) {
    m_data.clear();
    m_data.resize(length);
    fill();
}

}
}
