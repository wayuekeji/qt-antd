#include "qtantdrippleoverlay_p.h"

#include <QPainter>
#include <QPainterPath>
#include <QElapsedTimer>
#include <QTimer>
#include <QtGlobal>
#include <cmath>

namespace QtAntdInternal {

class RippleOverlayWidget : public QWidget {
public:
    RippleOverlayWidget(QWidget* parent,
                        const QRect& innerRectInOverlay,
                        int cornerRadius,
                        bool isCircle,
                        const QColor& color,
                        int extraPixels,
                        int durationMs)
        : QWidget(parent)
        , m_innerRect(innerRectInOverlay)
        , m_cornerRadius(cornerRadius)
        , m_isCircle(isCircle)
        , m_color(color)
        , m_extra(extraPixels)
        , m_duration(durationMs)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        setAttribute(Qt::WA_NoSystemBackground, true);
        setAttribute(Qt::WA_TranslucentBackground, true);
        m_timer = new QTimer(this);
        QObject::connect(m_timer, &QTimer::timeout, this, [this]() {
            if (m_clock.isValid()) {
                const int elapsed = int(m_clock.elapsed());
                if (elapsed >= m_duration) {
                    m_timer->stop();
                    this->deleteLater();
                    return;
                }
            }
            update();
        });
    }

    void start() {
        m_clock.start();
        m_timer->start(16); // ~60 FPS
        show();
        raise();
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);

        const qreal t = m_clock.isValid() ? qBound<qreal>(0.0, m_clock.elapsed() / qreal(m_duration), 1.0) : 0.0;
        const qreal ease = 1.0 - std::pow(1.0 - t, 3.0);
        const qreal grow = m_extra * ease;
        const qreal alpha = (1.0 - t) * 0.35;

        QRectF outer = m_innerRect.adjusted(-grow, -grow, grow, grow);
        const qreal outerRadius = m_isCircle ? outer.height() / 2.0 : m_cornerRadius + grow;

        QPainterPath innerPath;
        if (m_isCircle) {
            innerPath.addEllipse(m_innerRect);
        } else {
            innerPath.addRoundedRect(m_innerRect, m_cornerRadius, m_cornerRadius);
        }

        QPainterPath outerPath;
        if (m_isCircle) {
            outerPath.addEllipse(outer);
        } else {
            outerPath.addRoundedRect(outer, outerRadius, outerRadius);
        }

        QPainterPath ring = outerPath.subtracted(innerPath);
        QColor c = m_color;
        c.setAlphaF(alpha);
        p.fillPath(ring, c);
    }

private:
    QRectF m_innerRect;
    int m_cornerRadius;
    bool m_isCircle;
    QColor m_color;
    int m_extra;
    int m_duration;
    QElapsedTimer m_clock;
    QTimer* m_timer { nullptr };
};

void StartOuterRippleOverlay(QWidget* originWidget,
                             const QRect& innerRectInOrigin,
                             int cornerRadius,
                             bool isCircle,
                             const QColor& color,
                             int extraPixels,
                             int durationMs)
{
    if (!originWidget)
        return;

    QWidget* win = originWidget->window();
    if (!win)
        return;

    const QPoint topLeftInWin = originWidget->mapTo(win, innerRectInOrigin.topLeft());
    const QRect innerInWin(topLeftInWin, innerRectInOrigin.size());

    const QRect overlayRect = innerInWin.adjusted(-extraPixels, -extraPixels, extraPixels, extraPixels);
    const QRect innerInOverlay(innerInWin.topLeft() - overlayRect.topLeft(), innerInWin.size());

    auto overlay = new RippleOverlayWidget(win,
                                           innerInOverlay,
                                           cornerRadius,
                                           isCircle,
                                           color,
                                           extraPixels,
                                           durationMs);
    overlay->setGeometry(overlayRect);
    overlay->start();
}

} // namespace QtAntdInternal
