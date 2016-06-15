#include "RegularSequenceStrategy.hpp"
#include <QDebug>

namespace Granulation {
namespace Synthesis {

RegularSequenceStrategy::RegularSequenceStrategy() {}
RegularSequenceStrategy::RegularSequenceStrategy(int density, int length) :
    SequenceStrategy(1000 / density),
    m_density(density),
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
        //qDebug() << "updating time to" << m_totalTime << "delta" << (deltatime * 1000) << "ms";


        // Is it time to activate a new grain?

        m_timeSinceLastOnset += deltatime * 1000;
        int next = nextOnset();
        if (next <= 0) {
            m_timeSinceLastOnset = (-next) % m_interonset;
            //qDebug() << "activating new grain; time since last onset is" << m_timeSinceLastOnset;
            return true;
        }
    }
    return false;
}

int RegularSequenceStrategy::maxDensity() const {
    int res = m_density * m_length / 1000;
    //qDebug() << "densité max =" << res;
    return res;
}

}
}
