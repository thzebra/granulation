#pragma once
#include "Grain.hpp"

namespace Granulation {
namespace Synthesis {

class EssenceBase
{
public:
    EssenceBase();
    virtual ~EssenceBase() = default;

    virtual Grain makeGrain() = 0;
    virtual int channels() const = 0;
    virtual int sampleRate() const = 0;

    virtual int length() const = 0;
    virtual int grainDuration() const = 0;
    virtual std::shared_ptr<SourceData> rawData() const = 0;

    virtual void setDuration(int duration) = 0;
    virtual void setBegin(int firstSample) = 0;
};

}
}
