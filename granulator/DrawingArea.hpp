#pragma once

#include <QWidget>
#include <QRect>
#include <QGraphicsItem>
#include <memory>
#include "SourceData.hpp"
#include <vector>
#include <QGraphicsView>

class QMouseEvent;

namespace Granulation {
namespace Panel {

class DrawingArea final : public QWidget {
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = 0);

signals:
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
};

}
}
