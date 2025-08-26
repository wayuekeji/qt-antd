#ifndef QTANTDINPUT_H
#define QTANTDINPUT_H

#include <QtWidgets/QLineEdit>
#include <QIcon>
#include "components_global.h"

class QtAntdInputPrivate;

/*!\class QtAntdInput
 * \brief Ant Design styled line edit input for Qt Widgets.
 *
 * Implements Ant Design input field with sizes, status states, prefix/suffix support,
 * clear button functionality, and theme integration.
 *
 * Colors are driven by QtAntdStyle/Theme when useThemeColors() is true (default).
 */
class COMPONENTS_EXPORT QtAntdInput : public QLineEdit
{
    Q_OBJECT

public:
    /*! \brief Predefined input sizes affecting height and paddings. */
    enum InputSize {
        Small,
        Medium,
        Large
    };

    /*! \brief Input status states affecting border and text colors. */
    enum InputStatus {
        Normal,     //!< Default state
        Error,      //!< Error state with error colors
        Warning,    //!< Warning state with warning colors
        Success     //!< Success state with success colors
    };

    /*! \brief Construct an input field. */
    explicit QtAntdInput(QWidget *parent = nullptr);
    /*! \brief Construct an input field with placeholder text. */
    explicit QtAntdInput(const QString &placeholder, QWidget *parent = nullptr);
    ~QtAntdInput();

    /*! \brief Enable/disable using theme-driven colors.
     * When false, falls back to QLineEdit styling.
     */
    void setUseThemeColors(bool value);
    /*! \brief Whether theme-driven colors are used. */
    bool useThemeColors() const;

    /*! \brief Set the input size. Updates geometry and repaint. */
    void setInputSize(InputSize size);
    /*! \brief Current input size. */
    InputSize inputSize() const;

    /*! \brief Set the input status state. */
    void setInputStatus(InputStatus status);
    /*! \brief Current input status. */
    InputStatus inputStatus() const;

    /*! \brief Enable/disable clear button functionality. */
    void setClearButtonEnabled(bool enabled);
    /*! \brief Whether clear button is enabled. */
    bool isClearButtonEnabled() const;

    /*! \brief Set prefix text or icon. */
    void setPrefixText(const QString &text);
    void setPrefixIcon(const QIcon &icon);
    /*! \brief Get current prefix text. */
    QString prefixText() const;
    /*! \brief Get current prefix icon. */
    QIcon prefixIcon() const;

    /*! \brief Set suffix text or icon. */
    void setSuffixText(const QString &text);
    void setSuffixIcon(const QIcon &icon);
    /*! \brief Get current suffix text. */
    QString suffixText() const;
    /*! \brief Get current suffix icon. */
    QIcon suffixIcon() const;

    /*! \brief Size hint factoring size and prefix/suffix content. */
    QSize sizeHint() const Q_DECL_OVERRIDE;
    /*! \brief Minimum size hint per Ant Design sizing. */
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

signals:
    /*! \brief Emitted when clear button is clicked. */
    void clearButtonClicked();

protected:
    /*! \brief Custom paint implementing Ant Design look & feel. */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Handle focus changes for border styling. */
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Handle mouse events for clear button. */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Tracks hover state. */
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void enterEvent(QEnterEvent *event) Q_DECL_OVERRIDE;
#else
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
#endif
    /*! \brief Resets hover state. */
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Handle resize for layout adjustments. */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtAntdInputPrivate> d_ptr;

private slots:
    void onTextChanged();
    void onClearButtonClicked();

private:
    Q_DISABLE_COPY(QtAntdInput)
    Q_DECLARE_PRIVATE(QtAntdInput)
};

#endif // QTANTDINPUT_H