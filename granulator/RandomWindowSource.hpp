#pragma once
#include <vector>
#include "Source.hpp"
#include "SourceData.hpp"

namespace Granulation {
namespace Synthesis {

class RandomWindowSource : public Source {
public:
    RandomWindowSource();
    RandomWindowSource(int length, SourceData * source);
    virtual const unsigned int size() const override;
    virtual float data(int i) const override;
    virtual int sampleRate() const override;

private:
    std::vector<float> m_data;
    SourceData* m_rawdata;
};

}
}
