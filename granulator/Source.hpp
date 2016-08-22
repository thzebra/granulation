#pragma once
#include "SourceData.hpp"
#include <span.h>
#include <memory>

namespace Granulation {
namespace Synthesis {

/**
 * @brief The Source class is abstract and manages a window of samples taken
 * from a SourceData.
 */

class Source
{
public:

    /**
     * @brief Dummy constructor
     */

    Source();

    /**
     * @brief Constructor for the Source class.
     * @param source: A shared pointer to a SourceData
     * @param length: The number of samples to take from the SourceData
     * @param begin: The index of the first sample of the Source in the SourceData
     */

    Source(std::shared_ptr<SourceData> source, int length, int begin);

    /**
     * @brief Gets the size of the Source
     * @return The number of samples in the Source
     */

    virtual const unsigned int size() const = 0;

    /**
     * @brief Gets a sample
     * @param i: the index of the sample
     * @return The sample at index i
     */

    virtual float data(int i) const = 0;

    /**
     * @brief Gets the sample rate
     * @return The sample rate of the audio source
     */

    virtual int sampleRate() const = 0;

    /**
     * @brief Gets the number of channels
     * @return The number of channels of the audio source
     */

    virtual int channels() const = 0;

    /**
     * @brief Gets the SourceData managed by the Source
     * @return A reference to the SourceData managed by the Source
     */

    virtual SourceData& rawData() const = 0;

    /**
     * @brief Gets the first sample
     * @return The index of the first sample of the Source in the SourceData
     */

    unsigned int beginning() const;

    /**
     * @brief Resizes the Source's length
     * @param length: the new length (in samples)
     */

    virtual void resize(int length) = 0;

    /**
     * @brief Gets the data array as a gsl::span. May be empty.
     * Empty span means that the data has to be queried float by float.
     * @return A span containing the Source's samples, or an empty span.
     */

    virtual gsl::span<const float> data() const { return {}; }

    virtual ~Source() = default;

protected:
    unsigned int m_begin{0};
};

}
}
