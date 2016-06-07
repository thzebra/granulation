#include "RegularSequenceStrategy.hpp"

namespace Granulation {
namespace Synthesis {

RegularSequenceStrategy::RegularSequenceStrategy() {}
RegularSequenceStrategy::RegularSequenceStrategy(int density, int length) :
    SequenceStrategy(1000 / density),
    m_density(density),
    m_length(length),
    m_nextonset(m_interonset)
{}

int RegularSequenceStrategy::nextDuration() {
    return m_length;
}

int RegularSequenceStrategy::nextOnset() {
    return m_nextonset;
}

}
}
