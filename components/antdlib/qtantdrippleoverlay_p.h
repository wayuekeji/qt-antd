#pragma once

#include <QWidget>
#include <QColor>

namespace QtAntdInternal {

// Global expansion distance (px) the outer ripple expands beyond the control
constexpr int kAntdWaveExpandDistance = 6;

// Show an outer ripple overlay around an inner rect of a widget.
// innerRectInOrigin: rect in the coordinate space of originWidget.
// cornerRadius: corner radius for rounded-rect mode; ignored if isCircle=true.
// isCircle: if true, render circle ring; otherwise rounded-rect ring.
// color: ring color; alpha is controlled by animation.
// extraPixels: how much the ring expands beyond inner rect at max.
// durationMs: animation duration.
void StartOuterRippleOverlay(QWidget* originWidget,
                             const QRect& innerRectInOrigin,
                             int cornerRadius,
                             bool isCircle,
                             const QColor& color,
                             int extraPixels,
                             int durationMs = 450);

} // namespace QtAntdInternal
