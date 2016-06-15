#pragma once
#include "EssenceBase.hpp"
#include "SequenceStrategy.hpp"

namespace Granulation {
namespace Synthesis {

class GranulatorBase
{
public:
    GranulatorBase();
    virtual ~GranulatorBase() = default;

    virtual void setStrategy(SequenceStrategy* strategy) = 0;
    virtual float synthetize() = 0;
    virtual void generate(int n) = 0;
    virtual int sampleRate() const = 0;
    virtual int channels() const = 0;
    virtual void updateTime(double streamTime) = 0;
    virtual int maxDensity() const = 0;

    virtual void setEssenceDuration(int duration) = 0;
    virtual void setEssenceData(SourceData& sd) = 0;

    virtual void setMaxGrains(int m) = 0;

    unsigned int bufferFrames{512};
};

}
}
