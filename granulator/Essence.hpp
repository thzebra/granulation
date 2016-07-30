#pragma once
#include "Grain.hpp"
#include "SourceData.hpp"
#include "EssenceBase.hpp"

namespace Granulation {
namespace Synthesis {

template <class Env, class Src>

/**
 * @brief The Essence class is a template class storing paramters for creating new grains
 */

class Essence final : public EssenceBase
{
public:

    /**
     * @brief Default constructor (deleted).
     */

    Essence() = delete;

    /**
     * @brief Copy constructor
     * @param ess: a constant reference to the Essence to copy
     */

    Essence(const Essence<Env, Src>& ess) :
        m_rawdata{ess.m_rawdata},
        m_length{ess.m_length}
    {}

    /**
     * @brief Constructor
     * @param srcdata: the shared_pointer to the SourceData to use
     * @param grainduration: duration of each generated Grain, in milliseconds
     */

    Essence(std::shared_ptr<SourceData> srcdata, int grainduration) :
        m_rawdata(srcdata),
        m_length(grainduration * srcdata->sampleRate() / 1000)
    {}

    /**
     * @brief Dynamically calculates the Envelope and Source required and makes a grain from them
     * @return The Grain generated from the Essence's parameters.
     */

    virtual Grain makeGrain() override {
        return {std::make_shared<Env>(m_length, m_rawdata->sampleRate()), std::make_shared<Src>(m_rawdata, m_length, m_begin)};
    }

    /**
     * @brief Get the number of channels.
     * @return The number of channels in the essence's SourceData.
     */

    virtual int channels() const override {
        return m_rawdata->channels();
    }

    /**
     * @brief Get the sample rate.
     * @return the sample rate stored in the Essence
     */

    virtual int sampleRate() const override{
        return m_rawdata->sampleRate();
    }

    /**
     * @brief Gets the length of grains generated by this essence.
     * @return The number of frames of the essence.
     */

    int length() const override {
        return m_length;
    }

    /**
     * @brief Gets grain duration.
     * @return An integer representing the duration a Grain created from the Essence will have (in ms)
     */

    int grainDuration() const override {
        return 1000 * m_length / m_rawdata->sampleRate();
    }

    /**
     * @brief Returns the SourceData used by the essence.
     * @return The shared pointer to the essence's SourceData
     */

    std::shared_ptr<SourceData> rawData() const override {
        return m_rawdata;
    }

    /**
     * @brief Sets the Essence's grain duration.
     * @param duration: An integerrepresenting the new duration (in ms)
     */

    void setDuration(int duration) override {
        auto l = duration * (m_rawdata->sampleRate() / 1000.f);
        //std::cout << "set duration to " << duration << "ms ie " << l << " samples" << std::endl;
        m_length = l;
    }

    /**
     * @brief Sets the beginning position of the window used by Grains created by the Essence
     * @param firstSample the position of the first sample
     */

    void setBegin(int firstSample) override {
        m_begin = firstSample;
    }

private:

    /**
     * @brief m_rawdata the pointer to the SourceData used by the Essence
     */

    std::shared_ptr<SourceData> m_rawdata;

    /**
     * @brief m_length the length of Grains created by the Essence (aka the number of frames)
     */

    int m_length;

    /**
     * @brief m_begin the beginning of the window used by the Grains created by this Essence
     */

    int m_begin;
};
}
}
