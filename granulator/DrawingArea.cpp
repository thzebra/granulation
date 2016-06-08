#include "DrawingArea.hpp"
#include <QMouseEvent>
#include "GranulatorInterface.hpp"
#include <QDebug>

namespace Granulation {
namespace Panel {

DrawingArea::DrawingArea(QWidget *parent) :
    QWidget(parent)
{
    setStyleSheet(QString("QWidget { background-color: white; border: 1px black solid}"));
}

}
}
