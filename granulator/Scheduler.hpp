#pragma once
#include <vector>
#include <deque>
#include "Grain.hpp"
#include "Essence.hpp"

namespace Granulation {
namespace Synthesis {

class SequenceStrategy;
class Envelope;
class Source;

class Scheduler
{
public:
    Scheduler();
    Scheduler(SequenceStrategy* strategy);
    ~Scheduler();
    virtual float synthetize();
    virtual void setStrategy(SequenceStrategy* strategy);

    virtual void addGrain(const Grain& g, int maxgrains);

    virtual void activateNext();
    virtual int grainCount() const;
    virtual void updateTime(double streamTime);
    virtual int maxDensity() const;

private:
    std::deque<Grain> m_grains;
    SequenceStrategy* m_strategy;
    void removeCompleted();
    int m_actives{0};
};

}
}
