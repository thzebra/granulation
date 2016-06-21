#pragma once
#include <vector>
#include "Source.hpp"
#include "SourceData.hpp"
#include <memory>

namespace Granulation {
namespace Synthesis {

class RandomWindowSource final: public Source {
public:
    RandomWindowSource();
    RandomWindowSource(std::shared_ptr<SourceData> source, int length);
    const unsigned int size() const override {
        return m_data.size();
    }
    float data(int i) const override {
        return m_data[i];
    }
    int sampleRate() const override {
        return m_rawdata->sampleRate();
    }
    int channels() const override {
        return m_rawdata->channels();
    }
    SourceData& rawData() const override {
        if (m_rawdata)
            return *m_rawdata;
    }

private:
    std::vector<float> m_data;
    std::shared_ptr<SourceData> m_rawdata;
};


}
}
