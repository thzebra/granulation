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
    const unsigned int size() const override;
    float data(int i) const override;
    int sampleRate() const override;
    int channels() const override;
    SourceData& rawData() const override;

private:
    std::vector<float> m_data;
    std::shared_ptr<SourceData> m_rawdata;
};

}
}
