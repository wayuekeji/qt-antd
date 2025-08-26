#ifndef QTANTDSWITCH_H
#define QTANTDSWITCH_H

#include <QtWidgets/QAbstractButton>
#include <QIcon>
#include "components_global.h"

class QtAntdSwitchPrivate;

/*!
 * \class QtAntdSwitch
 * \brief Ant Design styled switch control for Qt Widgets.
 *
 * Implements Ant Design switch component with on/off states, loading state,
 * text/icon support, and size variants that integrate with the theme system.
 *
 * Colors are driven by QtAntdStyle/Theme when useThemeColors() is true (default).
 */
class COMPONENTS_EXPORT QtAntdSwitch : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(qreal animationValue READ animationValue WRITE setAnimationValue)

public:
    /*! \brief Switch size variants affecting dimensions and appearance. */
    enum SwitchSize {
        Small,   //!< Small switch (16px height)
        Medium,  //!< Default switch (22px height)
        Large    //!< Large switch (28px height)
    };

    /*! \brief Construct a switch. */
    explicit QtAntdSwitch(QWidget *parent = nullptr);
    /*! \brief Construct a switch with text. */
    explicit QtAntdSwitch(const QString &text, QWidget *parent = nullptr);
    ~QtAntdSwitch();

    /*! \brief Enable/disable using theme-driven colors.
     * When false, falls back to standard widget colors.
     */
    void setUseThemeColors(bool value);
    /*! \brief Whether theme-driven colors are used. */
    bool useThemeColors() const;

    /*! \brief Set the switch size. Updates geometry and repaint. */
    void setSwitchSize(SwitchSize size);
    /*! \brief Current switch size. */
    SwitchSize switchSize() const;

    /*! \brief Set text displayed when switch is checked (on). */
    void setCheckedText(const QString &text);
    /*! \brief Text displayed when switch is checked. */
    QString checkedText() const;

    /*! \brief Set text displayed when switch is unchecked (off). */
    void setUncheckedText(const QString &text);
    /*! \brief Text displayed when switch is unchecked. */
    QString uncheckedText() const;

    /*! \brief Set icon displayed when switch is checked (on). */
    void setCheckedIcon(const QIcon &icon);
    /*! \brief Icon displayed when switch is checked. */
    QIcon checkedIcon() const;

    /*! \brief Set icon displayed when switch is unchecked (off). */
    void setUncheckedIcon(const QIcon &icon);
    /*! \brief Icon displayed when switch is unchecked. */
    QIcon uncheckedIcon() const;

    /*! \brief Set loading state. When true, shows loading spinner and disables interaction. */
    void setLoading(bool loading);
    /*! \brief Whether switch is in loading state. */
    bool isLoading() const;

    /*! \brief Size hint based on switch size and content. */
    QSize sizeHint() const Q_DECL_OVERRIDE;
    /*! \brief Minimum size hint. */
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

    /*! \brief Animation value for internal use (0.0 = off, 1.0 = on). */
    qreal animationValue() const;
    /*! \brief Set animation value for internal use. */
    void setAnimationValue(qreal value);

protected:
    /*! \brief Custom paint implementing Ant Design switch appearance. */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Handle mouse press for interaction. */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Handle mouse release for toggle action. */
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Track hover state for visual feedback. */
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEnterEvent *event) Q_DECL_OVERRIDE;
#else
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
#endif
    /*! \brief Reset hover state. */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Handle focus changes. */
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Handle keyboard interaction (Space/Enter to toggle). */
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtAntdSwitchPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtAntdSwitch)
    Q_DECLARE_PRIVATE(QtAntdSwitch)
};

#endif // QTANTDSWITCH_H