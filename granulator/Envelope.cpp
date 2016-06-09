#include "Envelope.hpp"
#include <QDebug>

namespace Granulation {
namespace Synthesis {

Envelope::Envelope() : m_data(std::vector<float> (0)) {}
Envelope::Envelope(int length) {
    m_data.resize(length);
    // qDebug() << "created envelope of length" << m_data.size() << length;
}
Envelope::Envelope(const Envelope& env) {
    m_data = env.data();
    // qDebug() << m_data.size();
}

const float Envelope::data(int i) const {
    // qDebug() << "fetching envelope amplitude in" << i;
    if (i >= m_data.size() || i < 0)
        return 0.f;
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
