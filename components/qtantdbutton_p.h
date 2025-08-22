#ifndef QTANTDBUTTON_P_H
#define QTANTDBUTTON_P_H

#include <QtGlobal>
#include <QColor>

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
    void updateColors();
    
    QColor getBackgroundColor() const;
    QColor getBorderColor() const;
    QColor getTextColor() const;
    
    QColor getHoverBackgroundColor() const;
    QColor getHoverBorderColor() const;
    QColor getHoverTextColor() const;
    
    QColor getPressedBackgroundColor() const;
    QColor getPressedBorderColor() const;
    QColor getPressedTextColor() const;

    QtAntdButton *const q_ptr;

    QtAntdButton::ButtonType buttonType;
    QtAntdButton::ButtonSize buttonSize;
    QtAntdButton::ButtonShape buttonShape;
    
    bool useThemeColors;
    bool isBlock;
    bool isGhost;
    bool isDanger;
    bool isLoading;
    bool isHovered;
    bool isPressed;

    // Cached colors for performance
    QColor backgroundColor;
    QColor borderColor;
    QColor textColor;
    QColor hoverBackgroundColor;
    QColor hoverBorderColor;
    QColor hoverTextColor;
    QColor pressedBackgroundColor;
    QColor pressedBorderColor;
    QColor pressedTextColor;
};

#endif // QTANTDBUTTON_P_H