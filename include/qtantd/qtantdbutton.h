#ifndef QTANTDBUTTON_H
#define QTANTDBUTTON_H

#include <QtWidgets/QPushButton>
#include <QIcon>
#include "components_global.h"

class QtAntdButtonPrivate;

/*!\class QtAntdButton
 * \brief Ant Design styled push button for Qt Widgets.
 *
 * Implements Ant Design button types, sizes, shapes, block/ghost/danger states,
 * an optional loading spinner, and color presets that integrate with the theme system.
 *
 * Colors are driven by QtAntdStyle/Theme when useThemeColors() is true (default).
 */
class COMPONENTS_EXPORT QtAntdButton : public QPushButton
{
    Q_OBJECT

public:
    /*! \brief Ant Design button visual types. */
    enum ButtonType {
        Primary,   //!< Filled brand color button
        Default,   //!< Bordered button
        Dashed,    //!< Bordered button with dashed border
        Text,      //!< Text button (no border)
        Link       //!< Link-styled button (no border)
    };

    /*! \brief Predefined button sizes affecting height, paddings and spinner size. */
    enum ButtonSize {
        Small,
        Medium,
        Large
    };

    /*! \brief Shape options affecting corner radius and geometry. */
    enum ButtonShape {
        DefaultShape, //!< Round rect with default radius
        Circle,       //!< Circle, sized by min(width,height)
        Round         //!< Pill (full height radius)
    };

    /*! \brief Color presets mapping to theme palettes and custom colors.
     *
     * PresetDefault follows the theme primary palette by default.
     */
    enum ColorPreset {
        PresetDefault,   //!< Follow theme defaults (current behavior equals primary)
        PresetPrimary,   //!< Theme primary palette
        PresetDanger,    //!< Theme error palette
        PresetPink,      //!< Magenta/Pink palette
        PresetPurple,    //!< Purple palette
        PresetCyan       //!< Cyan palette
    };

    /*! \brief Construct an empty button. */
    explicit QtAntdButton(QWidget *parent = nullptr);
    /*! \brief Construct a text button. */
    explicit QtAntdButton(const QString &text, QWidget *parent = nullptr);
    /*! \brief Construct an icon + text button. */
    explicit QtAntdButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~QtAntdButton();

    /*! \brief Enable/disable using theme-driven colors.
     * When false, falls back to QPushButton painting.
     */
    void setUseThemeColors(bool value);
    /*! \brief Whether theme-driven colors are used. */
    bool useThemeColors() const;

    /*! \brief Set the visual type. Triggers a repaint. */
    void setButtonType(ButtonType type);
    /*! \brief Current visual type. */
    ButtonType buttonType() const;

    /*! \brief Set the size. Updates geometry and repaint. */
    void setButtonSize(ButtonSize size);
    /*! \brief Current size. */
    ButtonSize buttonSize() const;

    /*! \brief Set the shape (default, circle, round). */
    void setButtonShape(ButtonShape shape);
    /*! \brief Current shape. */
    ButtonShape buttonShape() const;

    /*! \brief Set block mode (expands horizontally). */
    void setBlock(bool block);
    /*! \brief Whether block mode is enabled. */
    bool isBlock() const;

    /*! \brief Set ghost mode (transparent background style for border/text types). */
    void setGhost(bool ghost);
    /*! \brief Whether ghost mode is enabled. */
    bool isGhost() const;

    /*! \brief Mark as danger (uses error palette). */
    void setDanger(bool danger);
    /*! \brief Whether danger style is active. */
    bool isDanger() const;

    /*! \brief Toggle loading state. Shows a spinner and disables icon.
     * Also animates the spinner and adjusts size hints accordingly.
     */
    void setLoading(bool loading);
    /*! \brief Whether loading is active. */
    bool isLoading() const;

    /*! \brief Set a color preset overriding type default brand color. */
    void setColorPreset(ColorPreset preset);
    /*! \brief Current color preset. */
    ColorPreset colorPreset() const;

    /*! \brief Size hint factoring size, text/icon, and loading spinner. */
    QSize sizeHint() const Q_DECL_OVERRIDE;
    /*! \brief Minimum size hint per Ant Design sizing. */
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

protected:
    /*! \brief Custom paint implementing Ant Design look & feel. */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Tracks press state and triggers ripple on release. */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Resets press state and starts outer ripple if applicable. */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Tracks hover state. */
    void enterEvent(QEnterEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Resets hover/press state. */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtAntdButtonPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtAntdButton)
    Q_DECLARE_PRIVATE(QtAntdButton)
};

#endif // QTANTDBUTTON_H