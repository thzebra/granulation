#pragma once
#include "Scheduler.hpp"

namespace Granulation {
namespace Synthesis {

class DummyScheduler : public Scheduler
{
public:
    DummyScheduler();
    float synthetize(int maxgrains = 1) override;
    void setStrategy(SequenceStrategy *strategy);
    void addGrain(const Grain &g);
    void activateNext();
    int grainCount() const;
    void updateTime(double streamTime);
    int maxDensity() const;

    int index;
};

}
}
