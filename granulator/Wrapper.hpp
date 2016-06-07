#pragma once
#include <QObject>
#include "Granulator.hpp"
#include "GranulatorInterface.hpp"

using namespace Granulation;

class Wrapper : public QObject
{
    Q_OBJECT
public:
    Wrapper();

    static Panel::GranulatorInterface* interface;
    static Synthesis::Granulator* granulator;

public slots:
    static void synth();
};

Panel::GranulatorInterface* Wrapper::interface = new Panel::GranulatorInterface ();
Synthesis::Granulator* Wrapper::granulator = new Synthesis::Granulator ();
