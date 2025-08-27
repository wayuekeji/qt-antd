#ifndef QTANTDSELECT_P_H
#define QTANTDSELECT_P_H

#include <QtGlobal>
#include <QColor>
#include <QTimer>
#include <QPropertyAnimation>
#include <QListWidget>
#include <QLineEdit>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include "qtantdselect.h"

class QtAntdSelect;

/*!
 * \brief Custom dropdown widget for Select component
 */
class QtAntdSelectDropdown : public QFrame
{
    Q_OBJECT

public:
    explicit QtAntdSelectDropdown(QtAntdSelect *parent);
    ~QtAntdSelectDropdown();

    void setOptions(const QList<QtAntdSelectOption> &options);
    void setSelectedValues(const QVariantList &values);
    void setSearchEnabled(bool enabled);
    void setMultipleSelection(bool multiple);
    void showDropdown();
    void hideDropdown();
    bool isVisible() const;
    void updatePosition();

    QVariantList selectedValues() const;

signals:
    void optionSelected(const QVariant &value);
    void optionDeselected(const QVariant &value);
    void searchTextChanged(const QString &text);
    void dropdownClosed();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onSearchTextChanged(const QString &text);
    void onItemClicked();

private:
    void createUI();
    void updateOptionsList();
    void filterOptions(const QString &searchText);
    void selectOption(const QVariant &value);
    void deselectOption(const QVariant &value);

    QtAntdSelect *m_select;
    QVBoxLayout *m_mainLayout;
    QLineEdit *m_searchEdit;
    QScrollArea *m_scrollArea;
    QWidget *m_optionsContainer;
    QVBoxLayout *m_optionsLayout;
    
    QList<QtAntdSelectOption> m_options;
    QVariantList m_selectedValues;
    bool m_searchEnabled;
    bool m_multipleSelection;
    bool m_isVisible;

    // Option item widgets
    struct OptionItemWidget {
        QFrame *container;
        QLabel *textLabel;
        QLabel *iconLabel;
        QPushButton *checkbox; // for multiple selection
        QtAntdSelectOption option;
        bool isSelected;
        bool isHighlighted;
    };
    QList<OptionItemWidget> m_optionWidgets;
    int m_highlightedIndex;
};

/*!
 * \brief Private implementation for QtAntdSelect
 */
class QtAntdSelectPrivate
{
    Q_DISABLE_COPY(QtAntdSelectPrivate)
    Q_DECLARE_PUBLIC(QtAntdSelect)

public:
    QtAntdSelectPrivate(QtAntdSelect *q);
    ~QtAntdSelectPrivate();

    void init();
    void updateGeometry();

    // Drawing helpers
    QColor getBorderColor() const;
    QColor getBackgroundColor() const;
    QColor getTextColor() const;
    QColor getPlaceholderColor() const;
    QRect getContentRect() const;
    QRect getArrowRect() const;
    QRect getClearButtonRect() const;
    int getVisualPadding() const;

    // Interaction helpers
    void toggleDropdown();
    void openDropdown();
    void closeDropdown();
    void updateSelectedDisplay();
    void clearSelection();
    bool isPointInArrow(const QPoint &pos) const;
    bool isPointInClearButton(const QPoint &pos) const;

    // Animation
    void startLoadingAnimation();
    void stopLoadingAnimation();
    void drawLoadingSpinner(QPainter *painter, const QRect &rect, const QColor &color);

    // Tag rendering (for multiple selection)
    void renderSelectedTags(QPainter *painter, const QRect &contentRect);
    QSize calculateTagSize(const QString &text) const;

    QtAntdSelect *const q_ptr;

    // Properties
    QtAntdSelect::SelectSize selectSize;
    bool useThemeColors;
    QString placeholder;
    bool allowClear;
    bool disabled;
    bool isLoading;
    bool showSearch;
    bool multiple;

    // State
    bool isHovered;
    bool isPressed;
    bool hasFocus;
    bool dropdownOpen;

    // Data
    QList<QtAntdSelectOption> options;
    QVariantList selectedValues;
    QString displayText;

    // UI Components
    QtAntdSelectDropdown *dropdown;

    // Animation
    QTimer *loadingTimer;
    int loadingAngle;

    // Cached dimensions based on size
    int selectHeight;
    int borderRadius;
    int fontSize;
    int iconSize;
    int padding;
};

#endif // QTANTDSELECT_P_H