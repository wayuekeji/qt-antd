#include "qtantdselect.h"
#include "qtantdselect_p.h"

#include "qtantdstyle.h"
#include "antdlib/qtantdrippleoverlay_p.h"
#include <QPainter>
#include <QFontMetrics>
#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QEnterEvent>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QTimer>
#include <QtMath>
#include <QDebug>
#include <QScreen>
#include <QScrollBar>

namespace {
    // Size constants based on Ant Design specifications
    constexpr int gSmallHeight = 24;
    constexpr int gMediumHeight = 32;
    constexpr int gLargeHeight = 40;
    
    constexpr int gBorderRadius = 4;
    constexpr int gBorderWidth = 1;
    constexpr int gPadding = 8;
    constexpr int gIconSize = 16;
    
    constexpr int gAnimationDuration = 150;
    constexpr int gLoadingSpinnerSpeed = 16; // ms per frame, ~60fps
    
    constexpr int gDropdownMaxHeight = 200;
    constexpr int gDropdownItemHeight = 32;
    constexpr int gDropdownPadding = 4;
    
    // Tag constants (for multiple selection)
    constexpr int gTagHeight = 20;
    constexpr int gTagPadding = 4;
    constexpr int gTagRadius = 2;
    constexpr int gTagSpacing = 4;
}

// QtAntdSelectDropdown Implementation
QtAntdSelectDropdown::QtAntdSelectDropdown(QtAntdSelect *parent)
    : QFrame(parent->window(), Qt::Popup | Qt::FramelessWindowHint)
    , m_select(parent)
    , m_mainLayout(nullptr)
    , m_searchEdit(nullptr)
    , m_scrollArea(nullptr)
    , m_optionsContainer(nullptr)
    , m_optionsLayout(nullptr)
    , m_searchEnabled(false)
    , m_multipleSelection(false)
    , m_isVisible(false)
    , m_highlightedIndex(-1)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setFocusPolicy(Qt::StrongFocus);
    createUI();
}

QtAntdSelectDropdown::~QtAntdSelectDropdown()
{
}

void QtAntdSelectDropdown::createUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(gDropdownPadding, gDropdownPadding, gDropdownPadding, gDropdownPadding);
    m_mainLayout->setSpacing(gDropdownPadding);
    
    // Search input (initially hidden)
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText("Search...");
    m_searchEdit->hide();
    m_mainLayout->addWidget(m_searchEdit);
    
    connect(m_searchEdit, &QLineEdit::textChanged, this, &QtAntdSelectDropdown::onSearchTextChanged);
    
    // Scrollable options container
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    
    m_optionsContainer = new QWidget();
    m_optionsLayout = new QVBoxLayout(m_optionsContainer);
    m_optionsLayout->setContentsMargins(0, 0, 0, 0);
    m_optionsLayout->setSpacing(0);
    
    m_scrollArea->setWidget(m_optionsContainer);
    m_mainLayout->addWidget(m_scrollArea);
    
    // Install event filter for keyboard navigation
    installEventFilter(this);
    m_searchEdit->installEventFilter(this);
}

void QtAntdSelectDropdown::setOptions(const QList<QtAntdSelectOption> &options)
{
    m_options = options;
    updateOptionsList();
}

void QtAntdSelectDropdown::setSelectedValues(const QVariantList &values)
{
    m_selectedValues = values;
    // Update visual selection state of options
    for (auto &optionWidget : m_optionWidgets) {
        optionWidget.isSelected = m_selectedValues.contains(optionWidget.option.value);
        if (optionWidget.checkbox) {
            optionWidget.checkbox->setChecked(optionWidget.isSelected);
        }
    }
    update();
}

void QtAntdSelectDropdown::setSearchEnabled(bool enabled)
{
    m_searchEnabled = enabled;
    m_searchEdit->setVisible(enabled);
}

void QtAntdSelectDropdown::setMultipleSelection(bool multiple)
{
    m_multipleSelection = multiple;
    updateOptionsList();
}

void QtAntdSelectDropdown::updateOptionsList()
{
    // Clear existing option widgets
    for (const auto &optionWidget : m_optionWidgets) {
        optionWidget.container->deleteLater();
    }
    m_optionWidgets.clear();
    
    // Create new option widgets
    for (const auto &option : m_options) {
        OptionItemWidget optionWidget;
        optionWidget.option = option;
        optionWidget.isSelected = m_selectedValues.contains(option.value);
        optionWidget.isHighlighted = false;
        
        // Create container frame
        optionWidget.container = new QFrame(m_optionsContainer);
        optionWidget.container->setFixedHeight(gDropdownItemHeight);
        optionWidget.container->setCursor(Qt::PointingHandCursor);
        
        QHBoxLayout *itemLayout = new QHBoxLayout(optionWidget.container);
        itemLayout->setContentsMargins(gPadding, 4, gPadding, 4);
        itemLayout->setSpacing(gPadding);
        
        // Icon (if present)
        optionWidget.iconLabel = new QLabel(optionWidget.container);
        if (!option.icon.isNull()) {
            optionWidget.iconLabel->setPixmap(option.icon.pixmap(gIconSize, gIconSize));
            optionWidget.iconLabel->setFixedSize(gIconSize, gIconSize);
            itemLayout->addWidget(optionWidget.iconLabel);
        } else {
            optionWidget.iconLabel->hide();
        }
        
        // Text label
        optionWidget.textLabel = new QLabel(option.label, optionWidget.container);
        optionWidget.textLabel->setEnabled(!option.disabled);
        itemLayout->addWidget(optionWidget.textLabel);
        
        itemLayout->addStretch();
        
        // Checkbox for multiple selection
        if (m_multipleSelection) {
            optionWidget.checkbox = new QPushButton(optionWidget.container);
            optionWidget.checkbox->setCheckable(true);
            optionWidget.checkbox->setChecked(optionWidget.isSelected);
            optionWidget.checkbox->setFixedSize(16, 16);
            itemLayout->addWidget(optionWidget.checkbox);
            
            connect(optionWidget.checkbox, &QPushButton::toggled, [this, option](bool checked) {
                if (checked) {
                    selectOption(option.value);
                } else {
                    deselectOption(option.value);
                }
            });
        } else {
            optionWidget.checkbox = nullptr;
        }
        
        // Install event filter for hover and click handling
        optionWidget.container->installEventFilter(this);
        
        m_optionsLayout->addWidget(optionWidget.container);
        m_optionWidgets.append(optionWidget);
    }
    
    m_optionsLayout->addStretch();
}

void QtAntdSelectDropdown::showDropdown()
{
    if (m_isVisible) return;
    
    updatePosition();
    show();
    m_isVisible = true;
    
    if (m_searchEnabled && m_searchEdit) {
        m_searchEdit->setFocus();
        m_searchEdit->selectAll();
    } else {
        setFocus();
    }
}

void QtAntdSelectDropdown::hideDropdown()
{
    if (!m_isVisible) return;
    
    hide();
    m_isVisible = false;
    emit dropdownClosed();
}

bool QtAntdSelectDropdown::isVisible() const
{
    return m_isVisible && QFrame::isVisible();
}

void QtAntdSelectDropdown::updatePosition()
{
    if (!m_select) return;
    
    QPoint globalPos = m_select->mapToGlobal(QPoint(0, m_select->height()));
    QSize selectSize = m_select->size();
    
    // Calculate dropdown size
    int dropdownWidth = selectSize.width();
    int dropdownHeight = qMin(gDropdownMaxHeight, m_options.size() * gDropdownItemHeight + gDropdownPadding * 2);
    
    if (m_searchEnabled) {
        dropdownHeight += m_searchEdit->sizeHint().height() + gDropdownPadding;
    }
    
    // Check if there's enough space below
    QScreen *screen = QApplication::screenAt(globalPos);
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        if (globalPos.y() + dropdownHeight > screenGeometry.bottom()) {
            // Show above if there's more space
            globalPos.setY(m_select->mapToGlobal(QPoint(0, 0)).y() - dropdownHeight);
        }
    }
    
    setGeometry(globalPos.x(), globalPos.y(), dropdownWidth, dropdownHeight);
}

QVariantList QtAntdSelectDropdown::selectedValues() const
{
    return m_selectedValues;
}

void QtAntdSelectDropdown::selectOption(const QVariant &value)
{
    if (!m_selectedValues.contains(value)) {
        if (m_multipleSelection) {
            m_selectedValues.append(value);
        } else {
            m_selectedValues.clear();
            m_selectedValues.append(value);
        }
        emit optionSelected(value);
    }
    
    // Update visual state
    for (auto &optionWidget : m_optionWidgets) {
        if (optionWidget.option.value == value) {
            optionWidget.isSelected = true;
            if (optionWidget.checkbox) {
                optionWidget.checkbox->setChecked(true);
            }
        } else if (!m_multipleSelection) {
            optionWidget.isSelected = false;
            if (optionWidget.checkbox) {
                optionWidget.checkbox->setChecked(false);
            }
        }
    }
    
    // Close dropdown for single selection
    if (!m_multipleSelection) {
        QTimer::singleShot(100, this, &QtAntdSelectDropdown::hideDropdown);
    }
}

void QtAntdSelectDropdown::deselectOption(const QVariant &value)
{
    if (m_selectedValues.contains(value)) {
        m_selectedValues.removeAll(value);
        emit optionDeselected(value);
        
        // Update visual state
        for (auto &optionWidget : m_optionWidgets) {
            if (optionWidget.option.value == value) {
                optionWidget.isSelected = false;
                if (optionWidget.checkbox) {
                    optionWidget.checkbox->setChecked(false);
                }
                break;
            }
        }
    }
}

void QtAntdSelectDropdown::onSearchTextChanged(const QString &text)
{
    filterOptions(text);
    emit searchTextChanged(text);
}

void QtAntdSelectDropdown::filterOptions(const QString &searchText)
{
    for (const auto &optionWidget : m_optionWidgets) {
        bool matches = searchText.isEmpty() || 
                      optionWidget.option.label.contains(searchText, Qt::CaseInsensitive);
        optionWidget.container->setVisible(matches);
    }
}

void QtAntdSelectDropdown::onItemClicked()
{
    // This will be handled by event filter
}

void QtAntdSelectDropdown::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw dropdown background and border
    QColor bgColor = Qt::white; // TODO: Use theme colors
    QColor borderColor = QColor(217, 217, 217);
    
    painter.fillRect(rect(), bgColor);
    painter.setPen(QPen(borderColor, 1));
    painter.drawRoundedRect(rect().adjusted(0, 0, -1, -1), gBorderRadius, gBorderRadius);
    
    // Draw shadow
    QColor shadowColor = Qt::black;
    shadowColor.setAlpha(15);
    for (int i = 1; i <= 3; ++i) {
        painter.setPen(QPen(shadowColor, 1));
        painter.drawRoundedRect(rect().adjusted(-i, -i, i, i), gBorderRadius + i, gBorderRadius + i);
        shadowColor.setAlpha(shadowColor.alpha() / 2);
    }
}

bool QtAntdSelectDropdown::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            // Find which option was clicked
            for (const auto &optionWidget : m_optionWidgets) {
                if (obj == optionWidget.container) {
                    if (!optionWidget.option.disabled) {
                        selectOption(optionWidget.option.value);
                    }
                    return true;
                }
            }
        }
    } else if (event->type() == QEvent::Enter) {
        // Handle hover state
        for (int i = 0; i < m_optionWidgets.size(); ++i) {
            if (obj == m_optionWidgets[i].container) {
                m_highlightedIndex = i;
                update();
                break;
            }
        }
    } else if (event->type() == QEvent::Leave) {
        // Reset hover state
        for (const auto &optionWidget : m_optionWidgets) {
            if (obj == optionWidget.container) {
                m_highlightedIndex = -1;
                update();
                break;
            }
        }
    }
    
    return QFrame::eventFilter(obj, event);
}

void QtAntdSelectDropdown::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        hideDropdown();
        break;
    case Qt::Key_Up:
        // Navigate up
        if (m_highlightedIndex > 0) {
            m_highlightedIndex--;
            update();
        }
        event->accept();
        break;
    case Qt::Key_Down:
        // Navigate down
        if (m_highlightedIndex < m_optionWidgets.size() - 1) {
            m_highlightedIndex++;
            update();
        }
        event->accept();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        // Select highlighted option
        if (m_highlightedIndex >= 0 && m_highlightedIndex < m_optionWidgets.size()) {
            const auto &optionWidget = m_optionWidgets[m_highlightedIndex];
            if (!optionWidget.option.disabled) {
                selectOption(optionWidget.option.value);
            }
        }
        event->accept();
        break;
    default:
        QFrame::keyPressEvent(event);
        break;
    }
}

// QtAntdSelectPrivate Implementation
QtAntdSelectPrivate::QtAntdSelectPrivate(QtAntdSelect *q)
    : q_ptr(q)
    , selectSize(QtAntdSelect::Medium)
    , useThemeColors(true)
    , placeholder("Please select")
    , allowClear(false)
    , disabled(false)
    , isLoading(false)
    , showSearch(false)
    , multiple(false)
    , isHovered(false)
    , isPressed(false)
    , hasFocus(false)
    , dropdownOpen(false)
    , dropdown(nullptr)
    , loadingTimer(nullptr)
    , loadingAngle(0)
    , selectHeight(gMediumHeight)
    , borderRadius(gBorderRadius)
    , fontSize(14)
    , iconSize(gIconSize)
    , padding(gPadding)
{
}

QtAntdSelectPrivate::~QtAntdSelectPrivate()
{
    if (dropdown) {
        dropdown->deleteLater();
    }
}

void QtAntdSelectPrivate::init()
{
    Q_Q(QtAntdSelect);
    
    // Create dropdown
    dropdown = new QtAntdSelectDropdown(q);
    
    // Connect dropdown signals
    QObject::connect(dropdown, &QtAntdSelectDropdown::optionSelected, [this](const QVariant &value) {
        Q_Q(QtAntdSelect);
        if (!multiple) {
            selectedValues.clear();
        }
        if (!selectedValues.contains(value)) {
            selectedValues.append(value);
        }
        updateSelectedDisplay();
        emit q->selectionChanged();
        emit q->valueSelected(value);
    });
    
    QObject::connect(dropdown, &QtAntdSelectDropdown::optionDeselected, [this](const QVariant &value) {
        Q_Q(QtAntdSelect);
        selectedValues.removeAll(value);
        updateSelectedDisplay();
        emit q->selectionChanged();
    });
    
    QObject::connect(dropdown, &QtAntdSelectDropdown::dropdownClosed, [this]() {
        dropdownOpen = false;
        q_ptr->update();
    });
    
    QObject::connect(dropdown, &QtAntdSelectDropdown::searchTextChanged, [this](const QString &text) {
        emit q_ptr->searchTextChanged(text);
    });
    
    // Setup loading timer
    loadingTimer = new QTimer(q);
    QObject::connect(loadingTimer, &QTimer::timeout, [this]() {
        loadingAngle = (loadingAngle + 6) % 360;
        q_ptr->update();
    });
    
    updateGeometry();
}

void QtAntdSelectPrivate::updateGeometry()
{
    switch (selectSize) {
    case QtAntdSelect::Small:
        selectHeight = gSmallHeight;
        fontSize = 12;
        break;
    case QtAntdSelect::Medium:
        selectHeight = gMediumHeight;
        fontSize = 14;
        break;
    case QtAntdSelect::Large:
        selectHeight = gLargeHeight;
        fontSize = 16;
        break;
    }
    
    q_ptr->setFixedHeight(selectHeight);
    q_ptr->updateGeometry();
}

QColor QtAntdSelectPrivate::getBorderColor() const
{
    if (disabled) {
        return QColor(217, 217, 217);
    }
    if (hasFocus || dropdownOpen) {
        return useThemeColors ? QColor(64, 169, 255) : QColor(64, 169, 255); // primary color
    }
    if (isHovered) {
        return QColor(64, 169, 255);
    }
    return QColor(217, 217, 217);
}

QColor QtAntdSelectPrivate::getBackgroundColor() const
{
    if (disabled) {
        return QColor(245, 245, 245);
    }
    return Qt::white;
}

QColor QtAntdSelectPrivate::getTextColor() const
{
    if (disabled) {
        return QColor(191, 191, 191);
    }
    return Qt::black;
}

QColor QtAntdSelectPrivate::getPlaceholderColor() const
{
    return QColor(191, 191, 191);
}

QRect QtAntdSelectPrivate::getContentRect() const
{
    QRect rect = q_ptr->rect();
    int rightPadding = padding + iconSize + padding; // space for arrow
    if (allowClear && !selectedValues.isEmpty()) {
        rightPadding += iconSize + padding; // space for clear button
    }
    return rect.adjusted(padding, 0, -rightPadding, 0);
}

QRect QtAntdSelectPrivate::getArrowRect() const
{
    QRect rect = q_ptr->rect();
    int x = rect.right() - padding - iconSize;
    int y = rect.center().y() - iconSize / 2;
    return QRect(x, y, iconSize, iconSize);
}

QRect QtAntdSelectPrivate::getClearButtonRect() const
{
    if (!allowClear || selectedValues.isEmpty()) {
        return QRect();
    }
    
    QRect arrowRect = getArrowRect();
    int x = arrowRect.left() - padding - iconSize;
    int y = arrowRect.y();
    return QRect(x, y, iconSize, iconSize);
}

int QtAntdSelectPrivate::getVisualPadding() const
{
    return padding;
}

void QtAntdSelectPrivate::toggleDropdown()
{
    if (disabled || isLoading) {
        return;
    }
    
    if (dropdownOpen) {
        closeDropdown();
    } else {
        openDropdown();
    }
}

void QtAntdSelectPrivate::openDropdown()
{
    if (dropdownOpen || disabled || isLoading) {
        return;
    }
    
    dropdownOpen = true;
    dropdown->setOptions(options);
    dropdown->setSelectedValues(selectedValues);
    dropdown->setSearchEnabled(showSearch);
    dropdown->setMultipleSelection(multiple);
    dropdown->showDropdown();
    
    q_ptr->update();
    emit q_ptr->dropdownOpened();
}

void QtAntdSelectPrivate::closeDropdown()
{
    if (!dropdownOpen) {
        return;
    }
    
    dropdown->hideDropdown();
    dropdownOpen = false;
    q_ptr->update();
    emit q_ptr->dropdownClosed();
}

void QtAntdSelectPrivate::updateSelectedDisplay()
{
    if (selectedValues.isEmpty()) {
        displayText = placeholder;
        return;
    }
    
    QStringList labels;
    for (const QVariant &value : selectedValues) {
        for (const QtAntdSelectOption &option : options) {
            if (option.value == value) {
                labels.append(option.label);
                break;
            }
        }
    }
    
    if (multiple && labels.size() > 1) {
        displayText = QString("%1 items selected").arg(labels.size());
    } else if (!labels.isEmpty()) {
        displayText = labels.first();
    } else {
        displayText = placeholder;
    }
    
    q_ptr->update();
}

void QtAntdSelectPrivate::clearSelection()
{
    if (!selectedValues.isEmpty()) {
        selectedValues.clear();
        updateSelectedDisplay();
        emit q_ptr->selectionChanged();
        if (dropdown) {
            dropdown->setSelectedValues(selectedValues);
        }
    }
}

bool QtAntdSelectPrivate::isPointInArrow(const QPoint &pos) const
{
    return getArrowRect().contains(pos);
}

bool QtAntdSelectPrivate::isPointInClearButton(const QPoint &pos) const
{
    QRect clearRect = getClearButtonRect();
    return !clearRect.isEmpty() && clearRect.contains(pos);
}

void QtAntdSelectPrivate::startLoadingAnimation()
{
    if (loadingTimer && !loadingTimer->isActive()) {
        loadingAngle = 0;
        loadingTimer->start(gLoadingSpinnerSpeed);
    }
}

void QtAntdSelectPrivate::stopLoadingAnimation()
{
    if (loadingTimer) {
        loadingTimer->stop();
    }
}

void QtAntdSelectPrivate::drawLoadingSpinner(QPainter *painter, const QRect &rect, const QColor &color)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    
    QPoint center = rect.center();
    int radius = qMin(rect.width(), rect.height()) / 2 - 2;
    
    // Draw spinner arcs
    for (int i = 0; i < 8; ++i) {
        int alpha = 255 - (i * 32);
        QColor arcColor = color;
        arcColor.setAlpha(qMax(alpha, 50));
        
        painter->setPen(QPen(arcColor, 2, Qt::SolidLine, Qt::RoundCap));
        
        int angle = loadingAngle + (i * 45);
        int startAngle = angle * 16;
        int spanAngle = 30 * 16;
        
        QRect arcRect(center.x() - radius, center.y() - radius, radius * 2, radius * 2);
        painter->drawArc(arcRect, startAngle, spanAngle);
    }
    
    painter->restore();
}

void QtAntdSelectPrivate::renderSelectedTags(QPainter *painter, const QRect &contentRect)
{
    if (!multiple || selectedValues.isEmpty()) {
        return;
    }
    
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    
    QFont font = painter->font();
    font.setPixelSize(fontSize - 2);
    painter->setFont(font);
    
    QFontMetrics fm(font);
    int x = contentRect.left();
    int y = contentRect.center().y() - gTagHeight / 2;
    int maxWidth = contentRect.width() - 20; // Leave space for "..." if needed
    
    // Render tags for selected values
    int renderedCount = 0;
    for (const QVariant &value : selectedValues) {
        QString label;
        for (const QtAntdSelectOption &option : options) {
            if (option.value == value) {
                label = option.label;
                break;
            }
        }
        
        if (label.isEmpty()) continue;
        
        int tagWidth = fm.horizontalAdvance(label) + gTagPadding * 2;
        if (x + tagWidth > contentRect.right() - 20) {
            // Draw "..." if there are more tags that don't fit
            if (renderedCount < selectedValues.size()) {
                painter->setPen(getTextColor());
                painter->drawText(x, y + gTagHeight / 2 + fm.ascent() / 2, "...");
            }
            break;
        }
        
        // Draw tag background
        QColor tagBg = QColor(245, 245, 245);
        painter->fillRect(x, y, tagWidth, gTagHeight, tagBg);
        painter->setPen(QPen(QColor(217, 217, 217), 1));
        painter->drawRoundedRect(x, y, tagWidth, gTagHeight, gTagRadius, gTagRadius);
        
        // Draw tag text
        painter->setPen(getTextColor());
        QRect textRect(x + gTagPadding, y, tagWidth - gTagPadding * 2, gTagHeight);
        painter->drawText(textRect, Qt::AlignCenter, label);
        
        x += tagWidth + gTagSpacing;
        renderedCount++;
    }
    
    painter->restore();
}

QSize QtAntdSelectPrivate::calculateTagSize(const QString &text) const
{
    QFont font = q_ptr->font();
    font.setPixelSize(fontSize - 2);
    QFontMetrics fm(font);
    int width = fm.horizontalAdvance(text) + gTagPadding * 2;
    return QSize(width, gTagHeight);
}

// QtAntdSelect Implementation
QtAntdSelect::QtAntdSelect(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new QtAntdSelectPrivate(this))
{
    Q_D(QtAntdSelect);
    d->init();
    
    setFocusPolicy(Qt::TabFocus);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setAttribute(Qt::WA_Hover);
}

QtAntdSelect::QtAntdSelect(const QString &placeholder, QWidget *parent)
    : QtAntdSelect(parent)
{
    setPlaceholder(placeholder);
}

QtAntdSelect::~QtAntdSelect()
{
}

void QtAntdSelect::setUseThemeColors(bool value)
{
    Q_D(QtAntdSelect);
    if (d->useThemeColors != value) {
        d->useThemeColors = value;
        update();
    }
}

bool QtAntdSelect::useThemeColors() const
{
    Q_D(const QtAntdSelect);
    return d->useThemeColors;
}

void QtAntdSelect::setSelectSize(SelectSize size)
{
    Q_D(QtAntdSelect);
    if (d->selectSize != size) {
        d->selectSize = size;
        d->updateGeometry();
        update();
    }
}

QtAntdSelect::SelectSize QtAntdSelect::selectSize() const
{
    Q_D(const QtAntdSelect);
    return d->selectSize;
}

void QtAntdSelect::setPlaceholder(const QString &text)
{
    Q_D(QtAntdSelect);
    if (d->placeholder != text) {
        d->placeholder = text;
        d->updateSelectedDisplay();
        update();
    }
}

QString QtAntdSelect::placeholder() const
{
    Q_D(const QtAntdSelect);
    return d->placeholder;
}

void QtAntdSelect::setAllowClear(bool allow)
{
    Q_D(QtAntdSelect);
    if (d->allowClear != allow) {
        d->allowClear = allow;
        update();
    }
}

bool QtAntdSelect::allowClear() const
{
    Q_D(const QtAntdSelect);
    return d->allowClear;
}

void QtAntdSelect::setDisabled(bool disabled)
{
    Q_D(QtAntdSelect);
    if (d->disabled != disabled) {
        d->disabled = disabled;
        setEnabled(!disabled);
        if (disabled && d->dropdownOpen) {
            d->closeDropdown();
        }
        update();
    }
}

bool QtAntdSelect::isDisabled() const
{
    Q_D(const QtAntdSelect);
    return d->disabled;
}

void QtAntdSelect::setLoading(bool loading)
{
    Q_D(QtAntdSelect);
    if (d->isLoading != loading) {
        d->isLoading = loading;
        if (loading) {
            d->startLoadingAnimation();
            if (d->dropdownOpen) {
                d->closeDropdown();
            }
        } else {
            d->stopLoadingAnimation();
        }
        update();
    }
}

bool QtAntdSelect::isLoading() const
{
    Q_D(const QtAntdSelect);
    return d->isLoading;
}

void QtAntdSelect::setShowSearch(bool show)
{
    Q_D(QtAntdSelect);
    if (d->showSearch != show) {
        d->showSearch = show;
        if (d->dropdown) {
            d->dropdown->setSearchEnabled(show);
        }
    }
}

bool QtAntdSelect::showSearch() const
{
    Q_D(const QtAntdSelect);
    return d->showSearch;
}

void QtAntdSelect::setMultiple(bool multiple)
{
    Q_D(QtAntdSelect);
    if (d->multiple != multiple) {
        d->multiple = multiple;
        if (!multiple && d->selectedValues.size() > 1) {
            // Keep only first selection for single mode
            QVariant firstValue = d->selectedValues.first();
            d->selectedValues.clear();
            d->selectedValues.append(firstValue);
            d->updateSelectedDisplay();
            emit selectionChanged();
        }
        if (d->dropdown) {
            d->dropdown->setMultipleSelection(multiple);
        }
        update();
    }
}

bool QtAntdSelect::isMultiple() const
{
    Q_D(const QtAntdSelect);
    return d->multiple;
}

void QtAntdSelect::addOption(const QtAntdSelectOption &option)
{
    Q_D(QtAntdSelect);
    d->options.append(option);
    if (d->dropdown) {
        d->dropdown->setOptions(d->options);
    }
}

void QtAntdSelect::addOption(const QString &label, const QVariant &value)
{
    addOption(QtAntdSelectOption(label, value.isValid() ? value : QVariant(label)));
}

void QtAntdSelect::addOptions(const QList<QtAntdSelectOption> &options)
{
    Q_D(QtAntdSelect);
    d->options.append(options);
    if (d->dropdown) {
        d->dropdown->setOptions(d->options);
    }
}

void QtAntdSelect::removeOption(const QVariant &value)
{
    Q_D(QtAntdSelect);
    auto it = std::remove_if(d->options.begin(), d->options.end(),
                           [&value](const QtAntdSelectOption &option) {
                               return option.value == value;
                           });
    if (it != d->options.end()) {
        d->options.erase(it, d->options.end());
        
        // Remove from selection if selected
        if (d->selectedValues.contains(value)) {
            d->selectedValues.removeAll(value);
            d->updateSelectedDisplay();
            emit selectionChanged();
        }
        
        if (d->dropdown) {
            d->dropdown->setOptions(d->options);
        }
    }
}

void QtAntdSelect::clearOptions()
{
    Q_D(QtAntdSelect);
    d->options.clear();
    d->selectedValues.clear();
    d->updateSelectedDisplay();
    if (d->dropdown) {
        d->dropdown->setOptions(d->options);
    }
    emit selectionChanged();
}

QList<QtAntdSelectOption> QtAntdSelect::options() const
{
    Q_D(const QtAntdSelect);
    return d->options;
}

void QtAntdSelect::setValue(const QVariant &value)
{
    Q_D(QtAntdSelect);
    if (d->multiple) {
        setValues(QVariantList() << value);
    } else {
        d->selectedValues.clear();
        if (value.isValid()) {
            d->selectedValues.append(value);
        }
        d->updateSelectedDisplay();
        if (d->dropdown) {
            d->dropdown->setSelectedValues(d->selectedValues);
        }
        emit selectionChanged();
        if (value.isValid()) {
            emit valueSelected(value);
        }
    }
}

void QtAntdSelect::setValues(const QVariantList &values)
{
    Q_D(QtAntdSelect);
    bool changed = (d->selectedValues != values);
    d->selectedValues = values;
    
    if (!d->multiple && d->selectedValues.size() > 1) {
        // Keep only first value for single selection
        QVariant firstValue = d->selectedValues.first();
        d->selectedValues.clear();
        d->selectedValues.append(firstValue);
    }
    
    d->updateSelectedDisplay();
    if (d->dropdown) {
        d->dropdown->setSelectedValues(d->selectedValues);
    }
    
    if (changed) {
        emit selectionChanged();
        for (const QVariant &value : values) {
            emit valueSelected(value);
        }
    }
}

QVariant QtAntdSelect::value() const
{
    Q_D(const QtAntdSelect);
    return d->selectedValues.isEmpty() ? QVariant() : d->selectedValues.first();
}

QVariantList QtAntdSelect::values() const
{
    Q_D(const QtAntdSelect);
    return d->selectedValues;
}

QString QtAntdSelect::selectedText() const
{
    Q_D(const QtAntdSelect);
    if (d->selectedValues.isEmpty()) {
        return QString();
    }
    
    QVariant value = d->selectedValues.first();
    for (const QtAntdSelectOption &option : d->options) {
        if (option.value == value) {
            return option.label;
        }
    }
    return QString();
}

QStringList QtAntdSelect::selectedTexts() const
{
    Q_D(const QtAntdSelect);
    QStringList texts;
    for (const QVariant &value : d->selectedValues) {
        for (const QtAntdSelectOption &option : d->options) {
            if (option.value == value) {
                texts.append(option.label);
                break;
            }
        }
    }
    return texts;
}

void QtAntdSelect::clearSelection()
{
    Q_D(QtAntdSelect);
    d->clearSelection();
}

QSize QtAntdSelect::sizeHint() const
{
    Q_D(const QtAntdSelect);
    QFontMetrics fm(font());
    int textWidth = fm.horizontalAdvance(d->displayText.isEmpty() ? d->placeholder : d->displayText);
    int width = textWidth + d->padding * 2 + d->iconSize + d->padding; // text + arrow
    if (d->allowClear && !d->selectedValues.isEmpty()) {
        width += d->iconSize + d->padding; // clear button
    }
    return QSize(qMax(width, 120), d->selectHeight);
}

QSize QtAntdSelect::minimumSizeHint() const
{
    Q_D(const QtAntdSelect);
    return QSize(80, d->selectHeight);
}

void QtAntdSelect::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    Q_D(QtAntdSelect);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QRect rect = this->rect();
    
    // Draw background
    QColor bgColor = d->getBackgroundColor();
    painter.fillRect(rect, bgColor);
    
    // Draw border
    QColor borderColor = d->getBorderColor();
    painter.setPen(QPen(borderColor, gBorderWidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(rect.adjusted(0, 0, -1, -1), d->borderRadius, d->borderRadius);
    
    // Draw focus glow effect (similar to Input component)
    if (d->hasFocus || d->dropdownOpen) {
        QColor glowColor = borderColor;
        glowColor.setAlpha(30);
        for (int i = 1; i <= 3; ++i) {
            painter.setPen(QPen(glowColor, 1));
            painter.drawRoundedRect(rect.adjusted(-i, -i, i, i), d->borderRadius + i, d->borderRadius + i);
            glowColor.setAlpha(glowColor.alpha() / 2);
        }
    }
    
    // Draw content
    QRect contentRect = d->getContentRect();
    
    painter.save();
    painter.setClipRect(contentRect);
    
    if (d->multiple && d->selectedValues.size() > 1) {
        // Render tags for multiple selection
        d->renderSelectedTags(&painter, contentRect);
    } else {
        // Draw text (selection or placeholder)
        painter.setPen(d->selectedValues.isEmpty() ? d->getPlaceholderColor() : d->getTextColor());
        
        QFont font = painter.font();
        font.setPixelSize(d->fontSize);
        painter.setFont(font);
        
        QString text = d->displayText.isEmpty() ? d->placeholder : d->displayText;
        painter.drawText(contentRect, Qt::AlignLeft | Qt::AlignVCenter, text);
    }
    
    painter.restore();
    
    // Draw arrow icon
    QRect arrowRect = d->getArrowRect();
    painter.setPen(d->getTextColor());
    
    if (d->isLoading) {
        // Draw loading spinner
        d->drawLoadingSpinner(&painter, arrowRect, d->getTextColor());
    } else {
        // Draw dropdown arrow
        QPolygon arrow;
        int arrowSize = 4;
        QPoint center = arrowRect.center();
        
        if (d->dropdownOpen) {
            // Up arrow
            arrow << QPoint(center.x(), center.y() - arrowSize / 2)
                  << QPoint(center.x() - arrowSize, center.y() + arrowSize / 2)
                  << QPoint(center.x() + arrowSize, center.y() + arrowSize / 2);
        } else {
            // Down arrow  
            arrow << QPoint(center.x(), center.y() + arrowSize / 2)
                  << QPoint(center.x() - arrowSize, center.y() - arrowSize / 2)
                  << QPoint(center.x() + arrowSize, center.y() - arrowSize / 2);
        }
        
        painter.setBrush(d->getTextColor());
        painter.setPen(Qt::NoPen);
        painter.drawPolygon(arrow);
    }
    
    // Draw clear button
    if (d->allowClear && !d->selectedValues.isEmpty() && !d->isLoading) {
        QRect clearRect = d->getClearButtonRect();
        if (!clearRect.isEmpty()) {
            painter.setPen(QPen(QColor(191, 191, 191), 1));
            painter.setBrush(Qt::NoBrush);
            
            // Draw X icon
            int margin = 4;
            QRect iconRect = clearRect.adjusted(margin, margin, -margin, -margin);
            painter.drawLine(iconRect.topLeft(), iconRect.bottomRight());
            painter.drawLine(iconRect.topRight(), iconRect.bottomLeft());
        }
    }
}

void QtAntdSelect::mousePressEvent(QMouseEvent *event)
{
    Q_D(QtAntdSelect);
    if (event->button() == Qt::LeftButton && !d->disabled && !d->isLoading) {
        QPoint pos = event->pos();
        
        if (d->isPointInClearButton(pos)) {
            // Clear selection
            d->clearSelection();
            event->accept();
            return;
        }
        
        d->isPressed = true;
        update();
    }
    QWidget::mousePressEvent(event);
}

void QtAntdSelect::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QtAntdSelect);
    if (event->button() == Qt::LeftButton && d->isPressed) {
        d->isPressed = false;
        
        if (rect().contains(event->pos()) && !d->disabled && !d->isLoading) {
            QPoint pos = event->pos();
            
            // Don't toggle dropdown if clicking clear button
            if (!d->isPointInClearButton(pos)) {
                d->toggleDropdown();
            }
        }
        
        update();
    }
    QWidget::mouseReleaseEvent(event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QtAntdSelect::enterEvent(QEnterEvent *event)
#else
void QtAntdSelect::enterEvent(QEvent *event)
#endif
{
    Q_D(QtAntdSelect);
    d->isHovered = true;
    update();
    QWidget::enterEvent(event);
}

void QtAntdSelect::leaveEvent(QEvent *event)
{
    Q_D(QtAntdSelect);
    d->isHovered = false;
    update();
    QWidget::leaveEvent(event);
}

void QtAntdSelect::focusInEvent(QFocusEvent *event)
{
    Q_D(QtAntdSelect);
    d->hasFocus = true;
    update();
    QWidget::focusInEvent(event);
}

void QtAntdSelect::focusOutEvent(QFocusEvent *event)
{
    Q_D(QtAntdSelect);
    d->hasFocus = false;
    if (d->dropdownOpen) {
        d->closeDropdown();
    }
    update();
    QWidget::focusOutEvent(event);
}

void QtAntdSelect::keyPressEvent(QKeyEvent *event)
{
    Q_D(QtAntdSelect);
    
    switch (event->key()) {
    case Qt::Key_Space:
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (!d->disabled && !d->isLoading) {
            d->toggleDropdown();
            event->accept();
            return;
        }
        break;
    case Qt::Key_Escape:
        if (d->dropdownOpen) {
            d->closeDropdown();
            event->accept();
            return;
        }
        break;
    case Qt::Key_Up:
    case Qt::Key_Down:
        if (!d->dropdownOpen && !d->disabled && !d->isLoading) {
            d->openDropdown();
            event->accept();
            return;
        }
        break;
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        if (d->allowClear && !d->selectedValues.isEmpty()) {
            d->clearSelection();
            event->accept();
            return;
        }
        break;
    }
    
    QWidget::keyPressEvent(event);
}

void QtAntdSelect::resizeEvent(QResizeEvent *event)
{
    Q_D(QtAntdSelect);
    if (d->dropdown && d->dropdownOpen) {
        d->dropdown->updatePosition();
    }
    QWidget::resizeEvent(event);
}