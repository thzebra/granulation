#include "Envelope.hpp"
#include <QDebug>
#include <cassert>

namespace Granulation {
namespace Synthesis {

void Envelope::setADSR(int attms, int decms, int susms, int relms, int samplerate) {
    m_attack = std::max(attms, 0);
    m_decay = std::max(decms, 0);
    m_release = std::max(relms, 0);

    if (m_sampleRate > 0)
        m_sampleRate = samplerate;

    int length = (attms + decms + susms + relms) * (m_sampleRate / 1000.f);

    recompute(length);
}

}
}
