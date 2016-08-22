#pragma once

#include <QGraphicsView>
#include "GrainDisplay.hpp"

namespace Granulation {
namespace Panel {

/**
 * @brief The GrainDisplayView class is a QGraphicsView used to show a GrainDisplay.
 */

class GrainDisplayView : public QGraphicsView {
    Q_OBJECT

public:
    explicit GrainDisplayView(QWidget* parent = 0);
    explicit GrainDisplayView(QGraphicsScene* scene, QWidget* parent = 0);

    /**
     * @brief Gets the GrainDisplay.
     * @return A reference to the GrainDisplay used by this View.
     */

    GrainDisplay& grainDisplay();

    /**
     * @brief Gets the GrainDisplay (const version).
     * @return A constant reference to the GrainDisplay used by this View.
     */

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
