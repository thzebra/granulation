#pragma once
#include <string>
#include <span.h>

namespace Granulation {
namespace Synthesis {

/**
 * @brief The SourceData class is an abstract class managing raw data from
 * a complete audio source. It allows access to relevant information such as
 * channel count or sample rate, as well as access to the audio data.
 *
 * A SourceData may allow overflow, in which case there is a number N of samples
 * that can be accessed beyond the audio source's actual last sample. Those samples
 * will be equal to the first N samples of the source. N is hardcoded and equal to 4096.
 *
 * Allowing overflow to simulate wrapping for the audio buffer is done by making
 * the data array overflowSize() * channel_count samples too big. These added
 * samples are filled with the corresponding number of samples from the beginning
 * of the audio file.
 */

class SourceData
{
public:

    /**
     * @brief Constructor for the SourceData class.
     * @param allowOverflow: True if the SourceData shall allow overflow, false
     * otherwise.
     */

    SourceData(bool allowOverflow = false);

    /**
     * @brief Returns whether this SourceData allows overflow.
     * @return True if the SourceData allows overflow, false
     * otherwise.
     */

    const bool allowsOverflow() const;

    /**
     * @brief Returns the number of overflowing samples.
     * @return 4096 if the SourceData allows overflow, 0 otherwise.
     */

    int overflowSize() const;

    /**
     * @brief Populates the data array.
     */

    virtual void populate() = 0;

    /**
     * @brief Gets the size of the SourceData
     * @return The number of samples of the SourceData.
     */

    virtual const unsigned int size() const = 0;

    /**
     * @brief Gets a sample.
     * @param i: the index of the sample in the data array.
     * @return A float
     */

    virtual float data(int i) const = 0;

    /**
     * @brief Gets the data array.
     * @return A gsl::span of floats corresponding to the array of samples
     * managed by the SourceData.
     */

    virtual gsl::span<const float> data() const;

    /**
     * @brief Gets the sample rate of the audio source.
     * @return The sample rate in Hz.
     */

    virtual int sampleRate() const = 0;

    /**
     * @brief Loads a new source in the SourceData.
     * @param filename: A std::string containing the name of the new filename to import.
     */

    virtual void setSource(std::string filename);

    /**
     * @brief Gets the number of channels in the audio source.
     * @return The number of channels.
     */

    virtual int channels() const = 0;

    virtual ~SourceData() = default;

protected:
    const bool m_allowoverflow; /** A boolean equal to True if the SourceData allows overflow, false otherwise.*/
};

}
}
