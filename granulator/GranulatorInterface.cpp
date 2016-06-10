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
      m_layout{new QVBoxLayout},
      m_points{std::vector<QPoint> (0)},
      m_devices{new QComboBox},
      m_drawingArea{new DrawingArea},
      m_maxpoints(100),
      m_pan{0}
{
    resize(600, 600);

    m_layout->addWidget(m_drawingArea);
    m_layout->addWidget(m_devices);
    m_layout->addWidget(m_button);
    m_layout->addWidget(m_label);

    m_central->setLayout(m_layout);
    this->setCentralWidget(m_central);
}

GranulatorInterface::~GranulatorInterface()
{
    delete m_devices;
    delete m_button;
    delete m_label;
    delete m_layout;
    delete m_central;
    m_points.~vector();
}

void GranulatorInterface::addPoint(QPoint p) {
    m_points.push_back(p);
    setPan((2 * (float)p.x() / (float)m_central->height()) - 1.f);
    granulator->generate(1);
}

void GranulatorInterface::addPoint(QMouseEvent *m) {
    addPoint(m->pos());
}

void GranulatorInterface::updateLabel() {
    m_label->setText(QString::number(granulator->synthetize()));
}

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

}
}
