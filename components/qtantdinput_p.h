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
};

#endif // QTANTDINPUT_P_H