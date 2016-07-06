#pragma once
#include "SourceData.hpp"
#include <vector>

namespace Granulation {
namespace Synthesis {

class FileSourceData final : public SourceData
{
public:
    FileSourceData();
    void populate() override;
    const unsigned int size() const override;
    float data(int i) const override;
    gsl::span<const float> data() const override;
    int sampleRate() const override;
    void setSource(std::string filename) override;
    int channels() const override;

private:
    std::vector<float> m_data;
    std::string m_filename;
    int m_sampleRate;
    int m_channels;
};

}
}
