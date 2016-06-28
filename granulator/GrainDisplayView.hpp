#pragma once

#include <QGraphicsView>
#include "GrainDisplay.hpp"

namespace Granulation {
namespace Panel {

class GrainDisplayView : public QGraphicsView {
    Q_OBJECT

public:
    explicit GrainDisplayView(QWidget* parent = 0);
    explicit GrainDisplayView(QGraphicsScene* scene, QWidget* parent = 0);

    GrainDisplay& grainDisplay();
    const GrainDisplay& grainDisplay() const;

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event) override;
    QGraphicsScene m_scene;
    GrainDisplay m_graindisplay;

};

}
}
