#include "GranulatorInterface.hpp"
#include <QDebug>
#include <rtaudio/RtAudio.h>
#include <exception>
#include <QMouseEvent>

namespace Granulation {
namespace Panel {

GranulatorInterface::GranulatorInterface(QWidget *parent)
    : QMainWindow(parent),
      m_central{new QWidget},
      m_button{new QPushButton("Generate!")},
      m_label{new QLabel},
      m_layout{new QGridLayout},
      m_points{std::vector<QPoint> (0)},
      m_devices{new QComboBox},
      m_drawingArea{new DrawingArea},
      m_maxpoints(100),
      m_pan{0},
      m_nthpoint{0},
      m_grainDuration{new QSpinBox},
      m_grainCount{new QSpinBox},
      m_mute{new QCheckBox},
      m_mutelabel{new QLabel},
      m_durationlabel{new QLabel},
      m_countlabel{new QLabel},
      m_density{new QSpinBox},
      m_densitylabel{new QLabel}
{
    resize(600, 600);

    m_grainCount->setMinimum(1);
    m_grainCount->setMaximum(1000);
    m_grainCount->setValue(10);

    m_grainDuration->setMinimum(10);
    m_grainDuration->setMaximum(10000);
    m_grainDuration->setValue(500);

    m_grainDuration->setSuffix(tr(" ms"));
    m_mute->setCheckState(Qt::Unchecked);
    m_mutelabel->setText(tr("Mute"));
    m_countlabel->setText(tr("Max grains"));
    m_durationlabel->setText(tr("Grain duration"));

    m_density->setMinimum(1);
    m_density->setValue(100);
    m_densitylabel->setText(tr("Number of grains per second"));

    m_layout->addWidget(m_drawingArea, 0, 0, 1, 2);
    m_layout->addWidget(m_devices, 1, 0, 1, 2);
    //m_layout->addWidget(m_button);
    //m_layout->addWidget(m_label);
    m_layout->addWidget(m_countlabel, 2, 0);
    m_layout->addWidget(m_grainCount, 2, 1);
    m_layout->addWidget(m_durationlabel, 3, 0);
    m_layout->addWidget(m_grainDuration, 3, 1);
    m_layout->addWidget(m_mutelabel, 4, 0);
    m_layout->addWidget(m_mute, 4, 1);

    m_central->setLayout(m_layout);
    this->setCentralWidget(m_central);

    connect(m_grainDuration,
            static_cast<void (QSpinBox::*)(int)> (&QSpinBox::valueChanged),
            [=] (int d) mutable -> void { granulator->setEssenceDuration(d); });
    connect(m_grainCount, static_cast<void (QSpinBox::*)(int)> (&QSpinBox::valueChanged),
            [=] (int c) mutable -> void { granulator->setMaxGrains(c); });
}

GranulatorInterface::~GranulatorInterface()
{
    delete m_devices;
    delete m_button;
    delete m_label;
    delete m_layout;
    delete m_central;
    delete granulator;
    m_points.~vector();
}

void GranulatorInterface::addPoint(QPoint p) {
    m_points.push_back(p);
    setPan((2 * float(p.x()) / float(m_central->height())) - 1.f);
    granulator->generate(1);
}

void GranulatorInterface::addPoint(QMouseEvent *m) {
    if (m_nthpoint == 0)
        addPoint(m->pos());
    m_nthpoint = (m_nthpoint + 1) % 50;
}

void GranulatorInterface::updateLabel() {}

float GranulatorInterface::pan() {
    return m_pan;
}

void GranulatorInterface::setPan(float p) {
    if (p < -1.f) {
        m_pan = -1.f;
        return;
    }
    if (p > 1.f) {
        m_pan = 1.f;
        return;
    }
    m_pan = p;
}

void GranulatorInterface::emptyPoints() {
    m_points.clear();
}

}
}
