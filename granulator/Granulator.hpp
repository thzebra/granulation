#pragma once
#include "Scheduler.hpp"
#include "Essence.hpp"
#include "GranulatorBase.hpp"
#include "DummyScheduler.hpp"

namespace Granulation {
namespace Synthesis {

template <class Env, class Src>
class Granulator : public GranulatorBase
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
        return m_scheduler.synthetize();
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

    virtual int maxDensity() const override {
        return m_scheduler.maxDensity();
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

    void setEssenceData(SourceData& sd) override {

    }

    void setMaxGrains(int m) {
        if (m > 0)
            m_maxgrains = m;
    }

private:
    Scheduler m_scheduler;
    Essence<Env, Src> m_essence;
    int m_maxgrains{1};
};

}
}
