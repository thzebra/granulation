#pragma once

namespace Granulation {
namespace Synthesis {

class SequenceStrategy
{
public:
    SequenceStrategy();

    /**
     * @brief nextDuration
     * @return the duration of the next grain (in ms)
     */
    virtual int nextDuration() = 0;

    /**
     * @brief nextOnset
     * @return the duration before the next grain (in ms)
     */
    virtual int nextOnset() = 0;

    /**
     * @brief update Updates the strategy's clock
     * @return whether or not a new grain should be activated
     */

    virtual bool update(double streamTime) = 0;
    virtual ~SequenceStrategy() = default;
    virtual int maxDensity() const = 0;
    double totalTime() const;

protected:
    SequenceStrategy(int interonset);
    int m_interonset {0};

    double m_totalTime {0}; // in seconds with precision >= 1ms
    double m_timeSinceLastOnset {0}; // in ms
};


}
}
