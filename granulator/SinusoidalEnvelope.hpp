#pragma once
#include "Envelope.hpp"

namespace Granulation {
namespace Synthesis {

class SinusoidalEnvelope final : public Envelope {
public:
    SinusoidalEnvelope();
    SinusoidalEnvelope(int64_t length, int sampleRate);
    void fill() override;
    ~SinusoidalEnvelope() = default;
};

}
}
