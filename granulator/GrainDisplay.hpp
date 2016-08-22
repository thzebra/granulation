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


/**
 * @brief The GrainDisplay class is used to display the waveform of the current sound sample
 * and grains using this sample, including the position of the current sample for each grain.
 */

class GrainDisplay final : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit GrainDisplay(QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * @brief Sets the SourceData whose waveform is to be drawn.
     * @param data: a shared pointer to the SourceData
     */

    void setData(const std::shared_ptr<Synthesis::SourceData> data);

    /**
     * @brief Recomputes all cached data.
     * @param durms: the duration of the sample in ms (unused)
     * @param zoom: the zoom factor, in samples per pixel
     */

    void recompute(const double& durms, double zoom);

    /**
     * @brief Sets the height of the widget
     * @param height: the new height
     */

    void setHeight(qreal height);

    /**
     * @brief Gets the height of the widget
     * @return the height of the widget
     */

    qreal height() const;

    /**
     * @brief Sets the width of the widget
     * @param width: the new width
     */

    void setWidth(qreal width);

    /**
     * @brief Gets the width of the widget
     * @return  the width of the widget
     */

    qreal width() const;

    /**
     * @brief Resizes the widget
     * @param w: the new width
     * @param h: the new height
     */

    void resize(const qreal& w, const qreal& h);

    /**
     * @brief Resizes the widget
     * @param size: the new size
     */

    void resize(const QSize& size);

    /**
     * @brief Sets all grains to draw and redraws them.
     * @param grains: an std::deque of Grains
     */

    void setGrains(const std::deque<Synthesis::Grain>& grains);

    /**
     * @brief Add a Grain to the list of Grains and draws it.
     * @param grain: a grain
     */

    void addGrain(const Synthesis::Grain &grain);

    /**
     * @brief Updates the reading mark of all active Grains.
     * @param nsamples: the number of samples the mark should be forwarded.
     */

    void advanceIndices(int nsamples);

    /**
     * @brief Set the "looping" parameter for a Grain.
     * @param looping: true if the Grain is to loop, false otherwise.
     */

    void setLoop(bool looping);

signals:
    void pressed(QGraphicsSceneMouseEvent* event);
    void moved(QGraphicsSceneMouseEvent* event);
    void released(QGraphicsSceneMouseEvent* event);
    void grainadded(int firstSample, int length);

public slots:

    /**
      * @brief Updates the view when it is scrolled.
      * @param scrollval: the number of pixels that were scrolled
      */

     void scrollValueChanged(int scrollval);

private:

     /**
     * @brief Does the actual painting of elements.
     */
    void paint_impl(QPainter*) const;

    /**
     * @brief Returns what to do depending on current density and stored density
     * @param density: the current density
     * @return An action (enum): whether to use current caches, another cached
     * density, or recalculate everything.
     */
    long compareDensity(const double density);

    /**
     * @brief Computes a data set for the given ZoomRatio
     * @param zoom: the current zoom ratio
     * @param density: the current density
     * @return
     */
    std::vector<std::vector<double> > computeDataSet(double zoom, double& density);

    /**
     * @brief Draws the waveforms into the QPainterPaths
     * @param zoom: the current zoom
     */

    void drawWaveForms(double zoom);

    /**
     * @brief Draws grains into the grains QPainterPaths.
     * @param zoom: the current zoom
     */

    void drawGrains(double zoom);

    /**
     * @brief Draw a grain into the grains QPainterPaths.
     * @param item: the grain to draw
     * @param zoom: the current zoom
     */

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
