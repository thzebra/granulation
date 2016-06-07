#include "RandomWindowSource.hpp"
#include <time.h>
#include <cstdlib>

namespace Granulation {
namespace Synthesis {

RandomWindowSource::RandomWindowSource() {}
RandomWindowSource::RandomWindowSource(int length, SourceData * source) :
    m_data(std::vector<float> (length, 0.f)),
    m_rawdata{source}
{
    int beginmax = m_rawdata->size() - length;
    if (beginmax >= 0) {
        int begin = 0;
        if (beginmax > 0) {
            srand(time(nullptr));
            begin = rand() % (beginmax + 1);
        }
        for (int i = 0; i < length; ++i) {
            m_data[i] = m_rawdata->data(begin + i);
        }
    }
}

const unsigned int RandomWindowSource::size() const {
    return m_data.size();
}

float RandomWindowSource::data(int i) const {
    return m_data[i];
}

int RandomWindowSource::sampleRate() const {
    return m_rawdata->sampleRate();
}

}
}
