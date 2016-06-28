#include "Source.hpp"

namespace Granulation {
namespace Synthesis {

Source::Source() {}
Source::Source(SourceData *sd, int length) {}

unsigned int Source::beginning() const {
    return m_begin;
}

}
}
