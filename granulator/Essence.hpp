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
        return {std::make_shared<Env>(m_length, m_rawdata->sampleRate()), std::make_shared<Src>(m_rawdata, m_length, m_begin)};
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

    void setDuration(int duration) override {
        auto l = duration * (m_rawdata->sampleRate() / 1000.f);
        //std::cout << "set duration to " << duration << "ms ie " << l << " samples" << std::endl;
        m_length = l;
    }

    void setBegin(int firstSample) override {
        m_begin = firstSample;
    }

private:
    std::shared_ptr<SourceData> m_rawdata;
    int m_length;
    int m_begin;
};
}
}
