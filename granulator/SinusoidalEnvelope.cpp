#include "SinusoidalEnvelope.hpp"
#include <cmath>

namespace Granulation {
namespace Synthesis {

SinusoidalEnvelope::SinusoidalEnvelope() {}
SinusoidalEnvelope::SinusoidalEnvelope(int64_t length, int sampleRate) : Envelope(length, sampleRate) {
    fill();
}

void SinusoidalEnvelope::fill() {
    if (size() == 0)
        return;

    auto s = size();
    for (int64_t i = 0; i < s; ++i)
        m_data[i] = std::sin(M_PI * i / s);
}

void SinusoidalEnvelope::recompute(int64_t length) {
    m_data.clear();
    m_data.resize(length);
    fill();
}

}
}
