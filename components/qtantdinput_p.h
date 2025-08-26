#ifndef QTANTDINPUT_P_H
#define QTANTDINPUT_P_H

#include <QtGlobal>
#include <QColor>
#include <QIcon>
#include <QRect>
#include <QToolButton>

class QtAntdInput;

class QtAntdInputPrivate
{
    Q_DISABLE_COPY(QtAntdInputPrivate)
    Q_DECLARE_PUBLIC(QtAntdInput)

public:
    QtAntdInputPrivate(QtAntdInput *q);
    ~QtAntdInputPrivate();

    void init();
    void updateGeometry();
    void updateTextMargins();
    void updateClearButton();
    
    // Layout helpers
    QRect prefixRect() const;
    QRect suffixRect() const;
    QRect clearButtonRect() const;
    QRect textRect() const;
    
    // Size calculations
    int getInputHeight() const;
    int getHorizontalPadding() const;
    int getBorderRadius() const;
    
    // Drawing helpers
    QColor getBorderColor() const;
    QColor getBackgroundColor() const;
    QColor getTextColor() const;

    // Visual margin reserved for focus glow (kept constant to avoid layout jump)
    int getVisualMargin() const { return focusGlowExtent; }

    QtAntdInput *const q_ptr;

    QtAntdInput::InputSize inputSize;
    QtAntdInput::InputStatus inputStatus;
    
    bool useThemeColors;
    bool clearButtonEnabled;
    bool isHovered;
    bool isFocused;
    bool clearButtonHovered;
    bool clearButtonPressed;

    // Prefix/suffix content
    QString prefixText;
    QIcon prefixIcon;
    QString suffixText;
    QIcon suffixIcon;
    
    // Clear button
    QToolButton *clearButton;

    // Reserved outward glow extent (px). Also used as visual margin around inner content rect
    int focusGlowExtent { 4 }; 
};

#endif // QTANTDINPUT_P_H