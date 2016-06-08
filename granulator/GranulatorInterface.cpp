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
      m_points{std::vector<QPoint> ()},
      m_devices{new QComboBox},
      m_drawingArea{new DrawingArea},
      m_maxpoints(100)
{
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
}

void GranulatorInterface::addPoint(QPoint p) {
    qDebug("adding point (%d, %d)", p.x(), p.y());
    m_points.push_back(p);
    if (m_points.size() > m_maxpoints)
        m_points.erase(m_points.begin());
}

void GranulatorInterface::addPoint(QMouseEvent *m) {
    addPoint(m->pos());
}

void GranulatorInterface::updateLabel() {
    m_label->setText(QString::number(granulator->synthetize()));
}

}
}
