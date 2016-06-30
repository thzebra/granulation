#include "Source.hpp"

namespace Granulation {
namespace Synthesis {

Source::Source() {}
Source::Source(std::shared_ptr<SourceData> sd, int length, int begin) :
    m_begin{begin}
{}

unsigned int Source::beginning() const {
    return m_begin;
}

}
}
