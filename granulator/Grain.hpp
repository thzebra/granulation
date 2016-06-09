#pragma once
#include "Envelope.hpp"
#include "Source.hpp"
#include <string>

namespace Granulation {
namespace Synthesis {

class Grain {
public:
    //Grain();
    Grain(Envelope& e, Source& s);
    Grain(const Grain&);
    ~Grain();


    bool completed() const;
    void activate(int duration);
    float synthetize();
    bool isActive() const;
    bool isReadBackwards() const;
    void operator=(const Grain&);
    std::string grainToString() const;

private:
    Envelope& m_envelope;
    Source& m_source;
    int m_index {0};
    int m_sampleRate {44100};
    int m_duration {0}; // in ms
    bool m_active {false};
    bool m_readBackwards {false};
    bool m_completed {false};
};


}
}
