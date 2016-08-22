#pragma once
#include "SequenceStrategy.hpp"
#include "time.h"

namespace Granulation {
namespace Synthesis {

/**
 * @brief The RegularSequenceStrategy class is a Strategy class that activates
 * grains at regular intervals.
 */

class RegularSequenceStrategy final : public SequenceStrategy {
public:
    RegularSequenceStrategy();

    /**
     * @brief RegularSequenceStrategy
     * @param density number of grain onsets per second
     * @param length duration of each grain in ms
     */

    RegularSequenceStrategy(int density, int length = 1);

    int nextDuration() override;
    int nextOnset() override;
    bool update(double streamTime) override;

private:
    int m_length; // length of each grain in ms
};

}
}
