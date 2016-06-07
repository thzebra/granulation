#include "Scheduler.hpp"
#include "SequenceStrategy.hpp"
#include "Grain.hpp"
#include <QDebug>

namespace Granulation {
namespace Synthesis {

Scheduler::Scheduler() {}
Scheduler::Scheduler(SequenceStrategy *strategy) :
    m_grains{std::vector<Grain> ()},
    m_strategy{strategy}
{}

float Scheduler::synthetize() {
    qDebug() << "scheduler is synthetizing from" << m_grains.size() << "grains";
    int nactive = 0;
    int ncompleted = 0;
    float amp = 0.f;
    auto it = m_grains.begin();
    for (; it != m_grains.end(); ++it) {
        auto grain = *it;
        if (grain.isActive()) {
            qDebug() << "grain is active";
            amp += grain.synthetize();
            ++it;
            ++nactive;
        }
        if (grain.completed())
            ++ncompleted;
    }
    removeCompleted();
    qDebug() << nactive << "grains were active and" << ncompleted << "grains completed for a synthetized value of" << amp;
    return amp;
}

void Scheduler::setStrategy(SequenceStrategy *strategy) {
    m_strategy = strategy;
}

void Scheduler::addGrain(Envelope *e, Source *s) {
    qDebug() << "adding new grain";
    m_grains.push_back(Grain(e, s));
}

void Scheduler::activateNext() {
    qDebug() << "activating next grain...";
    auto it = m_grains.begin();
    for (; it != m_grains.end() && it->isActive(); ++it) {}
    if (it != m_grains.end()) {
        it->activate(m_strategy->nextDuration());
        qDebug() << "grain was activated";
    }
    else {
        qDebug() << "no grain was activated";
    }
}

void Scheduler::removeCompleted() {
    for (auto it = m_grains.end(); it != m_grains.begin();) {
        --it;
        if ((*it).completed())
            m_grains.erase(it);
    }
}

}
}
