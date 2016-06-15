#include "Scheduler.hpp"
#include "SequenceStrategy.hpp"
#include "Grain.hpp"
#include <QDebug>
#include <algorithm>

namespace Granulation {
namespace Synthesis {

Scheduler::Scheduler() {}

Scheduler::Scheduler(SequenceStrategy *strategy) :
    m_grains{},
    m_strategy{strategy}
{}

Scheduler::~Scheduler() {
    delete m_strategy;
}

float Scheduler::synthetize(int maxgrains) {
    if (m_grains.size() == 0)
        return 0.f;

    int nactive = 0;
    int ncompleted = 0;
    float amp = 0.f;
    std::deque<Grain>::iterator it;
    for (it = m_grains.begin(); it != m_grains.end(); ++it) {
        if (it->isActive() && !it->completed()) {
            amp += it->synthetize();
            ++nactive;
            if (it->completed()) {
                ++ncompleted;
                --m_actives;
            }
        }
    }
    //removeCompleted();
    if (nactive)
    if (amp < 0)
        return amp / maxgrains;
}

void Scheduler::setStrategy(SequenceStrategy *strategy) {
    m_strategy = strategy;
}

void Scheduler::addGrain(const Grain& g, int maxgrains) {
    while (m_grains.size() >= maxgrains)
        m_grains.pop_front();
    m_grains.push_back(g);
}

void Scheduler::activateNext() {
    auto it = m_grains.begin();
    for (; it != m_grains.end() && it->isActive(); ++it) {}
    if (it != m_grains.end()) {
        it->activate(m_strategy->nextDuration());
        ++m_actives;
    }
}

void Scheduler::removeCompleted() {}

int Scheduler::grainCount() const {
    return m_grains.size();
}

void Scheduler::updateTime(double streamTime) {
    if (m_grains.size() > 0 && m_strategy->update(streamTime))
        activateNext();
}

void Scheduler::setInteronset(int i) {
    m_strategy->setInteronset(i);
}

void Scheduler::clearGrains() {
    m_grains.clear();
}

}
}
