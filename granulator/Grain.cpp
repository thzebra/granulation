#include "Grain.hpp"
#include <cmath>
#include <QDebug>
#include <sstream>

namespace Granulation {
namespace Synthesis {

//Grain::Grain() {}

Grain::Grain(std::shared_ptr<Envelope> e, std::shared_ptr<Source> s) :
    m_source{s},
    m_envelope{e}
{
    //qDebug() << "grain size" << size() << "env size" << m_envelope->size() << "src size" << m_source->size() << m_source->channels();
}

Grain::Grain(const Grain & grain) :
    m_active{grain.m_active},
    m_readBackwards{grain.m_readBackwards},
    m_index{grain.m_index},
    m_source{grain.m_source},
    m_envelope{grain.m_envelope},
    m_completed{grain.m_completed}
{
}

void Grain::operator =(const Grain& grain) {

    m_source = grain.m_source;
    m_envelope = grain.m_envelope;

    m_sourceChannels = grain.m_sourceChannels;
    m_sourceSize = grain.m_sourceSize;
    m_envelopeSize = grain.m_envelopeSize;

    m_active = grain.m_active;
    m_readBackwards = grain.m_readBackwards;
    m_completed = grain.m_completed;
    m_index = grain.m_index;
}

bool Grain::completed() const {
    return m_completed;
}

void Grain::activate(bool readBackwards) {
        m_completed = false;
        m_active = true;
        m_index = 0;
        m_channelindex = 0;
        m_readBackwards = readBackwards;
}

float Grain::synthetize(bool loop) {
    // Chaque appel à un std::shared_ptr<>-> fait un load et implique une mutex, donc
    // on les charge une fois au début
    const Source& source = *m_source;
    const Envelope& envelope = *m_envelope;
    const int nc = m_sourceChannels;
    const int srcs = m_sourceSize;

    if (m_index * nc + m_channelindex >= srcs || m_index < 0) {
        if (loop) {
            m_index = 0;
            m_channelindex = 0;
        }
        else {
            markRemove();
            m_completed = true;
            return 0.f;
        }
    }

    int idx = m_index * nc + m_channelindex;

    if (m_readBackwards)
        idx = srcs - 1 - idx;

    m_channelindex = (m_channelindex + 1) % nc;
    if (m_channelindex == 0)
        ++m_index;

    return source.data(idx) * envelope.data(m_envelopeIndex++ % m_envelopeSize);
}


void Grain::synthetize(gsl::span<float> vec, bool loop) {
    // Chaque appel à un std::shared_ptr<>-> fait un load et implique une mutex, donc
    // on les charge une fois au début
    const Source& source = *m_source;
    const auto& envelope = m_envelope->data();

    const int nc = m_sourceChannels;
    const int srcs = m_sourceSize;

    const int inputSize = vec.size();

    auto fast_data = source.data();
    if(fast_data.empty())
    {
        for(int i = 0; i < inputSize && !m_completed; i++)
        {
            if (m_index * nc + m_channelindex >= srcs || m_index < 0) {
                if (loop) {
                    m_index = 0;
                    m_channelindex = 0;
                }
                else {
                    m_completed = true;
                    markRemove();
                }
            }

            if (!m_completed) {

                int idx = m_channelindex + m_index * nc;

                if (m_readBackwards)
                    int idx = srcs - 1 - idx;

                m_channelindex = (m_channelindex + 1) % nc;
                if (m_channelindex == 0)
                    ++m_index;

                vec[i] += source.data(idx) * envelope[m_envelopeIndex++ % m_envelopeSize];
            }
        }
    }
    else
    {
        if(!m_readBackwards)
        {
            // This should be the most taken branch
            for(int i = 0; i < inputSize && !m_completed; ++i)
            {
                if (m_index * nc + m_channelindex >= srcs || m_index < 0) {
                    if (loop) {
                        m_index = 0;
                        m_channelindex = 0;
                    }
                    else {
                        m_completed = true;
                        markRemove();
                    }
                }
                if (!m_completed) {
                    int idx = m_channelindex + m_index * nc;

                    m_channelindex = (m_channelindex + 1) % nc;
                    if (m_channelindex == 0)
                        ++m_index;

                    vec[i] = fast_data[idx] * envelope[m_envelopeIndex++ % m_envelopeSize];
                }
            }
        }
        else
        {
            for(int i = 0; i < inputSize && !m_completed; i++)
            {
                if (m_index * nc + m_channelindex >= srcs || m_index < 0)
                {
                    if (loop) {
                        m_index = 0;
                        m_channelindex = 0;
                    }
                    else {
                        m_completed = true;
                        markRemove();
                    }
                }

                if (!m_completed) {
                    int idx = srcs - 1 - (m_channelindex + m_index * nc);

                    vec[i] = fast_data[idx] * envelope[m_envelopeIndex++ % m_envelopeSize];

                    m_channelindex = (m_channelindex + 1) % nc;
                    if (m_channelindex == 0)
                        ++m_index;
                }
            }
        }
    }
}

bool Grain::isActive() const {
    return m_active;
}

bool Grain::isReadBackwards() const {
    return m_readBackwards;
}

std::string Grain::grainToString() const {
    std::stringstream ss;
    if (m_active)
        ss << "Active, ";
    else
        ss << "Inactive, ";
    ss << "index = " << m_index;
    //ss << ", duration: " << m_speed << "ms";
    ss << ", is read " << (m_readBackwards ? "backwards" : "forward");
    ss << ", has " << (m_completed ? "completed" : "not completed yet");
    std::string str = ss.str();
    int length = str.size();
    char cstr[length + 1];
    str.copy(cstr, length, 0);
    cstr[length] = 0;
    return std::string(cstr);
}

void Grain::markRemove() {
    m_toRemove = true;
}

bool Grain::toRemove() const {
    return m_toRemove;
}

unsigned int Grain::beginning() const {
    if (m_source != nullptr)
        return m_source->beginning();
    else
        return 0;
}

unsigned int Grain::size() const {
    if (m_source == nullptr)
        return 0;
    int nch = m_source->channels();
    nch > 0 ? m_source->size() / nch : 0;
}

void Grain::resize(int newsize) {
    if (newsize >= 0) {
        m_envelope->recompute(newsize);
        m_source->resize(newsize * m_source->channels());
    }
}

int Grain::channels() const {
    if (m_source != nullptr)
        return m_source->channels();
    return 1;
}

int Grain::sampleRate() const {
    if (m_source != nullptr)
        return m_source->sampleRate();
    return 44100;
}

int Grain::currentIndex() const {
    return m_envelopeIndex;
}

}
}
