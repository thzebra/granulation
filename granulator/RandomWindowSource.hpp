#pragma once
#include <vector>
#include "Source.hpp"
#include "SourceData.hpp"
#include <memory>

namespace Granulation {
namespace Synthesis {

class RandomWindowSource : public Source {
public:
    RandomWindowSource();
    RandomWindowSource(std::shared_ptr<SourceData> source, int length);
    virtual const unsigned int size() const override;
    virtual float data(int i) const override;
    virtual int sampleRate() const override;
    virtual int channels() const override;
    virtual SourceData& rawData() const override;

private:
    std::vector<float> m_data;
    std::shared_ptr<SourceData> m_rawdata;
};

}
}
