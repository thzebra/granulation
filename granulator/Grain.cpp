#include "Grain.hpp"
#include <cmath>
#include <QDebug>
#include <sstream>
#include <iostream>

namespace Granulation {
namespace Synthesis {

using namespace RubberBand;

//Grain::Grain() {}

Grain::Grain(std::shared_ptr<Envelope> e, std::shared_ptr<Source> s, double timeratio, double pitchscale) :
    m_source{s},
    m_envelope{e},
    m_timeRatio{timeratio},
    m_pitchScale{pitchscale},
    m_rbs{s->sampleRate(), s->channels(), RubberBandStretcher::OptionProcessRealTime, timeratio, pitchscale}
{
    //deinterleave();
}

Grain::Grain(const Grain & grain) :
    m_active{grain.m_active},
    m_readBackwards{grain.m_readBackwards},
    m_index{grain.m_index},
    m_source{grain.m_source},
    m_sourceSize{grain.m_sourceSize},
    m_sourceChannels{grain.m_sourceChannels},
    m_envelope{grain.m_envelope},
    m_envelopeSize{grain.m_envelopeSize},
    m_completed{grain.m_completed},
    m_pitchScale{grain.m_pitchScale},
    m_timeRatio{grain.m_timeRatio},
    m_rbs{m_source->sampleRate(), m_source->channels(),
          RubberBandStretcher::OptionProcessRealTime,
          grain.m_timeRatio, grain.m_pitchScale}
{
    //deinterleave();
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
    m_rbs = RubberBandStretcher(sampleRate(), channels(), RubberBandStretcher::OptionProcessRealTime, grain.getTimeRatio(), grain.getPitchScale());
    deinterleave();
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
    if (nc == 0)
        return;

    const int srcs = m_sourceSize;

    const int nOutFrames = vec.size();

    auto fast_data = source.data();
    if(fast_data.empty())
    {
        for(int i = 0; i < nOutFrames && !m_completed; i++)
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

//            qDebug() << m_rbs.available() << "frames available,"
//                     << nOutFrames << "needed in out buffer";
//            qDebug() << "latency:" << m_rbs.getLatency();
//            qDebug() << m_rbs.getSamplesRequired() << "samples required";

            const int nOutFramesChan = (const int) (nOutFrames / nc);
            while (m_rbs.available() < nOutFrames) {
                int nNeededFrames = std::min(nOutFrames, int(m_rbs.getSamplesRequired()));
                if (nNeededFrames > 0) {
                    float * toProcess[nc];
                    for (int i = 0; i < nc; ++i) {
                        toProcess[i] = (float *) calloc(nOutFramesChan, sizeof(float));
                    }
                    for (int j = 0; j < nOutFrames; ++j) {
                        for (int i = 0; i < nc; ++i)
                            toProcess[i][j] = source.data(m_index + j * nc + i) * envelope[m_envelopeIndex];
                        m_envelopeIndex = (m_envelopeIndex + 1) % m_envelopeSize;
                    }
                    m_rbs.process(toProcess, nNeededFrames, false);
                    m_index = (m_index + nNeededFrames) % m_envelopeSize;
                    //qDebug() << "available:" << m_rbs.available();

                }
            }

            float * output[nc];
            for (int i = 0; i < nc; ++i)
                output[i] = (float*)calloc(nOutFrames, sizeof(float));

            int retrieved = m_rbs.retrieve(output, nOutFrames);
            for (int i = 0; i < nc; ++i) {
                for (int j = 0; j < nOutFramesChan; ++j) {
                    vec[j * nc + i] = output[nc][j];
                }
            }

            // This should be the most taken branch
//            for(int i = 0; i < nOutFrames && !m_completed; ++i)
//            {
//                if (m_index * nc + m_channelindex >= srcs || m_index < 0) {
//                    if (loop) {
//                        m_index = 0;
//                        m_channelindex = 0;
//                    }
//                    else {
//                        m_completed = true;
//                        markRemove();
//                    }
//                }
//                if (!m_completed) {
//                    int idx = m_channelindex + m_index * nc;

//                    m_channelindex = (m_channelindex + 1) % nc;
//                    if (m_channelindex == 0)
//                        ++m_index;

//                    vec[i] = fast_data[idx] * envelope[m_envelopeIndex++ % m_envelopeSize];
//                }
//            }
        }
        else
        {
            for(int i = 0; i < nOutFrames && !m_completed; i++)
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

double Grain::getPitchScale() const {
    return m_pitchScale;
}

double Grain::getTimeRatio() const {
    return m_timeRatio;
}

void Grain::setPitchScale(double pitchscale) {
    m_pitchScale = pitchscale;
    m_rbs.setPitchScale(pitchscale);
}

void Grain::setTimeRatio(double timeratio) {
    m_timeRatio = timeratio;
    m_rbs.setTimeRatio(timeratio);
}

void Grain::deinterleave() {
    int nc = channels();
    m_deinterleavedSource.resize(nc);

    const Source& src = *m_source;
    const Envelope& env = *m_envelope;
    int envsize = env.size();

    auto chansize = size() + src.rawData().overflowSize();
    for (int i = 0; i < nc; ++i) {
        m_deinterleavedSource[i].resize(chansize);
    }

    for (int i = 0; i < chansize; ++i) {
        for (int j = 0; j < nc; ++j) {
            m_deinterleavedSource[j][i] = src.data(i * nc + j) * env.data(i % envsize);
        }
    }
}

}
}
