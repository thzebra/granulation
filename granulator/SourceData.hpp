#pragma once
#include <string>

namespace Granulation {
namespace Synthesis {

class SourceData
{
public:
    SourceData();
    virtual void populate() = 0;
    virtual const unsigned int size() const = 0;
    virtual float data(int i) const = 0;
    virtual int sampleRate() const = 0;
    virtual void setSource(std::string filename);

    virtual ~SourceData() = default;
};

}
}
