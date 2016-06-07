#include "RandomSourceData.hpp"
#include <cstdlib>
#include <time.h>

namespace Granulation {
namespace Synthesis {

RandomSourceData::RandomSourceData() {}
RandomSourceData::RandomSourceData(int length) : m_data(std::vector<float> (length, 0.f)){}

void RandomSourceData::populate() {
    int s = m_data.size();
    srand(time(nullptr));
    for (int i = 0; i < s; ++i) {
        int r = rand() % 2001;
        m_data[i] = (r - 1000) / 1000.;
    }
}

const unsigned int RandomSourceData::size() const {
    return m_data.size();
}

float RandomSourceData::data(int i) const {
    return m_data[i];
}

int RandomSourceData::sampleRate() const {
    return 44100;
}

}
}
