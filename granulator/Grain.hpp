#pragma once
#include "Envelope.hpp"
#include "Source.hpp"
#include <string>
#include <memory>

namespace Granulation {
namespace Synthesis {

class Grain {
public:
    //Grain();
    Grain(std::shared_ptr<Envelope> e, std::shared_ptr<Source> s);
    Grain(const Grain&);
    ~Grain() = default;

    bool completed() const;
    void activate(int duration);
    float synthetize();
    bool isActive() const;
    bool isReadBackwards() const;
    void operator=(const Grain&);
    std::string grainToString() const;
    void markRemove();
    bool toRemove() const;

private:
    std::shared_ptr<Envelope> m_envelope;
    std::shared_ptr<Source> m_source;
    int m_index {0};
    int m_channelindex{0};
    //int m_duration {0}; // in ms
    bool m_active {false};
    bool m_readBackwards {false};
    bool m_completed {false};
    bool m_toRemove{false};
};


}
}
