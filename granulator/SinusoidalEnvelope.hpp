#pragma once
#include "Envelope.hpp"

namespace Granulation {
namespace Synthesis {

/**
 * @brief The SinusoidalEnvelope class is a simple sinusoidal Envelope. Its values describe half a sine:
 * i-th value is equal to sin(i * M_PI / envelope_length)
 */

class SinusoidalEnvelope final : public Envelope {
public:
    SinusoidalEnvelope();
    SinusoidalEnvelope(int64_t length, int sampleRate);
    void fill() override;
    ~SinusoidalEnvelope() = default;
};

}
}
