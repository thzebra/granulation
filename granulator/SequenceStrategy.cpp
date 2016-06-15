#include "SequenceStrategy.hpp"

namespace Granulation {
namespace Synthesis {

SequenceStrategy::SequenceStrategy() {}
SequenceStrategy::SequenceStrategy(int interonset) :
    m_interonset(interonset)
{}

double SequenceStrategy::totalTime() const {
    return m_totalTime;
}

}
}
