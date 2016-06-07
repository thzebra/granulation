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

protected:
    SequenceStrategy(int interonset);
    int m_interonset {0};
};


}
}
