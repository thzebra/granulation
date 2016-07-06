#pragma once
#include <string>
#include <span.h>

namespace Granulation {
namespace Synthesis {

class SourceData
{
public:
    SourceData(bool allowOverflow = false);
    const bool allowsOverflow() const;
    virtual void populate() = 0;
    virtual const unsigned int size() const = 0;
    virtual float data(int i) const = 0;
    virtual gsl::span<const float> data() const;
    virtual int sampleRate() const = 0;
    virtual void setSource(std::string filename);
    virtual int channels() const = 0;

    virtual ~SourceData() = default;

protected:
    const bool m_allowoverflow;
};

}
}
