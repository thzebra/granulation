#pragma once
#include "SourceData.hpp"
#include <span.h>

namespace Granulation {
namespace Synthesis {

class Source
{
public:
    Source();
    Source(SourceData* sd, int length);
    virtual const unsigned int size() const = 0;
    virtual float data(int i) const = 0;
    virtual int sampleRate() const = 0;
    virtual int channels() const = 0;
    virtual SourceData& rawData() const = 0;

    // Empty span means that the data has to be queried float by float
    virtual gsl::span<const float> data() const { return {}; }

    virtual ~Source() = default;
    unsigned int beginning() const;

protected:
    unsigned int m_begin{0};
};

}
}
