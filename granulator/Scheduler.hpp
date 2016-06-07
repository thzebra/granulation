#pragma once
#include <vector>
#include "Grain.hpp"

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
    float synthetize();
    void setStrategy(SequenceStrategy* strategy);
    void addGrain(Envelope* e, Source* s);
    void activateNext();

private:
    std::vector<Grain> m_grains;
    SequenceStrategy* m_strategy;
    void removeCompleted();
};

}
}
