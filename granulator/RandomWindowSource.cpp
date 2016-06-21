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
            int begin = std::rand();
            begin = (begin - begin % source->channels()) % datasize; // ensuring canal alignment
            qDebug() << "first sample at" << begin << "out of" << source->size() << "last sample at" << ((begin + length - 1) % datasize);
            for (int i = 0; i < nsamples; ++i) {

                int idx = (begin + i) % datasize;
                m_data[i] = source->data(idx);
            }
        }
    }
}











}
}
