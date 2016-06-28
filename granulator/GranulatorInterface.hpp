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
#include <QLineEdit>
#include <memory>
#include <sndfile.hh>
#include "GrainDisplayView.hpp"

namespace Granulation {
namespace Panel {

using namespace Synthesis;

class GranulatorInterface : public QMainWindow
{
    Q_OBJECT

public:
    GranulatorInterface(QWidget *parent = 0);
    ~GranulatorInterface();

    QPushButton* m_capturebutton{};
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

    QPushButton* m_cleargrains{};

    QLineEdit* m_sourcefilename{};
    QLineEdit* m_outfilename{};

    QCheckBox* m_loop{};
    QLabel* m_looplabel{};

    GrainDisplayView* m_graindisplayview{};


    float pan();
    void setPan(float p);
    void addPoint(QPoint);
    void setSourceData(const QString& src);
    std::shared_ptr<SourceData> sourceData() const;
    void write(const float* ptr, int nframes = 512);

public slots:
    void updateLabel();
    void emptyPoints();
    void addPoint(QMouseEvent*);

private:
    std::vector<QPoint> m_points;
    //const unsigned int m_maxpoints;
    float m_pan {0.f};
    int m_nthpoint;
    std::shared_ptr<SourceData> m_sourcedata;

    int m_maxgrains{10};
    int m_duration{500};

    SNDFILE* m_outfile{};
    SF_INFO m_outinfo{};

    void openOutFile();
    void closeOutFile();
    void toggleCapture(bool b = false);
};

}
}
