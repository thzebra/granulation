#include "Wrapper.hpp"
#include "Granulator.hpp"
#include "GranulatorInterface.hpp"

Wrapper::Wrapper() {}

void Wrapper::synth() {
    Wrapper::interface->m_display->setText(QString::number(Wrapper::granulator->synthetize()));
}
