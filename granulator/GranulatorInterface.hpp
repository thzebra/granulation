#pragma once
#include <QMainWindow>
#include "Granulator.hpp"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPoint>
#include <vector>
#include <QComboBox>
#include "DrawingArea.hpp"
#include "Envelope.hpp"
#include "Source.hpp"

namespace Granulation {
namespace Panel {

using namespace Synthesis;

class GranulatorInterface : public QMainWindow
{
    Q_OBJECT

public:
    GranulatorInterface(QWidget *parent = 0);
    ~GranulatorInterface();

    void addPoint(QPoint);

    QPushButton* m_button{};
    QLabel* m_label{};
    QVBoxLayout* m_layout{};
    QWidget* m_central{};
    QComboBox* m_devices{};
    DrawingArea* m_drawingArea{};

    Granulator* granulator;

public slots:
    void updateLabel();

private:
    std::vector<QPoint> m_points;
    const unsigned int m_maxpoints;
};

}
}
