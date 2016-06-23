#pragma once
#include "Grain.hpp"
#include "SourceData.hpp"
#include "EssenceBase.hpp"

namespace Granulation {
namespace Synthesis {

template <class Env, class Src>
class Essence final : public EssenceBase
{
public:
    Essence() = delete;
    Essence(const Essence<Env, Src>& ess) :
        m_rawdata{ess.m_rawdata},
        m_length{ess.m_length}
    {}
    Essence(std::shared_ptr<SourceData> srcdata, int grainduration) :
        m_rawdata(srcdata),
        m_length(grainduration * srcdata->sampleRate() / 1000)
    {}

    virtual Grain makeGrain() override {
        return {std::make_shared<Env>(m_length), std::make_shared<Src>(m_rawdata, m_length)};
    }

    virtual int channels() const override {
        return m_rawdata->channels();
    }

    virtual int sampleRate() const override{
        return m_rawdata->sampleRate();
    }

    int length() const override {
        return m_length;
    }

    int grainDuration() const override {
        return 1000 * m_length / m_rawdata->sampleRate();
    }

    std::shared_ptr<SourceData> rawData() const override {
        return m_rawdata;
    }

    void setDuration(int duration) {
        m_length = duration * m_rawdata->sampleRate() / 1000;
    }

private:
    std::shared_ptr<SourceData> m_rawdata;
    int m_length;
};
}
}
