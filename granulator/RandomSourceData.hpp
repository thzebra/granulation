#pragma once
#include "SourceData.hpp"
#include <vector>

namespace Granulation {
namespace Synthesis {

/**
 * @brief The RandomSourceData class is a SourceData of user-defined length that is filled
 * with random samples.
 *
 * A RandomSourceData has two channels and an arbitrary sample rate of 44100Hz. This kind of
 * SourceData is mostly intended as a test case. The random samples are not truly random.
 */

class RandomSourceData : public SourceData {
public:
    RandomSourceData();
    RandomSourceData(int length);

    /**
     * @brief Populates the data array with floats between -1 and 1.
     *
     * The data is obtained from random integers between -1000 and 1000
     * that are then divided by 1000.
     */
    virtual void populate() override;
    virtual const int unsigned size() const override;
    virtual float data(int i) const override;

    /**
     * @brief Gets the sample rate.
     * @return A dummy sample rate equal to 44100Hz.
     */
    virtual int sampleRate() const override;

    /**
     * @brief Gets the number of channels.
     * @return 2
     */
    virtual int channels() const override;

    std::vector<float> m_data;
};

}
}
