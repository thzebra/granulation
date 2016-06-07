#pragma once

#include <QWidget>

class QMouseEvent;

namespace Granulation {
namespace Panel {

class GranulatorInterface;

class DrawingArea : public QWidget {
    Q_OBJECT
public:
    explicit DrawingArea(QWidget *parent = 0);
    void setGranulatorInterface(GranulatorInterface* interface);

signals:

public slots:

protected:
    virtual void mouseMoveEvent(QMouseEvent* event);

private:
    GranulatorInterface* m_granulatorGUI{};
};

}
}
