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
    RandomWindowSource(std::shared_ptr<SourceData> source, int length, int begin = 0);
    const unsigned int size() const override {
        return m_data.size();
    }
    float data(int i) const override {
        return m_data[i];
    }

    gsl::span<const float> data() const override {
        return m_data;
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

    void resize(int length) override;

private:
    std::vector<float> m_data;
    std::shared_ptr<SourceData> m_rawdata;
};


class RandomWindowSourceView final: public Source {
public:
    RandomWindowSourceView();
    RandomWindowSourceView(std::shared_ptr<SourceData> source, int length, int begin);

    const unsigned int size() const override {
        return m_length;
    }

    float data(int i) const override {
        if (m_rawdata == nullptr)
            return 0.f;
        return m_rawdata->data(i);
    }

    gsl::span<const float> data() const override {
        return {};
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
    const unsigned int m_length;
    std::shared_ptr<SourceData> m_rawdata;
};

}
}
