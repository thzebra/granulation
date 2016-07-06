#include "CustomWindowSource.hpp"

namespace Granulation {
namespace Synthesis {

CustomWindowSource::CustomWindowSource()  : Source(), m_length(0) {}
CustomWindowSource::CustomWindowSource(std::shared_ptr<SourceData> source, int length, int begin)  :
    Source(source, length, begin),
    m_rawdata{source},
    m_length{length}
{}

const unsigned int CustomWindowSource::size() const {
    return m_length;
}

float CustomWindowSource::data(int i) const {
    if (m_rawdata == nullptr || i < 0 || i >= m_length)
        return 0.f;
    int index = (i + m_begin) % m_rawdata->size();
    return m_rawdata->data(index);
}

int CustomWindowSource::sampleRate() const {
    return m_rawdata->sampleRate();
}

int CustomWindowSource::channels() const {
    return m_rawdata->channels();
}

SourceData& CustomWindowSource::rawData() const {
    if (m_rawdata != nullptr)
        return *m_rawdata;
}

gsl::span<const float> CustomWindowSource::data() const {
    return m_rawdata->data();
}

void CustomWindowSource::resize(int length) {
    if (length >= 0)
        m_length = length;
}

}
}
