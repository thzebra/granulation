#pragma once
#include <vector>

namespace Granulation {
namespace Synthesis {

class Envelope {
public:
    Envelope();
    Envelope(int length);
    Envelope(const Envelope& env);
    const unsigned int size() const;
    const float data(int i) const;
    const std::vector<float> data() const;
    virtual void fill() = 0;

protected:
    std::vector<float> m_data;
};


}
}
