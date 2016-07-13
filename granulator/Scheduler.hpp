#pragma once
#include <vector>
#include <deque>
#include "Grain.hpp"
#include "Essence.hpp"
#include <span.h>
#include <mutex>
#include <rubberband/RubberBandStretcher.h>

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
    virtual ~Scheduler();
    virtual float synthetize(int maxgrains = 1, bool loop = false);
    virtual void synthetize(gsl::span<float> vec, int maxgrains, bool loop = false);
    virtual void setStrategy(SequenceStrategy* strategy);

    virtual void addGrain(const Grain& g, int maxgrains);

    virtual void activateNext();
    virtual int grainCount() const;
    virtual void updateTime(double streamTime);
    virtual void setInteronset(int i);
    virtual void clearGrains();
    virtual const Grain& lastGrainAdded() const;
    virtual const std::deque<Grain>& grains() const;
    virtual void setReadDirection(bool readBackwards);

private:
    mutable std::mutex m_grainsLock;
    std::deque<Grain> m_grains;
    SequenceStrategy* m_strategy;
    void removeCompleted();
    int m_actives{0};
    bool m_readBackwards{false};
};

}
}
