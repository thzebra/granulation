#pragma once
#include <vector>
#include <cassert>
#include <iostream>

namespace Granulation {
namespace Synthesis {

class Envelope {
public:
    Envelope() = default;

    Envelope(int64_t length)
    {
        std::cout << "constructing envelope of length " << length << " with a max length of " << m_data.max_size() << std::endl;
        m_data.resize(length);
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

    virtual void recompute(int64_t length) = 0;

    virtual ~Envelope() = default;

protected:
    std::vector<float> m_data;
};


}
}
