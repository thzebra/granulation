#include "DrawingArea.hpp"
#include <QMouseEvent>
#include "GranulatorInterface.hpp"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QScrollBar>

namespace Granulation {
namespace Panel {

DrawingArea::DrawingArea(QWidget *parent) :
    QWidget(parent)
{
    QPalette p;
    p.setColor(QPalette::Background, Qt::white);
    setAutoFillBackground(true);
    setPalette(p);
}

}
}
