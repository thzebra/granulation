#include "RandomWindowSource.hpp"
#include <time.h>
#include <cstdlib>
#include <QDebug>

namespace Granulation {
namespace Synthesis {

RandomWindowSource::RandomWindowSource() {}
RandomWindowSource::RandomWindowSource(int length, SourceData * source) :
    m_data(std::vector<float> (length, 0.f)),
    m_rawdata{source}
{
    if (source) {
        std::srand(time(nullptr));
        int datasize = (int) source->size();
        if (datasize > 0) {
            int begin = std::rand() % datasize;
            for (int i = 0; i < length; ++i) {
                int idx = (begin + i) % datasize;
                m_data[i] = source->data(idx);
            }
        }
    }
}

const unsigned int RandomWindowSource::size() const {
    return m_data.size();
}

float RandomWindowSource::data(int i) const {
    // qDebug() << "fetching sample" << i << "out of" << m_data.size();
    float res = m_data[i];
    return res;
}

int RandomWindowSource::sampleRate() const {
    return m_rawdata->sampleRate();
}

}
}
