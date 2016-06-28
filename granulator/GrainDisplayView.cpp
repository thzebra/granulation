#include "GrainDisplayView.hpp"

namespace Granulation {
namespace Panel {

GrainDisplayView::GrainDisplayView(QWidget *parent) :
    QGraphicsView(parent)
{
    m_scene.addItem(&m_graindisplay);
    this->setScene(&m_scene);
    setSceneRect(m_graindisplay.boundingRect());
}

GrainDisplayView::GrainDisplayView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent) {}

void GrainDisplayView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
}

void GrainDisplayView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
}

GrainDisplay& GrainDisplayView::grainDisplay() {
    return m_graindisplay;
}

const GrainDisplay& GrainDisplayView::grainDisplay() const {
    const GrainDisplay& gd = m_graindisplay;
    return gd;
}

void GrainDisplayView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    m_graindisplay.resize(event->size());
    setSceneRect(m_graindisplay.boundingRect());
}

}
}
