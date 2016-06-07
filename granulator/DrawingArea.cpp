#include "DrawingArea.hpp"
#include <QMouseEvent>
#include "GranulatorInterface.hpp"

namespace Granulation {
namespace Panel {

DrawingArea::DrawingArea(QWidget *parent) :
    QWidget(parent),
    m_granulatorGUI{nullptr}
{}

void DrawingArea::setGranulatorInterface(GranulatorInterface *interface) {
    m_granulatorGUI = interface;
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event) {
    if (m_granulatorGUI) {
        m_granulatorGUI->addPoint(event->pos());
    }
}

}
}
