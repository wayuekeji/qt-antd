#ifndef QTANTDSWITCH_P_H
#define QTANTDSWITCH_P_H

#include <QtGlobal>
#include <QColor>
#include <QTimer>
#include <QPropertyAnimation>
#include <QIcon>
#include <QString>

class QtAntdSwitch;

class QtAntdSwitchPrivate
{
    Q_DISABLE_COPY(QtAntdSwitchPrivate)
    Q_DECLARE_PUBLIC(QtAntdSwitch)

public:
    QtAntdSwitchPrivate(QtAntdSwitch *q);
    ~QtAntdSwitchPrivate();

    void init();
    void updateGeometry();

    // Drawing helpers
    QColor getTrackColor() const;
    QColor getHandleColor() const;
    QColor getTextColor() const;
    QRect getTrackRect() const;
    QRect getHandleRect() const;
    QRect getTextRect() const;
    int getTrackWidth() const;
    int getTrackHeight() const;
    int getHandleSize() const;
    int getHandlePadding() const;

    // Animation
    void startToggleAnimation();
    void updateAnimationValue();
    void startLoadingAnimation();
    void stopLoadingAnimation();
    void drawLoadingSpinner(QPainter *painter, const QRect &rect, const QColor &color);

    QtAntdSwitch *const q_ptr;

    // Properties
    QtAntdSwitch::SwitchSize switchSize;
    bool useThemeColors;
    bool isLoading;
    bool isHovered;
    bool isPressed;
    bool hasFocus;

    // Content
    QString checkedText;
    QString uncheckedText;
    QIcon checkedIcon;
    QIcon uncheckedIcon;

    // Animation
    QPropertyAnimation *toggleAnimation;
    qreal animationValue; // 0.0 = off, 1.0 = on
    QTimer *loadingTimer;
    int loadingAngle;

    // Cached dimensions based on size
    int trackWidth;
    int trackHeight;
    int handleSize;
    int handlePadding;
};

#endif // QTANTDSWITCH_P_H