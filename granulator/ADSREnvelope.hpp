#pragma once
#include "Envelope.hpp"

namespace Granulation {
namespace Synthesis {

class ADSREnvelope : public Envelope {
public:
    ADSREnvelope();
    ADSREnvelope(int64_t length);

    // Envelope interface
public:
    void fill() override;
    void recompute(int64_t length) override;
    void setADSR(int attms, int decms, int susms, int relms, int samplerate) override;

    ~ADSREnvelope() = default;

private:
    static int m_attack;
    static int m_decay;
    static int m_release;
    static int m_sampleRate;
};

int ADSREnvelope::m_attack = 50;
int ADSREnvelope::m_decay = 0;
int ADSREnvelope::m_release = 50;
int ADSREnvelope::m_sampleRate = 44100;


}
}
