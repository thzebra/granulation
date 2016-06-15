#pragma once
#include <QMainWindow>
#include "Granulator.hpp"
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QPoint>
#include <vector>
#include <QComboBox>
#include "DrawingArea.hpp"
#include "Envelope.hpp"
#include "Source.hpp"
#include <QSpinBox>
#include <QCheckBox>

namespace Granulation {
namespace Panel {

using namespace Synthesis;

class GranulatorInterface : public QMainWindow
{
    Q_OBJECT

public:
    GranulatorInterface(QWidget *parent = 0);
    ~GranulatorInterface();

    QPushButton* m_button{};
    QLabel* m_label{};
    QGridLayout* m_layout{};
    QWidget* m_central{};
    QComboBox* m_devices{};
    DrawingArea* m_drawingArea{};

    QSpinBox* m_grainCount{};
    QSpinBox* m_grainDuration{};

    GranulatorBase* granulator{};
    QCheckBox* m_mute{};
    QLabel* m_mutelabel{};

    QLabel* m_countlabel{};
    QLabel* m_durationlabel{};

    QLabel* m_densitylabel{};
    QSpinBox* m_density{};

    float pan();
    void setPan(float p);
    void addPoint(QPoint);

public slots:
    void updateLabel();
    void emptyPoints();
    void addPoint(QMouseEvent*);

private:
    std::vector<QPoint> m_points;
    const unsigned int m_maxpoints;
    float m_pan {0.f};
    int m_nthpoint;

    int m_maxgrains{1};
    int m_duration{10};
};

}
}
