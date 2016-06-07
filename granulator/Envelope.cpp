#include "Envelope.hpp"

namespace Granulation {
namespace Synthesis {

Envelope::Envelope() {}
Envelope::Envelope(int length) {
    m_data.resize(sizeof(float) * length);
}
Envelope::Envelope(const Envelope& env) {
    m_data = env.data();
}

const float Envelope::data(int i) const {
    if (i >= m_data.size() || i < 0)
        return 0;
    else
        return m_data[i];
}

const std::vector<float> Envelope::data() const {
    const std::vector<float> vec = m_data;
    return vec;
}

const unsigned Envelope::size() const {
    return m_data.size();
}

}
}
