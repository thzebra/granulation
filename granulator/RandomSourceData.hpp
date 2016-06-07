#pragma once
#include "SourceData.hpp"
#include <vector>

namespace Granulation {
namespace Synthesis {

class RandomSourceData : public SourceData {
public:
    RandomSourceData();
    RandomSourceData(int length);

    virtual void populate() override;
    virtual const int unsigned size() const override;
    virtual float data(int i) const override;
    virtual int sampleRate() const override;

    std::vector<float> m_data;
};

}
}
