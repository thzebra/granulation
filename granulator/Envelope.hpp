#pragma once
#include <vector>
#include <cassert>
#include <iostream>

namespace Granulation {
namespace Synthesis {

/**
 * @brief The Envelope class is the base class for all Grain envelopes.
 *
 * An Envelope will be equally applied to all channels of any Source, so it
 * only ever has one channel.
 */

class Envelope {
public:
    Envelope() = default;

    /**
     * @brief Constructor for the Envelope class.
     * @param length: An integer which represents the number of frames in the Envelope
     * @param sampleRate: An integer yielding the sample rate of the Envelope (in Hz)
     */

    Envelope(int64_t length, int sampleRate = 44100)
    {
        m_data.resize(length);
        m_sampleRate = sampleRate;
    }

    /**
     * @brief Copy constructor for the Envelope class.
     * @param env: A constant reference to the Envelope to copy.
     */

    Envelope(const Envelope& env):
        m_data{env.m_data}
    {
    }

    /**
     * @brief Move constructor for the Envelope class.
     * @param env: An rvalue reference to the Envelope to move.
     */

    Envelope(Envelope&& env):
        m_data{std::move(env.m_data)}
    {
    }

    /**
     * @brief Returns the size of the envelope.
     * @return The number of samples the envelope contains.
     */

    const std::size_t size() const
    {
        return m_data.size();
    }

    /**
     * @brief Returns the sample at the given position.
     *
     * The program will crash if the position is outside of bounds.
     *
     * @param i: The position of the sample to fetch.
     * @return A sample between 0 and 1.
     */

    float data(std::size_t i) const
    {
        assert(i >= 0 && i < m_data.size());

        return m_data[i];
    }

    /**
     * @brief Returns the whole sample array.
     * @return The array of samples that make the Envelope.
     */

    const std::vector<float>& data() const
    {
        return m_data;
    }

    /**
     * @brief Fills the Envelope.
     *
     * This function must be implemented in every child class.
     */

    virtual void fill() = 0;

    /**
     * @brief Sets the attack, decay, sustain and release times of the Envelope.
     *
     * This function may resize the Envelope. It will use the combined length of attack,
     * decay, sustain and release as the new length.
     *
     * @param attms: The attack time in milliseconds.
     * @param decms: The decay time in milliseconds.
     * @param susms: The sustain time in milliseconds.
     * @param relms: The release time in milliseconds.
     * @param samplerate: The sample rate of the Source used in adequation with this envelope.
     */

    virtual void setADSR(int attms, int decms, int susms, int relms, int samplerate);

    /**
     * @brief Resizes the Envelope and re-fills it.
     * @param length: The Envelope's new length (in number of samples).
     */

    virtual void recompute(int64_t length);

    virtual ~Envelope() = default;

protected:
    std::vector<float> m_data;

    int m_attack{50};
    int m_decay{0};
    int m_release{50};
    int m_sampleRate{44100};
};



}
}
