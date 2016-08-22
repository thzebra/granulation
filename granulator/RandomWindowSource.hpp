#pragma once
#include <vector>
#include "Source.hpp"
#include "SourceData.hpp"
#include <memory>

namespace Granulation {
namespace Synthesis {

/**
 * @brief The RandomWindowSource class is a type of Source that has
 * a user-defined length and a first sample chosen at random in the
 * source sample.
 */

class RandomWindowSource final: public Source {
public:

    /**
     * @brief Default constructor that does nothing.
     */
    RandomWindowSource();

    /**
     * @brief Constructor for the RandomWindowSource class.
     * @param source: A shared_ptr to the SourceData used for this source.
     * @param length: The number of frames the source will span.
     * @param begin: A dummy argument included for the sake of compatibility.
     */
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


/**
 * @brief The RandomWindowSourceView class is similar to the RandomWindowSource class
 * except that is does not copy data from the original sample.
 */

class RandomWindowSourceView final: public Source {
public:

    /**
     * @brief Default constructor that does nothing.
     */
    RandomWindowSourceView();

    /**
     * @brief Constructor for the RandomWindowSource class.
     * @param source: A shared_ptr to the SourceData used for this source.
     * @param length: The number of frames the source will span.
     * @param begin: A dummy argument included for the sake of compatibility.
     */
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
