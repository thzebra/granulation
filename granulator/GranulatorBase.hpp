#pragma once
#include "EssenceBase.hpp"
#include "SequenceStrategy.hpp"
#include "Scheduler.hpp"

namespace Granulation {
namespace Synthesis {

/**
 * @brief The GranulatorBase class serves as a non-template base for the template class Granulator
 */

class GranulatorBase
{
public:
    GranulatorBase();
    virtual ~GranulatorBase() = default;

    /**
     * @brief setStrategy sets the strategy to use for the granulator
     * @param strategy: a pointer to the strategy to use
     */

    virtual void setStrategy(SequenceStrategy* strategy) = 0;

    /**
     * @brief synthetize Computes a new sample from all active grains
     * @return A float
     */

    virtual float synthetize() = 0;

    /**
     * @brief synthetize Computes S new samples from all active grains and stores the results in the vec array.
     * (where S is the size of the vec array)
     * If the loop parameter is true, then grains are looped (ie read again from the beginning when they reach their end),
     * otherwise grains end when reaching their end.
     * @param vec An array of floats; contents are irrelevant
     * @param loop Whether grains should be looped
     */

    virtual void synthetize(gsl::span<float> vec, bool loop = false) = 0;

    /**
     * @brief generate Generates n new grains from the stored Source and Envelope
     * @param n A positive integer
     */

    virtual void generate(int n) = 0;

    /**
     * @brief sampleRate Returns the sample rate of the current Source
     * @return A positive integer
     */

    virtual int sampleRate() const = 0;

    /**
     * @brief channels Returns the number of channels of the current Source
     * @return A positive integer
     */

    virtual int channels() const = 0;

    /**
     * @brief updateTime Updates the Scheduler
     * @param streamTime The number of milliseconds elapsed since the beginning
     */

    virtual void updateTime(double streamTime) = 0;

    /**
     * @brief maxGrains returns the maximal number of grains managed by the granulator
     * @return An integer
     */

    virtual int maxGrains() const = 0;

    /**
     * @brief Returns the last grain added to the granulator
     * @return A constant reference to the last grain added
     */

    virtual const Grain& lastGrainAdded() const = 0;

    /**
     * @brief Sets the duration used by the granulator's essence
     * @param duration: An integer representing the number of milliseconds the essence shall use
     */

    virtual void setEssenceDuration(int duration) = 0;

    /**
     * @brief Sets the data used by the granulator's essence
     * @param sd: A shared pointer to the SourceData the essence shall use
     */

    virtual void setEssenceData(std::shared_ptr<SourceData> sd) = 0;

    /**
     * @brief Sets the maximal number of grains the granulator can manage
     * @param m: A positive integer representing the maximal number of grains
     */

    virtual void setMaxGrains(int m) = 0;

    /**
     * @brief Returns the number of Grains the granulator is currently managing
     * @return A positive integer
     */

    virtual int grainCount() const = 0;

    /**
     * @brief Sets the time between two grain onsets
     * @param i: An integer representing the time between two successive onsets, in milliseconds
     */

    virtual void setInteronset(int i) = 0;

    /**
     * @brief Removes all grains from the granulator
     */

    virtual void clearGrains() = 0;

    /**
     * @brief Sets the looping option in the granulator to b
     * @param b: A boolean. If b is true, then grains will be looped; otherwise, they will only be read once each.
     */

    virtual void setLoop(bool b) = 0;

    /**
     * @brief Gets the granulator's looping option
     * @return True if the granulator loops the grains, false otherwise.
     */

    virtual bool loop() = 0;

    /**
     * @brief Sets the beginning of any newly created Grain in the SourceData to the given sample number.
     *
     * @param begin: An integer representing the sample number that should be used as the beginning
     * of each new grain's sample window
     */

    virtual void setBegin(int begin) = 0;

    /**
     * @brief Gets the grains currently managed by the granulator
     * @return A constant reference to the double-ended queue of grains managed by the granulator
     */

    virtual const std::deque<Grain>& grains() const = 0;

    /**
     * @brief Sets the direction of reading
     * @param readBackwards: A boolean; true means grains will be read backwards,
     * false that they will be read forward.
     */

    virtual void setReadDirection(bool readBackwards) = 0;

    /**
     * @brief Number of frames in the output buffer.
     */

    unsigned int bufferFrames{512};
};

}
}
