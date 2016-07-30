#pragma once
#include "Scheduler.hpp"

namespace Granulation {
namespace Synthesis {

/**
 * @brief The DummyScheduler class is a test class.
 *
 * The DummyScheduler class is only meant to be used as a test class. Its functions
 * have no effect and return arbitrary values. Its synthetize function returns a sinus
 * of frequency 440Hz with a sample rate of 48000.
 */

class DummyScheduler : public Scheduler
{
public:
    DummyScheduler();
    float synthetize(int maxgrains = 1, bool loop = false) override;
    void setStrategy(SequenceStrategy *strategy);
    void addGrain(const Grain &g);
    void activateNext();
    int grainCount() const;
    void updateTime(double streamTime);
    int maxDensity() const;

    int index; /**< the step number, used in the sinus */
};

}
}
