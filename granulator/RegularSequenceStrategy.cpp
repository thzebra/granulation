#include "RegularSequenceStrategy.hpp"
#include <QDebug>

namespace Granulation {
namespace Synthesis {

RegularSequenceStrategy::RegularSequenceStrategy() {}
RegularSequenceStrategy::RegularSequenceStrategy(int density, int length) :
    SequenceStrategy(1000 / density),
    m_length(length)
{}

int RegularSequenceStrategy::nextDuration() {
    return m_length;
}

int RegularSequenceStrategy::nextOnset() {
    return m_interonset - m_timeSinceLastOnset;
}

bool RegularSequenceStrategy::update(double streamTime) {
    if (streamTime > m_totalTime) {
        double deltatime = streamTime - m_totalTime;
        m_totalTime = streamTime;

        // Is it time to activate a new grain?

        m_timeSinceLastOnset += deltatime * 1000;
        int next = nextOnset();
        if (next <= 0) {
            m_timeSinceLastOnset = (-next) % m_interonset;
            return true;
        }
    }
    return false;
}

}
}
