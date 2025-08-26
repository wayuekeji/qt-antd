#ifndef QTANTDBUTTON_H
#define QTANTDBUTTON_H

#include <QtWidgets/QPushButton>
#include <QIcon>
#include "components_global.h"

class QtAntdButtonPrivate;

class COMPONENTS_EXPORT QtAntdButton : public QPushButton
{
    Q_OBJECT

public:
    enum ButtonType {
        Primary,
        Default,
        Dashed,
        Text,
        Link
    };

    enum ButtonSize {
        Small,
        Medium,
        Large
    };

    enum ButtonShape {
        DefaultShape,
        Circle,
        Round
    };

    enum ColorPreset {
        PresetDefault,   // Follow theme defaults (existing behaviour)
        PresetPrimary,   // Use theme primary palette
        PresetDanger,    // Use theme error palette
        PresetPink,      // Magenta/Pink palette
        PresetPurple,    // Purple palette
        PresetCyan       // Cyan palette
    };

    explicit QtAntdButton(QWidget *parent = nullptr);
    explicit QtAntdButton(const QString &text, QWidget *parent = nullptr);
    explicit QtAntdButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~QtAntdButton();

    void setUseThemeColors(bool value);
    bool useThemeColors() const;

    void setButtonType(ButtonType type);
    ButtonType buttonType() const;

    void setButtonSize(ButtonSize size);
    ButtonSize buttonSize() const;

    void setButtonShape(ButtonShape shape);
    ButtonShape buttonShape() const;

    void setBlock(bool block);
    bool isBlock() const;

    void setGhost(bool ghost);
    bool isGhost() const;

    void setDanger(bool danger);
    bool isDanger() const;

    void setLoading(bool loading);
    bool isLoading() const;

    void setColorPreset(ColorPreset preset);
    ColorPreset colorPreset() const;

    QSize sizeHint() const Q_DECL_OVERRIDE;
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEnterEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtAntdButtonPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtAntdButton)
    Q_DECLARE_PRIVATE(QtAntdButton)
};

#endif // QTANTDBUTTON_H