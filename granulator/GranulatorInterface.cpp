#include "GranulatorInterface.hpp"
#include <QDebug>
#include <rtaudio/RtAudio.h>
#include <exception>
#include <QMouseEvent>
#include "FileSourceData.hpp"

namespace Granulation {
namespace Panel {

GranulatorInterface::GranulatorInterface(QWidget *parent)
    : QMainWindow(parent),
      m_central{new QWidget},
      m_capturebutton{new QPushButton(tr("Capture"))},
      m_label{new QLabel},
      m_layout{new QGridLayout},
      m_devices{new QComboBox},
      m_drawingArea{new DrawingArea},
      m_grainDuration{new QSpinBox},
      m_grainCount{new QSpinBox},
      m_mute{new QCheckBox},
      m_mutelabel{new QLabel},
      m_durationlabel{new QLabel},
      m_countlabel{new QLabel},
      m_density{new QSpinBox},
      m_densitylabel{new QLabel},
      m_cleargrains{new QPushButton(tr("Clear grains"))},
      m_sourcefilename{new QLineEdit},
      m_outfilename{new QLineEdit},
      m_loop{new QCheckBox},
      m_looplabel{new QLabel},
      m_graindisplayview{new GrainDisplayView(this)}
{
    resize(600, 600);

    m_capturebutton->setCheckable(true);

    m_grainCount->setMinimum(1);
    m_grainCount->setMaximum(1000);
    m_grainCount->setValue(10);

    m_grainDuration->setMinimum(10);
    m_grainDuration->setMaximum(10000);
    m_grainDuration->setValue(500);

    m_grainDuration->setSuffix(tr(" ms"));
    m_mute->setCheckState(Qt::Unchecked);
    m_mutelabel->setText(tr("Mute"));
    m_countlabel->setText(tr("Max grains"));
    m_durationlabel->setText(tr("Grain duration"));

    m_density->setMinimum(1);
    m_density->setMaximum(500);
    m_density->setValue(100);
    m_densitylabel->setText(tr("Number of grains per second"));

    m_label->setText("No active grains");
    m_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    m_loop->setChecked(false);
    m_looplabel->setText(tr("Loop grains"));

    QString firstSample = tr("sample.wav");
    m_sourcefilename->setText(firstSample);

    FileSourceData* fsd = new FileSourceData();
    m_sourcedata = std::make_shared<FileSourceData> (*fsd);
    setSourceData(m_sourcefilename->text());
    m_graindisplayview->grainDisplay().setData(m_sourcedata);

    m_outfilename->setText(tr("out.wav"));

    m_layout->addWidget(m_drawingArea, 0, 0, 1, 1);
    m_layout->addWidget(m_graindisplayview, 0, 1, 1, 1);
    m_layout->addWidget(m_label, 1, 0);
    m_layout->addWidget(m_devices, 2, 0, 1, 2);
    m_layout->addWidget(m_countlabel, 3, 0);
    m_layout->addWidget(m_grainCount, 3, 1);
    m_layout->addWidget(m_durationlabel, 4, 0);
    m_layout->addWidget(m_grainDuration, 4, 1);
    m_layout->addWidget(m_densitylabel, 5, 0);
    m_layout->addWidget(m_density, 5, 1);
    m_layout->addWidget(m_mutelabel, 6, 0);
    m_layout->addWidget(m_mute, 6, 1);
    m_layout->addWidget(m_looplabel, 7, 0);
    m_layout->addWidget(m_loop, 7, 1);
    m_layout->addWidget(m_capturebutton, 8, 0);
    m_layout->addWidget(m_cleargrains, 8, 1);
    m_layout->addWidget(m_sourcefilename, 9, 0);
    m_layout->addWidget(m_outfilename, 9, 1);

    m_central->setLayout(m_layout);
    this->setCentralWidget(m_central);

    connect(m_grainDuration,
            static_cast<void (QSpinBox::*)(int)> (&QSpinBox::valueChanged),
            [=] (int d) mutable -> void { granulator->setEssenceDuration(d); });
    connect(m_grainCount, static_cast<void (QSpinBox::*)(int)> (&QSpinBox::valueChanged),
            [=] (int c) mutable -> void { granulator->setMaxGrains(c); });
    connect(m_cleargrains,
            &QPushButton::clicked,
            [=] () mutable -> void { granulator->clearGrains(); m_label->setText(tr("No active grains"));});
    connect(m_sourcefilename,
            &QLineEdit::editingFinished,
            [=] () mutable -> void { setSourceData(m_sourcefilename->text());
                                     m_graindisplayview->grainDisplay().setData(m_sourcedata);});
    connect(m_capturebutton,
            &QPushButton::clicked,
            [=] (bool checked) mutable -> void { toggleCapture(checked); });
    connect(m_drawingArea,
            &DrawingArea::mouseMoveEvent,
            [=] (QMouseEvent* event) mutable -> void { addPoint(event); });
    connect(m_loop,
            &QCheckBox::stateChanged,
            [=] (bool isChecked) mutable -> void { granulator->setLoop(isChecked); });
}

GranulatorInterface::~GranulatorInterface()
{
    delete m_devices;
    delete m_capturebutton;
    delete m_label;
    delete m_layout;
    delete m_central;
    delete granulator;
    delete m_mute;
    delete m_mutelabel;
    delete m_drawingArea;
    delete m_grainCount;
    delete m_grainDuration;
    delete m_density;
    delete m_densitylabel;
    delete m_countlabel;
    delete m_durationlabel;
}

void GranulatorInterface::addPoint(QPoint p) {
    m_points.push_back(p);
    setPan((2 * float(p.x()) / float(m_central->height())) - 1.f);
    granulator->generate(1);
    auto g = granulator->lastGrainAdded();
    m_graindisplayview->grainDisplay().addGrain(g);
}

void GranulatorInterface::addPoint(QMouseEvent *m) {
//    addPoint(m->pos());
//    int nactive = granulator->grainCount();
//    m_label->setText(QString::number(nactive) + tr(" active grains"));
    if (m_nthpoint == 0)
        addPoint(m->pos());
    m_nthpoint = (m_nthpoint + 1) % 50;
    int nactive = granulator->grainCount();
    m_label->setText(QString::number(nactive) + tr(" active grains"));
}

void GranulatorInterface::updateLabel() {}

float GranulatorInterface::pan() {
    return m_pan;
}

void GranulatorInterface::setPan(float p) {
    if (p < -1.f) {
        m_pan = -1.f;
        return;
    }
    if (p > 1.f) {
        m_pan = 1.f;
        return;
    }
    m_pan = p;
}

void GranulatorInterface::emptyPoints() {
    m_points.clear();
}

void GranulatorInterface::setSourceData(const QString& src) {
    qDebug() << src;
    m_sourcedata->setSource(src.toStdString());
    if (granulator)
        granulator->setEssenceData(m_sourcedata);
}

std::shared_ptr<SourceData> GranulatorInterface::sourceData() const {
    return m_sourcedata;
}

void GranulatorInterface::openOutFile() {
    m_outinfo.channels = granulator->channels();
    m_outinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    m_outinfo.samplerate = granulator->sampleRate();
    std::string outstr = m_outfilename->text().toStdString();
    if (outstr.compare(std::string ()))
        outstr = "out.wav";
    m_outfile = sf_open(outstr.c_str(), SFM_WRITE, &m_outinfo);
}

void GranulatorInterface::closeOutFile() {
    if (m_outfile)
        sf_close(m_outfile);
}

void GranulatorInterface::write(const float* ptr, int nframes) {
    sf_writef_float(m_outfile, ptr, nframes);
}

void GranulatorInterface::toggleCapture(bool b) {
    if (b)
        openOutFile();
    else
        closeOutFile();
}

}
}
