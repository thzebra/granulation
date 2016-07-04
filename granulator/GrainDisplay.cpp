#include "GrainDisplay.hpp"
#include "SourceData.hpp"
#include <QPainter>
#include <cmath>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QScrollBar>
#include <Grain.hpp>
#include <QGraphicsSceneDragDropEvent>

namespace Granulation {
namespace Panel {

GrainDisplay::GrainDisplay(QGraphicsItem* parent) :
    QGraphicsObject(parent),
    m_zoom{1.f}
{
    setHeight(600);
}

QRectF GrainDisplay::boundingRect() const {
    return {0, 0, m_width, m_height};
}

void GrainDisplay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    paint_impl(painter);
}

void GrainDisplay::setData(const std::shared_ptr<Synthesis::SourceData> data) {
    prepareGeometryChange();
    m_data.clear();

    m_lock.lock();
    m_grains.clear();
    m_grainsPaths.clear();
    m_lock.unlock();

    m_density = -1;
    if (data != nullptr) {
        const Synthesis::SourceData& srcdata = *data;
        m_sampleRate = srcdata.sampleRate();
        int nchannels = srcdata.channels();
        for (int i = 0; i < nchannels; ++i)
            m_data.push_back(std::vector<float> ());
        const unsigned int s = srcdata.size() / nchannels;
        for (unsigned int i = 0; i < s; ++i) {
            for (unsigned int j = 0; j < nchannels; ++j) {
                m_data[j].push_back(srcdata.data(i * nchannels + j));
            }
        }

        auto sc = scene();
        if (sc) {
            auto view = sc->views().first();
            if (view) {
                setWidth(view->width());
            }
        }

        recompute(0, m_zoom);
    }
}

void GrainDisplay::recompute(const double& durms, double zoom) {
    m_zoom = zoom;
    m_paths = QList<QPainterPath> ();
    m_channels = QPainterPath{};
    m_grainsPaths = QList<QPainterPath> {};

    if(m_data.size() == 0)
        return;

    const int density = std::max((int)(zoom * m_sampleRate / 1000), 1);
    long action = compareDensity(density);

    switch (action) {
    case KEEP_CUR:
        break;
    case USE_NEXT:
        m_prevdata = std::vector<std::vector<double> > (m_curdata);
        m_prevdensity = m_density;
        m_curdata = std::vector<std::vector<double> > (m_nextdata);
        m_density = m_nextdensity;
        if (density > 1)
            m_nextdata = computeDataSet(zoom / 2., m_nextdensity);
        break;
    case USE_PREV:
        m_nextdata = std::vector<std::vector<double> > (m_curdata);
        m_nextdensity = m_density;
        m_curdata = std::vector<std::vector<double> > (m_prevdata);
        m_density = m_prevdensity;
        m_prevdata = computeDataSet(2. * zoom, m_prevdensity);
        break;
    case RECOMPUTE_ALL:
        m_curdata = computeDataSet(zoom, m_density);
        m_prevdata = computeDataSet(2. * zoom, m_prevdensity);
        m_nextdata = computeDataSet(zoom / 2., m_nextdensity);
        break;
    default:
        break;
    }

    drawWaveForms(zoom);
    drawGrains(zoom);

    update();
}

void GrainDisplay::paint_impl(QPainter* painter) const {
    const int nchannels = m_data.size();
    if (nchannels == 0)
        return;

    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);

    for (const auto& path : m_paths)
        painter->drawPath(path);

    painter->save();

    const auto h = -height() / nchannels;
    const auto dblh = 2. * h;

    painter->scale(1, -1);
    painter->translate(0, h);

    for (const auto& path : m_paths) {
        painter->drawPath(path);
        painter->translate(0., dblh);
    }

    painter->restore();

    // Paint channel limits

    painter->setPen(Qt::lightGray);
    painter->drawPath(m_channels);

    // Paint grains

    painter->setPen(Qt::blue);
    painter->setBrush(QColor(0, 255, 255, 100));

    painter->save();

    //qDebug() << "painting" << m_grainsPaths.size() << "grains";

    m_lock.lock();
    for (int c = 0; c < nchannels; ++c) {
        for (const auto& path: m_grainsPaths) {
            painter->drawPath(path);
        }
        painter->translate(0, -h);
    }
    m_lock.unlock();

    painter->restore();
    painter->save();

    painter->setPen(Qt::red);

    m_lock.lock();
    for (int c = 0; c < nchannels; ++c) {
        for (const auto& path: m_indexPaths) {
            painter->drawPath(path);
        }
        painter->translate(0, -h);
    }
    m_lock.unlock();

    painter->restore();
    painter->save();

    painter->setPen(Qt::green);
    painter->setBrush(QColor(0, 255, 0, 100));

    for (int c = 0; c < nchannels; ++c) {
        painter->drawRect(m_newGrain);
        painter->translate(0, -h);
    }
    painter->restore();

}

void GrainDisplay::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    qreal xpos = event->pos().x();
    m_newGrain = QRectF(xpos, 0, 1, height() / m_data.size());
    m_grainbegin = xpos * m_data[0].size() / width();
    //qDebug() << "source has" << m_data[0].size() << "samples";
    emit pressed(event);
}

void GrainDisplay::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    emit released(event);
    int length = m_newGrain.width() * m_data[0].size() / width();
    //qDebug() << "emitting grainadded firstSample at" << m_grainbegin << "length in samples" << length << "aka in pixels" << m_newGrain.width();
    emit grainadded(m_grainbegin, length);

    m_newGrain = QRectF();
}

void GrainDisplay::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    m_newGrain.setWidth(std::max(qreal(0), event->pos().x() - m_newGrain.left()));
    emit moved(event);
}

long GrainDisplay::compareDensity(const double density) {
    if (m_density == -1 || m_density >= 4 * density || 4 * m_density <= density || (int)width() == 0 || m_curdata.empty()) {
        return RECOMPUTE_ALL;
    }
    if (m_density >= 2 * density) {
        return USE_NEXT;
    }
    if (2 * m_density <= density) {
        return USE_PREV;
    }
    return KEEP_CUR;
}

std::vector<std::vector<double> > GrainDisplay::computeDataSet(double zoom, double& density) {
    const int nchannels = m_data.size();

    density = std::max((m_sampleRate * zoom) / 1000., 1.);
    const int density_i = (int) density;

    std::vector<std::vector<double> > dataset;
    dataset.resize(nchannels);
    for (int c = 0; c < nchannels; ++c) {

        const auto& chan = m_data[c];
        const auto chan_n = chan.size();

        const double length = (1000 * chan_n) / m_sampleRate; // duration of the track
        const double size = zoom > 0 ? length / zoom : 0; // number of pixels the track will occupy in its entirety
        const int npoints = size;

        std::vector<double>& rmsv = dataset[c];
        rmsv.reserve(npoints);

        for (int i = 0; i < npoints; ++i)
        {
            double rms = 0;
            for (int j = 0;
                 (j < density_i) && ((i * density_i + j) < chan_n);
                 ++j)
            {
                auto s = chan[i * density_i + j];
                rms += s * s;
            }
            rmsv.push_back(std::sqrt(rms / density));
        }
    }
    return dataset;
}

void GrainDisplay::drawWaveForms(double zoom) {
    const double density = std::max((zoom * m_sampleRate) / 1000., 1.);
    const double densityratio = (m_density > 0 && density > 0) ? m_density / density : 1.;

    int nchannels = m_data.size();
    if (nchannels == 0)
        return;

    // Height of each channel
    const auto h = height() / (double)nchannels;
    const int w = width();

    // Trace lines between channels

    for (int c = 1 ; c < nchannels; ++c) {
        m_channels.moveTo(0, c * h);
        m_channels.lineTo(w, c * h);
    }

    // Get horizontal offset

    auto view = scene()->views().first();

//    auto x0 = mapFromScene(view->mapToScene(0, 0)).x();
    auto x0 = 0;
//    int i0 = x0 / densityratio;
    int i0 = 0;
    const int n = m_curdata[0].size();

//    qDebug() << "graphics: first sample is" << i0 << "at" << x0;

    //auto xf = mapFromScene(view->mapToScene(view->width(), 0)).x();

   for (int c = 0; c < nchannels ; ++c) {
        const int current_height = c * h;
        std::vector<double> dataset = m_curdata[c];

        QPainterPath path{};
        path.setFillRule(Qt::WindingFill);

        // Draw path for current channel

        auto height_adjustemnt = current_height + h / 2.;
        if (n > i0) {
            path.moveTo(x0, dataset[i0] + height_adjustemnt);
            double x = x0;
            for (int i = i0; (i < n) && (x <= w); ++i) {
                x = i * densityratio;
                path.lineTo(x, dataset[i] * h / 2. + height_adjustemnt);
            }
            path.lineTo(x, height_adjustemnt);
        }
        m_paths.push_back(path);
   }
}

void GrainDisplay::scrollValueChanged(int scrollval) {
    prepareGeometryChange();
    recompute(0, m_zoom);
}


void GrainDisplay::printAction(long action) {
    switch (action) {
    case KEEP_CUR:
        qDebug() << "keep current";
        break;
    case USE_NEXT :
        qDebug() << "use next";
        break;
    case USE_PREV :
        qDebug() << "use prev";
        break;
    case RECOMPUTE_ALL :
        qDebug() << "recompute all";
        break;
    default :
        qDebug() << "unknown action";
    }
}

void GrainDisplay::setHeight(qreal height)
{
    prepareGeometryChange();
    m_height = height;
    emit heightChanged();
    recompute(0, m_zoom);
}


qreal GrainDisplay::height() const
{
    return m_height;
}


void GrainDisplay::setWidth(qreal width)
{
    prepareGeometryChange();
    m_width = width;
    m_zoom = 1.f;
    if (!m_data.empty() && m_width > 0) {
        int nsamples = m_data[0].size();
        if (nsamples > 0) {
            m_zoom = nsamples / ((m_sampleRate / 1000.f) * m_width);
        }
    }
    emit widthChanged();
    recompute(0, m_zoom);
}


qreal GrainDisplay::width() const
{
    return m_width;
}

void GrainDisplay::addGrain(const Synthesis::Grain& grain) {
    GrainItem item = {int64_t(grain.beginning()), grain.size(), grain.currentIndex(), false};
    m_grains.push_back(item);
    drawGrain(item, m_zoom);
}

void GrainDisplay::setGrains(std::deque<Synthesis::Grain> grains) {
    m_lock.lock();
    m_grains.clear();
    m_grainsPaths.clear();
    m_indexPaths.clear();
    for (auto grain: grains) {
        addGrain(grain);
    }
    m_lock.unlock();
    update();
}

void GrainDisplay::advanceIndices(int nsamples) {
    for (GrainItem& item: m_grains) {
        if (!item.ended) {
            item.index += nsamples;
            if (item.index >= item.length) {
                if (m_loop)
                    item.index %= item.length;
                else {
                    item.index = 2 * item.length;
                    item.ended = true;
                }
            }
        }
    }
}

void GrainDisplay::drawGrain(const GrainItem &item, double zoom) {
    if (!item.ended) {
        const double density = std::max((zoom * m_sampleRate) / 1000., 1.);
        const double densityratio = (m_density > 0 && density > 0) ? m_density / density : 1.;

        int64_t nchannels = m_data.size();
        if (nchannels == 0)
            return;

        const int64_t h = height() / (double)nchannels;
        const int w = width();
        int64_t topVal = 0;
        int64_t approxLeft = item.firstSample * 1000 / (zoom * m_sampleRate);
        int64_t index = (item.index + item.firstSample) * 1000 / (zoom * m_sampleRate);

        QPainterPath path{};
        QRectF rect = QRectF(approxLeft, topVal, item.length / density, h);
        path.addRect(rect);
        m_grainsPaths.append(path);

        QPainterPath idxpath{};
        idxpath.moveTo(index, topVal);
        idxpath.lineTo(index, topVal + h);
        m_indexPaths.append(idxpath);
    }
}

void GrainDisplay::drawGrains(double zoom) {
    m_indexPaths.clear();
    m_grainsPaths.clear();
    for (GrainItem& item: m_grains) {
        drawGrain(item, zoom);
    }
}

QString GrainItem::toQString() const {
    QString str = QString("First sample: ") + QString::number(firstSample) +
            QString("; length: ") + QString::number(length) +
            QString("; index: ") + QString::number(index);
    return str;
}

void GrainDisplay::resize(const qreal &w, const qreal &h) {
    setHeight(h);
    setWidth(w);
}

void GrainDisplay::resize(const QSize &size) {
    resize(size.width(), size.height());
}

void GrainDisplay::setLoop(bool looping) {
    m_loop = looping;
}

}
}

