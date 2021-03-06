#pragma once
#include "Scheduler.hpp"
#include "Essence.hpp"
#include "GranulatorBase.hpp"
#include "DummyScheduler.hpp"

#include <span.h>
#include <iostream>

namespace Granulation {
namespace Synthesis {

template <class Env, class Src>

/**
 * @brief The Granulator class manages Grains.
 *
 * It implements all functions of its parent class GranulatorBase. It contains
 * a Scheduler for scheduling grain activation and an Essence able to make
 * Grains.
 */

class Granulator final : public GranulatorBase
{
public:
    Granulator() = delete;
    Granulator(std::shared_ptr<SourceData> sd, int grainduration) :
        m_essence{sd, grainduration}
    {}
    ~Granulator() {
       // m_scheduler.~Scheduler();
    }

    virtual void setStrategy(SequenceStrategy* strategy) override {
        m_scheduler.setStrategy(strategy);
    }

    virtual float synthetize() override {
        return m_scheduler.synthetize(m_maxgrains, m_loop);
    }

    virtual void synthetize(gsl::span<float> vec, bool loop = false) override {
        m_scheduler.synthetize(vec, m_maxgrains, loop);
    }

    virtual void generate(int n) override {
        for (int i = 0; i < n; ++i) {
            m_scheduler.addGrain(m_essence.makeGrain(), m_maxgrains);
        }
    }

    virtual int sampleRate() const override {
        return m_essence.sampleRate();
    }

    virtual int channels() const override {
        return m_essence.channels();
    }

    virtual int maxGrains() const override {
        return m_maxgrains;
    }

    virtual void updateTime(double streamTime) override {
        m_scheduler.updateTime(streamTime);
    }

    void setEssence(const Essence<Env, Src>& essence) {
        m_essence = essence;
    }

    void setEssenceDuration(int duration) override {
        m_essence.setDuration(duration);
    }

    void setEssenceData(std::shared_ptr<SourceData> sd) override {
        Essence<Env, Src> ess (sd, m_essence.grainDuration());
        m_essence = ess;
    }

    void setMaxGrains(int m) {
        if (m > 0)
            m_maxgrains = m;
    }

    int grainCount() const {
        return m_scheduler.grainCount();
    }

    void setInteronset(int i) override {
        m_scheduler.setInteronset(i);
    }

    void clearGrains() override {
        m_scheduler.clearGrains();
    }

    void setLoop(bool b) override {
        m_loop = b;
    }

    bool loop() override {
        return m_loop;
    }

    const Grain &lastGrainAdded() const {
        return m_scheduler.lastGrainAdded();
    }

    void setBegin(int begin) override {
        m_essence.setBegin(begin);
    }

    const std::deque<Grain>& grains() const override {
        return m_scheduler.grains();
    }

    void setReadDirection(bool readBackwards) override {
        m_scheduler.setReadDirection(readBackwards);
    }

private:
    Scheduler m_scheduler;
    Essence<Env, Src> m_essence;
    int m_maxgrains{1};
    bool m_loop{false};
};

}
}
