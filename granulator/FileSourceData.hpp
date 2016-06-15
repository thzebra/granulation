#pragma once
#include "SourceData.hpp"
#include <vector>

namespace Granulation {
namespace Synthesis {

class FileSourceData : public SourceData
{
public:
    FileSourceData();
    virtual void populate() override;
    virtual const unsigned int size() const override;
    virtual float data(int i) const override;
    virtual int sampleRate() const override;
    virtual void setSource(std::string filename) override;
    virtual int channels() const override;

private:
    std::vector<float> m_data;
    std::string m_filename;
    int m_sampleRate;
    int m_channels;
};

}
}
