#pragma once
#include "SourceData.hpp"

namespace Granulation {
namespace Synthesis {

class Source
{
public:
    Source();
    Source(SourceData* sd);
    virtual const unsigned int size() const = 0;
    virtual float data(int i) const = 0;
    virtual int sampleRate() const = 0;
};

}
}
