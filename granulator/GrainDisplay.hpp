#pragma once
#include <QGraphicsItem>
#include <memory>
#include <QMouseEvent>
#include <deque>
#include <QMutex>

namespace Granulation {
namespace Synthesis {
class SourceData;
class Grain;
}

namespace Panel {

struct GrainItem {
    int64_t firstSample; // index of first sample in the base sound
    unsigned int length; // number of samples (in one channel)
    int index; // local index of current sample
    QString toQString() const;
    bool ended;
};


class GrainDisplay final : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit GrainDisplay(QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setData(const std::shared_ptr<Synthesis::SourceData> data);
    void recompute(const double& durms, double zoom);

    void setHeight(qreal height);
    qreal height() const;

    void setWidth(qreal width);
    qreal width() const;

    void resize(const qreal& w, const qreal& h);
    void resize(const QSize& size);

    void setGrains(std::deque<Synthesis::Grain> grains);
    void addGrain(const Synthesis::Grain &grain);

    void advanceIndices(int nsamples);

    void setLoop(bool looping);

signals:
    void pressed(QGraphicsSceneMouseEvent* event);
    void moved(QGraphicsSceneMouseEvent* event);
    void released(QGraphicsSceneMouseEvent* event);
    void grainadded(int firstSample, int length);

public slots:
     void scrollValueChanged(int scrollval);

private:
    void paint_impl(QPainter*) const;

    // Returns what to do depending on current density and stored density
    long compareDensity(const double);

    // Computes a data set for the given ZoomRatio
    std::vector<std::vector<double> > computeDataSet(double zoom, double& density);

    void drawWaveForms(double zoom);
    void drawGrains(double zoom);
    void drawGrain(const GrainItem& item, double zoom);

    std::vector<std::vector<float>> m_data{};
    QList<QPainterPath> m_paths;
    QPainterPath m_channels{};
    int m_sampleRate;

    double m_prevdensity = -1;
    double m_density = -1;
    double m_nextdensity = -1;

    double m_zoom;

    qreal m_width{0};
    qreal m_height{0};

    std::vector<std::vector<double>> m_prevdata;
    std::vector<std::vector<double>> m_curdata;
    std::vector<std::vector<double>> m_nextdata;

    enum {KEEP_CUR = 0, USE_PREV, USE_NEXT, RECOMPUTE_ALL};
    void printAction(long);

    std::deque<GrainItem> m_grains;
    QList<QPainterPath> m_grainsPaths;
    QList<QPainterPath> m_indexPaths;

    bool m_loop{false};

    QPainterPath m_newGrainPath;
    QRectF m_newGrain;
    int m_grainbegin{0};

    mutable QMutex m_lock;

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

}
}
