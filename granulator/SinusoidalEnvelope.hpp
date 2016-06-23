#pragma once
#include "Envelope.hpp"

namespace Granulation {
namespace Synthesis {

class SinusoidalEnvelope final : public Envelope {
public:
    SinusoidalEnvelope();
    SinusoidalEnvelope(int length);
    void fill() override;
    ~SinusoidalEnvelope() = default;
};

}
}
