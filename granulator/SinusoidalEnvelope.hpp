#pragma once
#include "Envelope.hpp"

namespace Granulation {
namespace Synthesis {

class SinusoidalEnvelope final : public Envelope {
public:
    SinusoidalEnvelope();
    SinusoidalEnvelope(int64_t length);
    void fill() override;
    void recompute(int64_t length) override;
    ~SinusoidalEnvelope() = default;
};

}
}
