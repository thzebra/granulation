#pragma once
#include <vector>
#include <cassert>
#include <iostream>

namespace Granulation {
namespace Synthesis {

class Envelope {
public:
    Envelope() = default;

    Envelope(int64_t length, int sampleRate = 44100)
    {
        m_data.resize(length);
        m_sampleRate = sampleRate;
    }

    Envelope(const Envelope& env):
        m_data{env.m_data}
    {
    }

    Envelope(Envelope&& env):
        m_data{std::move(env.m_data)}
    {
    }

    const std::size_t size() const
    {
        return m_data.size();
    }

    float data(std::size_t i) const
    {
        assert(i >= 0 && i < m_data.size());

        return m_data[i];
    }

    const std::vector<float>& data() const
    {
        return m_data;
    }

    virtual void fill() = 0;
    virtual void setADSR(int attms, int decms, int susms, int relms, int samplerate);

    virtual void recompute(int64_t length) = 0;

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
