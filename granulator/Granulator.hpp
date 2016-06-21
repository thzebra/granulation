#pragma once
#include "Scheduler.hpp"
#include "Essence.hpp"
#include "GranulatorBase.hpp"
#include "DummyScheduler.hpp"

namespace Granulation {
namespace Synthesis {

template <class Env, class Src>
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
        return m_scheduler.synthetize(m_maxgrains);
    }

    virtual void synthetize(std::vector<float>& vec) override {
        m_scheduler.synthetize(vec, m_maxgrains);
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

private:
    Scheduler m_scheduler;
    Essence<Env, Src> m_essence;
    int m_maxgrains{1};
};

}
}
