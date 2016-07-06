#pragma once
#include "Envelope.hpp"
#include "Source.hpp"
#include <string>
#include <memory>
#include <span.h>

namespace Granulation {
namespace Synthesis {

class Grain {
public:
    //Grain();
    Grain(std::shared_ptr<Envelope> e, std::shared_ptr<Source> s);
    Grain(const Grain&);
    ~Grain() = default;

    bool completed() const;
    void activate(bool readBackwards);
    float synthetize(bool loop = false);
    void synthetize(gsl::span<float> vec, bool loop = false);
    bool isActive() const;
    bool isReadBackwards() const;
    void operator=(const Grain&);
    std::string grainToString() const;
    void markRemove();
    bool toRemove() const;
    unsigned int beginning() const;
    unsigned int size() const;
    int channels() const;
    int sampleRate() const;

    int currentIndex() const;

    void resize(int newsize);

private:
    std::shared_ptr<Envelope> m_envelope;
    std::shared_ptr<Source> m_source;

    int m_sourceChannels{m_source->channels()};
    int m_sourceSize{m_source->size()};
    int m_envelopeSize{m_envelope->size()};

    int m_index {0};
    int m_channelindex{0};
    int m_envelopeIndex{0};
    bool m_active {false};
    bool m_readBackwards {false};
    bool m_completed {false};
    bool m_toRemove{false};
};


}
}
