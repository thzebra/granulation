#pragma once
#include "Source.hpp"
#include "SourceData.hpp"
#include <memory>
#include <vector>
#include <deque>

namespace Granulation {
namespace Synthesis {

class CustomWindowSource : public Source {
public:
    CustomWindowSource();
    CustomWindowSource(std::shared_ptr<SourceData> source, int length, int begin);

    // Source interface
public:
    const unsigned int size() const override;
    float data(int i) const override;
    int sampleRate() const override;
    int channels() const override;
    SourceData &rawData() const override;
    gsl::span<const float> data() const override;
    void resize(int length) override;

protected:
    unsigned int m_length;
    std::shared_ptr<SourceData> m_rawdata;
};

}
}
