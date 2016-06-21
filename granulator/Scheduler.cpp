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
    std::lock_guard<std::mutex> lock(m_grainsLock);

    while (m_grains.size() > 0 && m_grains.begin()->toRemove()) {
        m_grains.pop_front();
        --m_actives;
    }

    if (m_grains.size() == 0 || maxgrains <= 0)
        return 0.f;

    int nactive = 0;
    int ncompleted = 0;
    float amp = 0.f;

    for(Grain& grain : m_grains)
    {
        if (grain.isActive() && !grain.completed()) {
            amp += grain.synthetize();
            ++nactive;
            if (grain.completed()) {
                ++ncompleted;
                --m_actives;
            }
        }
    }
    // qDebug() << "SCHEDULER GOT AMPLITUDE" << amp;
    return amp / (nactive > 0 ? nactive : maxgrains);
}

void Scheduler::synthetize(gsl::span<float> vec, int maxgrains) {
    std::lock_guard<std::mutex> lock(m_grainsLock);

    while (m_grains.size() > 0 && m_grains.begin()->toRemove()) {
        m_grains.pop_front();
        --m_actives;
    }

    if (m_grains.size() == 0 || maxgrains <= 0)
        return;

    int nactive = 0;
    int ncompleted = 0;

    for(Grain& grain : m_grains)
    {
        if (grain.isActive() && !grain.completed()) {
            grain.synthetize(vec); // The grain mixes itself.
            ++nactive;
            if (grain.completed()) {
                ++ncompleted;
                --m_actives;
            }
        }
    }

    const int vec_size = vec.size();
    float max = 0;
    for(int i = 0; i < vec_size; i++)
    {
        max = std::max(vec[i], max);
    }
    const float factor = 1. / max;
    for(int i = 0; i < vec_size; i++)
    {
        vec[i] *= factor;
    }
}
void Scheduler::setStrategy(SequenceStrategy *strategy) {
    m_strategy = strategy;
}

void Scheduler::addGrain(const Grain& g, int maxgrains) {
    std::lock_guard<std::mutex> lock(m_grainsLock);
    int toMark = std::max(int(m_grains.size() - maxgrains), 0);
    int nth = 0;
    for (auto it = m_grains.begin(); it != m_grains.end() && nth < toMark; ++it) {
        ++nth;
        it->markRemove();
    }
    m_grains.push_back(g);
}

void Scheduler::activateNext() {
    std::lock_guard<std::mutex> lock(m_grainsLock);
    auto it = m_grains.begin();
    for (; it != m_grains.end() && it->isActive(); ++it) {}
    if (it != m_grains.end()) {
        it->activate(m_strategy->nextDuration());
        ++m_actives;
    }
}

void Scheduler::removeCompleted() {}

int Scheduler::grainCount() const {
    std::lock_guard<std::mutex> lock(m_grainsLock);
    return m_grains.size();
}

void Scheduler::updateTime(double streamTime) {
    if (grainCount() > 0 && m_strategy->update(streamTime))
    {
        activateNext();
    }
}

void Scheduler::setInteronset(int i) {
    m_strategy->setInteronset(i);
}

void Scheduler::clearGrains() {
    std::lock_guard<std::mutex> lock(m_grainsLock);
    m_grains.clear();
}

}
}
