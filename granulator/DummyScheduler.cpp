#include "DummyScheduler.hpp"
#include "math.h"
#include <QDebug>

namespace Granulation {
namespace Synthesis {

DummyScheduler::DummyScheduler() : index(0) {}

float DummyScheduler::synthetize(int maxgrains) {
    //qDebug() << "plop" << index;
    return sin(2.f * M_PI * index++ * 440.f / 48000.f);
}

void DummyScheduler::addGrain(const Grain &g) {}
void DummyScheduler::activateNext() {}
int DummyScheduler::grainCount() const {return 1;}
void DummyScheduler::updateTime(double streamTime) {}
int DummyScheduler::maxDensity() const {return 1;}
void DummyScheduler::setStrategy(SequenceStrategy *strategy) {}

}
}


