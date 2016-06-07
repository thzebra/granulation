#pragma once
#include "SequenceStrategy.hpp"
#include "time.h"

namespace Granulation {
namespace Synthesis {

class RegularSequenceStrategy : public SequenceStrategy {
public:
    RegularSequenceStrategy();

    /**
     * @brief RegularSequenceStrategy
     * @param density number of grain onsets per second
     * @param length duration of each grain in ms
     */

    RegularSequenceStrategy(int density, int length = 1);

    virtual int nextDuration();
    virtual int nextOnset();

private:
    int m_density; // number of grain onsets per second
    int m_length; // length of each grain in ms
    int m_nextonset;
};

}
}
