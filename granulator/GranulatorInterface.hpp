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

/**
 * @brief The GranulatorInterface class is a QMainWindow that implements a
 * graphical interface for a Granulator.
 *
 * The GranulatorInterface is able to load an audio file for its Granulator to use
 * and to write audio samples produced by the Granulator to an output file. The
 * GranulatorInterface also manages a list of points that are captured from mouse
 * events (via the interface's DrawingArea) and can be used to control parameters
 * based on mouse trajectory, speed, etc.
 */

class GranulatorInterface : public QMainWindow
{
    Q_OBJECT

public:
    GranulatorInterface(QWidget *parent = 0);
    ~GranulatorInterface();

    /// These attributes are QWidgets used to display
    /// and modify the Granulator's parameters.

    QPushButton* m_capturebutton{};
    QLabel* m_label{};
    QGridLayout* m_layout{};
    QWidget* m_central{};
    QComboBox* m_devices{};
    DrawingArea* m_drawingArea{};

    QSpinBox* m_grainCount{};
    QSpinBox* m_grainDuration{};

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

    QPushButton* m_generateGrains{};

    QSpinBox* m_speed{};
    QLabel* m_speedlabel{};
    QSpinBox* m_pitch{};
    QLabel* m_pitchlabel{};

    /// The Granulator managed by the GranulatorInterface.

    GranulatorBase* granulator{};

    /**
     * @brief Get the pan.
     * @return The pan used by the Granulator (currently unused)
     */
    float pan();

    /**
     * @brief Sets the pan.
     * @param p: the pan, a float between -1 and 1.
     */
    void setPan(float p);

    /**
     * @brief Adds a point in the list of points.
     * @param qp: the point to add.
     */
    void addPoint(QPoint qp);

    /**
     * @brief Sets the source file for the sample.
     * @param src: A QString containing the relative path to the file.
     */
    void setSourceData(const QString& src);

    /**
     * @brief Gets the SourceData used by the Granulator.
     * @return A shared_ptr to the SourceData used by the Granulator.
     */
    std::shared_ptr<SourceData> sourceData() const;

    /**
     * @brief Writes raw audio to the output file.
     * @param ptr: An array of floats.
     * @param nframes: The amount of frames to write.
     */
    void write(const float* ptr, int nframes = 512);

public slots:
    /**
     * @brief Updates the label displaying the number of active grains.
     */
    void updateLabel();

    /**
     * @brief Clears the list of points.
     */
    void emptyPoints();

    /**
     * @brief Adds a point to the list of points.
     * @param me: A MouseEvent pointer. Its current position is the QPoint that
     * will be added to the list.
     */
    void addPoint(QMouseEvent* me);

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

    /**
     * @brief Opens the output file via the sndfile library.
     *
     * This function is used when toggleCapture(true) is called.
     */
    void openOutFile();

    /**
     * @brief Closes the output file via the sndfile library.
     *
     * This function is used when toggleCapture(false) is called.
     */
    void closeOutFile();

    /**
     * @brief Toggles between capturing generated audio and not capturing it.
     * @param b: True to start the capture, false to stop it.
     */
    void toggleCapture(bool b = false);
};

}
}
