#pragma once
#include "Envelope.hpp"

namespace Granulation {
namespace Synthesis {

class SinusoidalEnvelope : public Envelope {
public:
    SinusoidalEnvelope();
    SinusoidalEnvelope(int length);
    void fill() override;
};

}
}
