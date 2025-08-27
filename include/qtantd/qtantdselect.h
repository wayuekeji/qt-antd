#ifndef QTANTDSELECT_H
#define QTANTDSELECT_H

#include <QtWidgets/QWidget>
#include <QVariant>
#include <QStringList>
#include <QIcon>
#include "components_global.h"

class QtAntdSelectPrivate;

/*!
 * \brief Data structure for Select option items
 */
struct COMPONENTS_EXPORT QtAntdSelectOption {
    QString label;           //!< Display text for the option
    QVariant value;         //!< Value associated with the option
    QIcon icon;             //!< Optional icon for the option
    bool disabled = false;  //!< Whether this option is disabled
    QString tooltip;        //!< Optional tooltip text

    QtAntdSelectOption() = default;
    QtAntdSelectOption(const QString &label, const QVariant &value = QVariant())
        : label(label), value(value) {}
    QtAntdSelectOption(const QString &label, const QVariant &value, const QIcon &icon)
        : label(label), value(value), icon(icon) {}
};

/*!
 * \class QtAntdSelect
 * \brief Ant Design styled select control for Qt Widgets.
 *
 * Implements Ant Design select component with dropdown options, search functionality,
 * single/multiple selection modes, and various UI states that integrate with the theme system.
 *
 * Colors and styling are driven by QtAntdStyle/Theme when useThemeColors() is true (default).
 */
class COMPONENTS_EXPORT QtAntdSelect : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString placeholder READ placeholder WRITE setPlaceholder)
    Q_PROPERTY(bool allowClear READ allowClear WRITE setAllowClear)
    Q_PROPERTY(bool disabled READ isDisabled WRITE setDisabled)
    Q_PROPERTY(bool loading READ isLoading WRITE setLoading)
    Q_PROPERTY(bool showSearch READ showSearch WRITE setShowSearch)
    Q_PROPERTY(bool multiple READ isMultiple WRITE setMultiple)

public:
    /*! \brief Select size variants affecting dimensions and appearance. */
    enum SelectSize {
        Small,   //!< Small select (24px height)
        Medium,  //!< Default select (32px height)  
        Large    //!< Large select (40px height)
    };

    /*! \brief Construct a select. */
    explicit QtAntdSelect(QWidget *parent = nullptr);
    /*! \brief Construct a select with placeholder. */
    explicit QtAntdSelect(const QString &placeholder, QWidget *parent = nullptr);
    ~QtAntdSelect();

    /*! \brief Enable/disable using theme-driven colors.
     * When false, falls back to standard widget colors.
     */
    void setUseThemeColors(bool value);
    /*! \brief Whether theme-driven colors are used. */
    bool useThemeColors() const;

    /*! \brief Set the select size. Updates geometry and repaint. */
    void setSelectSize(SelectSize size);
    /*! \brief Current select size. */
    SelectSize selectSize() const;

    /*! \brief Set placeholder text shown when no selection is made. */
    void setPlaceholder(const QString &text);
    /*! \brief Current placeholder text. */
    QString placeholder() const;

    /*! \brief Enable/disable clear button when there is a selection. */
    void setAllowClear(bool allow);
    /*! \brief Whether clear button is enabled. */
    bool allowClear() const;

    /*! \brief Set disabled state. When true, component becomes non-interactive. */
    void setDisabled(bool disabled);
    /*! \brief Whether select is disabled. */
    bool isDisabled() const;

    /*! \brief Set loading state. When true, shows loading spinner. */
    void setLoading(bool loading);
    /*! \brief Whether select is in loading state. */
    bool isLoading() const;

    /*! \brief Enable/disable search functionality in dropdown. */
    void setShowSearch(bool show);
    /*! \brief Whether search is enabled. */
    bool showSearch() const;

    /*! \brief Enable/disable multiple selection mode. */
    void setMultiple(bool multiple);
    /*! \brief Whether multiple selection is enabled. */
    bool isMultiple() const;

    /*! \brief Add an option to the select. */
    void addOption(const QtAntdSelectOption &option);
    /*! \brief Add an option with label and value. */
    void addOption(const QString &label, const QVariant &value = QVariant());
    /*! \brief Add options from a list. */
    void addOptions(const QList<QtAntdSelectOption> &options);

    /*! \brief Remove an option by value. */
    void removeOption(const QVariant &value);
    /*! \brief Remove all options. */
    void clearOptions();

    /*! \brief Get all options. */
    QList<QtAntdSelectOption> options() const;

    /*! \brief Set currently selected value(s). */
    void setValue(const QVariant &value);
    /*! \brief Set multiple selected values. */
    void setValues(const QVariantList &values);
    /*! \brief Get currently selected value. Returns invalid QVariant if nothing selected. */
    QVariant value() const;
    /*! \brief Get currently selected values (for multiple mode). */
    QVariantList values() const;

    /*! \brief Get currently selected option text. */
    QString selectedText() const;
    /*! \brief Get currently selected option texts (for multiple mode). */
    QStringList selectedTexts() const;

    /*! \brief Clear current selection. */
    void clearSelection();

    /*! \brief Size hint based on select size and content. */
    QSize sizeHint() const Q_DECL_OVERRIDE;
    /*! \brief Minimum size hint. */
    QSize minimumSizeHint() const Q_DECL_OVERRIDE;

signals:
    /*! \brief Emitted when selection changes. */
    void selectionChanged();
    /*! \brief Emitted when a specific value is selected. */
    void valueSelected(const QVariant &value);
    /*! \brief Emitted when dropdown opens. */
    void dropdownOpened();
    /*! \brief Emitted when dropdown closes. */
    void dropdownClosed();
    /*! \brief Emitted when search text changes (when showSearch is true). */
    void searchTextChanged(const QString &text);

protected:
    /*! \brief Custom paint implementing Ant Design select appearance. */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Handle mouse press for dropdown toggle. */
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Handle mouse release. */
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
    /*! \brief Handle keyboard interaction (Arrow keys, Enter, Escape, etc). */
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    /*! \brief Handle resize events. */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    const QScopedPointer<QtAntdSelectPrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtAntdSelect)
    Q_DECLARE_PRIVATE(QtAntdSelect)
};

#endif // QTANTDSELECT_H