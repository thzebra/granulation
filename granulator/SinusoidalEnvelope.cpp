#include "SinusoidalEnvelope.hpp"
#include <cmath>

namespace Granulation {
namespace Synthesis {

SinusoidalEnvelope::SinusoidalEnvelope() {}
SinusoidalEnvelope::SinusoidalEnvelope(int length) : Envelope(length) {
    fill();
}

void SinusoidalEnvelope::fill() {
    if (size() == 0)
        return;

    int s = size();
    for (int i = 0; i < s; ++i)
        m_data[i] = std::sin(M_PI * i / s);
}

}
}
