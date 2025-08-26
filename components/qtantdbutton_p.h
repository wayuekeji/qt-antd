#ifndef QTANTDBUTTON_P_H
#define QTANTDBUTTON_P_H

#include <QtGlobal>
#include <QColor>
#include <QTimer>
#include <QPropertyAnimation>

class QtAntdButton;

class QtAntdButtonPrivate
{
    Q_DISABLE_COPY(QtAntdButtonPrivate)
    Q_DECLARE_PUBLIC(QtAntdButton)

public:
    QtAntdButtonPrivate(QtAntdButton *q);
    ~QtAntdButtonPrivate();

    void init();
    void updateGeometry();

    int getSpinnerSize() const;
    void startLoadingAnimation();
    void stopLoadingAnimation();
    void drawLoadingSpinner(QPainter *painter, const QRect &rect, const QColor &color);

    QtAntdButton *const q_ptr;

    QtAntdButton::ButtonType buttonType;
    QtAntdButton::ButtonSize buttonSize;
    QtAntdButton::ButtonShape buttonShape;
    QtAntdButton::ColorPreset colorPreset { QtAntdButton::PresetDefault };
    
    bool useThemeColors;
    bool isBlock;
    bool isGhost;
    bool isDanger;
    bool isLoading;
    bool isHovered;
    bool isPressed;

    // Loading animation
    QTimer *loadingTimer;
    int loadingAngle;
};

#endif // QTANTDBUTTON_P_H