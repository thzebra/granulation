#pragma once
#include "Envelope.hpp"

namespace Granulation {
namespace Synthesis {

class ADSREnvelope : public Envelope {
public:
    ADSREnvelope();
    ADSREnvelope(int64_t length, int sampleRate);

    // Envelope interface
public:
    void fill() override;
    void recompute(int64_t length) override;

    ~ADSREnvelope() = default;
};

}
}
