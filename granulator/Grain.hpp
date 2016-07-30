#pragma once
#include "Envelope.hpp"
#include "Source.hpp"
#include <string>
#include <memory>
#include <span.h>
#include <rubberband/RubberBandStretcher.h>

namespace Granulation {
namespace Synthesis {

class Grain {
public:
    //Grain();

    /**
     * @brief Constructor for the class Grain
     * @param e: A shared_ptr to an Envelope.
     * @param s: A shared_ptr to a Source.
     * @param timeratio: The time stretch ratio for the grain.
     * @param pitchscale: The pitch shift for the grain.
     */

    Grain(std::shared_ptr<Envelope> e, std::shared_ptr<Source> s, double timeratio = 1.0, double pitchscale = 1.0);

    /**
     * @brief Copy Constructor.
     * @param g: The Grain to copy.
     */

    Grain(const Grain& g);
    ~Grain() = default;

    /**
     * @brief Gets the Grain's status
     *
     * The Grain is completed if it has reached its end without being
     * looped or if it was replaced by a newer Grain.
     *
     * @return A boolean that is true if the Grain has completed, false otherwise.
     */

    bool completed() const;

    /**
     * @brief Sets the Grain's status to "active".
     * @param readBackwards: true if the Grain is to be read backwards, false otherwise.
     */

    void activate(bool readBackwards);

    /**
     * @brief Mixes the Grain's Source and Envelope to make a new sample.
     *
     * This function increments the Grain's internal indexes.
     *
     * @param loop: true if the Grain loops, false otherwise.
     * @return A float between -1 and 1 corresponding to the current sample.
     */

    float synthetize(bool loop = false);

    /**
     * @brief Mixes the Grain's Source and Envelope to make vec.size() new samples.
     * @param vec: A span of floats that will be filled with the new samples.
     * @param loop: true if the Grain loops, false otherwise.
     */

    void synthetize(gsl::span<float> vec, bool loop = false);

    /**
     * @brief Gets the Grain's active status.
     * @return True if the Grain is active (ie if it is being read), false otherwise.
     */

    bool isActive() const;

    /**
     * @brief Gets the Grain's direction of reading.
     * @return True if the Grain is read backwards, false otherwise.
     */

    bool isReadBackwards() const;

    /**
     * @brief Assignment operator. All contents are copied.
     * @param g: The Grain whose contents will be copied to the current Grain.
     */

    void operator=(const Grain& g);

    /**
     * @brief A debug function that makes a human-readable representation of the Grain.
     * @return A std::string.
     */

    std::string grainToString() const;

    /**
     * @brief Marks the Grain as having to be removed from the Grain queue.
     */

    void markRemove();

    /**
     * @brief Check if the Grain should be removed from the Grain queue.
     * @return True if the Grain should be removed, false otherwise.
     */

    bool toRemove() const;

    /**
     * @brief Gets the position of the Grain's first sample in the SourceData it was
     * made from.
     * @return An integer.
     */

    unsigned int beginning() const;

    /**
     * @brief Gets the number of frames in the Grain.
     * @return An integer.
     */

    unsigned int size() const;

    /**
     * @brief Gets the number of channels in the Grain.
     * @return An integer.
     */

    int channels() const;

    /**
     * @brief Gets the sample rate of the Grain.
     * @return An integer.
     */

    int sampleRate() const;

    /**
     * @brief Gets the Grain's current frame index in its Source.
     * @return An integer.
     */

    int currentIndex() const;

    /**
     * @brief Resizes the Grain's Envelope and Source.
     * @param newsize: The new size the Grain should have, in frame numbers.
     */

    void resize(int newsize);

    /**
     * @brief Gets the Grain's pitch scale.
     * See RubberBandStretcher's documentation for more information on pitch shifting
     * and time stretching.
     * @return A double.
     */

    double getPitchScale() const;

    /**
     * @brief Gets the Grain's time stretch.
     *
     * See RubberBandStretcher's documentation for more information on pitch shifting
     * and time stretching.
     * @return A double.
     */

    double getTimeRatio() const;

    /**
     * @brief Sets the new pitch scale.
     *
     * See RubberBandStretcher's documentation for more information on pitch shifting
     * and time stretching.
     * @param pitchscale: The Grain's new pitch scale.
     */

    void setPitchScale(double pitchscale);

    /**
     * @brief Sets the time ratio.
     *
     * See RubberBandStretcher's documentation for more information on pitch shifting
     * and time stretching.
     * @param timeratio: The Grain's new time ratio.
     */

    void setTimeRatio(double timeratio);

private:
    std::shared_ptr<Envelope> m_envelope;
    std::shared_ptr<Source> m_source;

    int m_sourceChannels{m_source->channels()};
    int m_sourceSize{m_source->size()};
    int m_envelopeSize{m_envelope->size()};

    int m_index {0};
    int m_channelindex{0};
    int m_envelopeIndex{0};
    bool m_active {false};
    bool m_readBackwards {false};
    bool m_completed {false};
    bool m_toRemove{false};

    double m_pitchScale{1.f};
    double m_timeRatio{1.f};
    std::shared_ptr<RubberBand::RubberBandStretcher> m_rbs{}; /**< The RubberBandStretcher manages pitch shifting and time stretching */
};


}
}
