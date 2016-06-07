#include "SinusoidalEnvelope.hpp"
#include <math.h>

namespace Granulation {
namespace Synthesis {

SinusoidalEnvelope::SinusoidalEnvelope() {}
SinusoidalEnvelope::SinusoidalEnvelope(int length) : Envelope(length) {}

void SinusoidalEnvelope::fill() {
    if (size() == 0)
        return;

    int s = size();
    for (int i = 0; i < s; ++i)
        m_data[i] = sin(M_PI * i / s);
}

}
}
