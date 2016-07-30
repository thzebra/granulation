#pragma once
#include "Source.hpp"
#include "SourceData.hpp"
#include <memory>
#include <vector>
#include <deque>

namespace Granulation {
namespace Synthesis {

/**
 * @brief The CustomWindowSource class defines a type of Source where the length and
 * beginning can be defined by the user.
 */

class CustomWindowSource : public Source {
public:

    /**
     * @brief Default constructor that does nothing
     */

    CustomWindowSource();

    /**
     * @brief Constructor
     * @param source: a shared_pointer to the SourceData to sample
     * @param length: the length in samples
     * @param begin: the position of the first sample of the window in the SourceData
     */

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
