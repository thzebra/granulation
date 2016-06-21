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
    virtual void synthetize(std::vector<float>& vec) = 0;
    virtual void generate(int n) = 0;
    virtual int sampleRate() const = 0;
    virtual int channels() const = 0;
    virtual void updateTime(double streamTime) = 0;
    virtual int maxGrains() const = 0;

    virtual void setEssenceDuration(int duration) = 0;
    virtual void setEssenceData(std::shared_ptr<SourceData> sd) = 0;

    virtual void setMaxGrains(int m) = 0;
    virtual int grainCount() const = 0;
    virtual void setInteronset(int i) = 0;
    virtual void clearGrains() = 0;

    unsigned int bufferFrames{512};
};

}
}
