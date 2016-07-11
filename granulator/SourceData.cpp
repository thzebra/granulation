#include "SourceData.hpp"

namespace Granulation {
namespace Synthesis {

SourceData::SourceData(bool allowOverflow) : m_allowoverflow{allowOverflow} {}
void SourceData::setSource(std::string filename) {}

const bool SourceData::allowsOverflow() const {
    return m_allowoverflow;
}

int SourceData::overflowSize() const {
    if (allowsOverflow())
        return 4096;
    else
        return 0;
}

gsl::span<const float> SourceData::data() const {
    return {};
}

}
}
