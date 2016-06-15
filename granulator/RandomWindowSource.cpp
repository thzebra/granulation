#include "RandomWindowSource.hpp"
#include <time.h>
#include <cstdlib>
#include <QDebug>

namespace Granulation {
namespace Synthesis {

RandomWindowSource::RandomWindowSource() {}
RandomWindowSource::RandomWindowSource(std::shared_ptr<SourceData> source, int length) :
    m_rawdata{source}
{
    if (source) {
        int nsamples = length * source->channels();
        m_data.resize(nsamples);
        std::srand(time(nullptr));
        int datasize = (int) source->size();
        if (datasize > 0) {
            int begin = std::rand() % datasize;
            //qDebug() << "first sample at" << begin << "out of" << source->size();
            for (int i = 0; i < nsamples; ++i) {

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
    /*qDebug() << "fetching sample" << i << "out of" << m_data.size();*/
    return m_data[i];
}

int RandomWindowSource::sampleRate() const {
    return m_rawdata->sampleRate();
}

int RandomWindowSource::channels() const {
    return m_rawdata->channels();
}

SourceData& RandomWindowSource::rawData() const {
    if (m_rawdata)
        return *m_rawdata;
}

}
}
