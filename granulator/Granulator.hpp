#pragma once
#include "Scheduler.hpp"

namespace Granulation {
namespace Synthesis {

class Granulator
{
public:
    Granulator();
    void setEssence(Envelope* env, Source* src);
    void setStrategy(SequenceStrategy* strategy);
    float synthetize();
    void generate(int n);
    int sampleRate();

private:
    Scheduler m_scheduler;
    Envelope* m_envelope {};
    Source* m_source {};
};

}
}
