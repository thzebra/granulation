#pragma once
#include "SourceData.hpp"
#include <vector>

namespace Granulation {
namespace Synthesis {

/**
 * @brief The FileSourceData class is a SourceData whose data is extracted from a
 * given source file.
 *
 * FileSourceData uses all the relevant properties of the sound file such as sample rate and
 * number of channels. The supported file type is WAV.
 *
 * Additionally, FileSourceData's data array emulates a ring array to enable easy looping
 * of source files, ie one can read "too far" in the array (to a certain extent) and get the
 * beginning of the source file as if a modulo was applied.
 */

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
