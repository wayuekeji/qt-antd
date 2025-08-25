#include "qtantdbutton.h"
#include "qtantdbutton_p.h"
#include "qtantdtheme.h"

#include "../include/qtantd/lib/qtantdstyle.h"
#include <QPainter>
#include <QFontMetrics>
#include <QApplication>
#include <QMouseEvent>
#include <QStyleOptionButton>
#include <QEnterEvent>
#include <QTimer>
#include <QtMath>
#include <QDebug>
#include <functional> // For std::hash

struct ButtonTypeToChooseColor
{
    QtAntdButton::ButtonType type;
    bool isDanger;
    bool isHover;
    bool isPressed;
    bool isGhost;

    bool operator==(const ButtonTypeToChooseColor &other) const {
        return type == other.type &&
               isDanger == other.isDanger &&
               isHover == other.isHover &&
               isPressed == other.isPressed &&
               isGhost == other.isGhost;
    }

};

// Hash function specialization for ButtonTypeToChooseColor
namespace std {
    template<>
    struct hash<ButtonTypeToChooseColor> {
        std::size_t operator()(const ButtonTypeToChooseColor& key) const {
            // Combine the hash of the struct members
            std::size_t h1 = std::hash<int>{}(static_cast<int>(key.type));
            std::size_t h2 = std::hash<bool>{}(key.isDanger);
            std::size_t h3 = std::hash<bool>{}(key.isHover);
            std::size_t h4 = std::hash<bool>{}(key.isPressed);
            std::size_t h5 = std::hash<bool>{}(key.isGhost);
            
            // Simple hash combination
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
        }
    };
}

const std::unordered_map<QtAntdButton::ButtonSize, int> buttonSize2ExtraWidth = {
    { QtAntdButton::Small,  -12 },
    { QtAntdButton::Medium, 0 },
    { QtAntdButton::Large,  16 }
};

const std::unordered_map<QtAntdButton::ButtonSize, int> buttonSize2ExtraHeight = {
    { QtAntdButton::Small,  -6 },
    { QtAntdButton::Medium, 0 },
    { QtAntdButton::Large,  8 }
};

QtAntdStyle &style = QtAntdStyle::instance();
auto theme = new QtAntdTheme;
const std::unordered_map<struct ButtonTypeToChooseColor, QColor> button2BackgroundColor = {
    { { QtAntdButton::Primary, false, false, false, false }, style.themeColor("primary") }, // primary, default
    { { QtAntdButton::Primary, false, true, false, false }, style.themeColor("primary-hover") }, // hover
    { { QtAntdButton::Primary, false, false, true, false }, style.themeColor("primary-active") }, // press
    { { QtAntdButton::Primary, true, false, false, false }, style.themeColor("error") }, // danger
    { { QtAntdButton::Primary, true, true, false, false }, style.themeColor("error-hover") }, // danger & hover
    { { QtAntdButton::Primary, true, false, true, false }, style.themeColor("error-active") }, // danger & press

    { { QtAntdButton::Default, false, false, false, false }, style.themeColor("background") }, // default, default
    { { QtAntdButton::Default, true, false, false, false }, QColor(255,255,255,0) },

    { { QtAntdButton::Dashed, false, false, false, false }, style.themeColor("background")},
    { { QtAntdButton::Dashed, true, false, false, false }, style.themeColor("background") },

    { { QtAntdButton::Text, false, false, false, false }, QColor(255, 255, 255, 0) }, // text, default
    { { QtAntdButton::Text, false, true, false, false }, theme->getColor(Antd::Gray3) }, // hover
    { { QtAntdButton::Text, false, false, true, false }, theme->getColor(Antd::Gray5) }, // pressed

    { { QtAntdButton::Text, true, false, false, false }, QColor(255, 255, 255, 0) }, // text, danger
    { { QtAntdButton::Text, true, true, false, false }, QColor("#fff2f0")}, // danger, hover
    { { QtAntdButton::Text, true, false, true, false }, QColor("#ffccc7") }, // danger, pressed

    { { QtAntdButton::Link, false, false, false, false }, QColor(255, 255, 255, 0) },  // link, default
    { { QtAntdButton::Link, false, true, false, false }, QColor(255, 255, 255, 0) },  // hover
    { { QtAntdButton::Link, false, false, true, false }, QColor(255, 255, 255, 0) },  // press
    { { QtAntdButton::Link, true, false, false, false }, QColor(255, 255, 255, 0) },  // link, danger
    { { QtAntdButton::Link, true, true, false, false }, QColor(255, 255, 255, 0) },  // danger, hover
    { { QtAntdButton::Link, true, false, true, false }, QColor(255, 255, 255, 0) }  // danger, press
};

const std::unordered_map<struct ButtonTypeToChooseColor, QColor> button2BorderColor = {
    { { QtAntdButton::Primary, false, false, false, false }, style.themeColor("primary") }, // primary, default
    { { QtAntdButton::Primary, false, true, false, false }, style.themeColor("primary") }, // hover
    { { QtAntdButton::Primary, false, false, true, false }, style.themeColor("primary") }, // press

    { { QtAntdButton::Default, false, false, false, false }, style.themeColor("border") }, // default, default
    { { QtAntdButton::Default, false, true, false, false }, style.themeColor("primary-hover") }, // hover
    { { QtAntdButton::Default, false, false, true, false }, style.themeColor("primary-active") }, // press
    { { QtAntdButton::Default, true, false, false, false }, style.themeColor("error") }, // default, danger
    { { QtAntdButton::Default, true, true, false, false }, style.themeColor("error-hover") }, // danger, hover
    { { QtAntdButton::Default, true, false, true, false }, style.themeColor("error-active") }, // danger, press

    { { QtAntdButton::Dashed, false, false, false, false }, style.themeColor("border") }, // default, default
    { { QtAntdButton::Dashed, false, true, false, false }, style.themeColor("primary-hover") }, // hover
    { { QtAntdButton::Dashed, false, false, true, false }, style.themeColor("primary-active") }, // press
    { { QtAntdButton::Dashed, true, false, false, false }, style.themeColor("error") }, // default, danger
    { { QtAntdButton::Dashed, true, true, false, false }, style.themeColor("error-hover") }, // danger, hover
    { { QtAntdButton::Dashed, true, false, true, false }, style.themeColor("error-active") }, // danger, press

    { { QtAntdButton::Text, false, false, false, false }, QColor(255, 255, 255, 0) }, // text, default
    { { QtAntdButton::Text, false, true, false, false }, QColor(255, 255, 255, 0) }, // hover
    { { QtAntdButton::Text, false, false, true, false }, QColor(255, 255, 255, 0) }, // press
    { { QtAntdButton::Text, true, false, false, false }, QColor(255, 255, 255, 0) }, // text, danger
    { { QtAntdButton::Text, true, true, false, false }, QColor(255, 255, 255, 0) }, // danger, hover
    { { QtAntdButton::Text, true, false, true, false }, QColor(255, 255, 255, 0) }, // danger, press

    { { QtAntdButton::Link, false, false, false, false }, QColor(255, 255, 255, 0) },  // link, default
    { { QtAntdButton::Link, false, true, false, false }, QColor(255, 255, 255, 0) },  // hover
    { { QtAntdButton::Link, false, false, true, false }, QColor(255, 255, 255, 0) },  // press
    { { QtAntdButton::Link, true, false, false, false }, QColor(255, 255, 255, 0) },  // link, danger
    { { QtAntdButton::Link, true, true, false, false }, QColor(255, 255, 255, 0) },  // danger, hover
    { { QtAntdButton::Link, true, false, true, false }, QColor(255, 255, 255, 0) }  // danger, press
};


//"d9363e"
const std::unordered_map<struct ButtonTypeToChooseColor, QColor> button2TextColor = {
    { { QtAntdButton::Primary, false, false, false, false }, Qt::white }, // primary, default
    { { QtAntdButton::Primary, false, true, false, false }, Qt::white }, // primary, hover
    { { QtAntdButton::Primary, false, false, true, false }, Qt::white }, // primary, press

    { { QtAntdButton::Primary, true, false, false, false }, Qt::white}, // primary, danger
    { { QtAntdButton::Primary, true, true, false, false }, Qt::white }, // primary, danger, hover
    { { QtAntdButton::Primary, true, false, true, false }, Qt::white }, // primary, danger, press


    { { QtAntdButton::Default, false, false, false, false }, style.themeColor("text") }, // default, default
    { { QtAntdButton::Default, false, true, false, false }, style.themeColor("primary-hover") }, // default, hover
    { { QtAntdButton::Default, false, false, true, false }, style.themeColor("primary-active") }, // default, press
    { { QtAntdButton::Default, true, false, false, false }, style.themeColor("error") }, // default, danger
    { { QtAntdButton::Default, true, true, false, false }, style.themeColor("error-hover") }, // danger, hover
    { { QtAntdButton::Default, true, false, true, false }, style.themeColor("error-active") }, // danger, press

    { { QtAntdButton::Dashed, false, false, false, false }, style.themeColor("text") }, // default, default
    { { QtAntdButton::Dashed, false, true, false, false }, style.themeColor("primary-hover") }, // default, hover
    { { QtAntdButton::Dashed, false, false, true, false }, style.themeColor("primary-active") }, // default, press
    { { QtAntdButton::Dashed, true, false, false, false }, style.themeColor("error") }, // default, danger
    { { QtAntdButton::Dashed, true, true, false, false }, style.themeColor("error-hover") }, // danger, hover
    { { QtAntdButton::Dashed, true, false, true, false }, style.themeColor("error-active") }, // danger, press

    { { QtAntdButton::Text, false, false, false, false }, style.themeColor("text") },
    { { QtAntdButton::Text, false, true, false, false }, style.themeColor("text") },
    { { QtAntdButton::Text, false, false, true, false }, style.themeColor("text") },

    { { QtAntdButton::Text, true, false, false, false }, style.themeColor("error") },
    { { QtAntdButton::Text, true, true, false, false }, style.themeColor("error") },
    { { QtAntdButton::Text, true, false, true, false }, style.themeColor("error") },

    { { QtAntdButton::Link, false, false, false, false }, style.themeColor("primary") }, // link, default
    { { QtAntdButton::Link, false, true, false, false }, style.themeColor("primary-hover") }, // hover
    { { QtAntdButton::Link, false, false, true, false }, style.themeColor("primary-active") }, // press
    { { QtAntdButton::Link, true, false, false, false }, style.themeColor("error") }, // link, default
    { { QtAntdButton::Link, true, true, false, false }, style.themeColor("error-hover") }, // hover
    { { QtAntdButton::Link, true, false, true, false }, style.themeColor("error-active") } // press
};

const int horizontalPadding = 16;
const int iconTextSpacing = 8;
const int buttonMinimumHeight = 24;
const int timerInterval = 30; // ms

/*!
 * \class QtAntdButtonPrivate
 * \internal
 */

/*!
 * \internal
 */
QtAntdButtonPrivate::QtAntdButtonPrivate(QtAntdButton *q)
    : q_ptr(q)
{
}

/*!
 * \internal
 */
QtAntdButtonPrivate::~QtAntdButtonPrivate()
{
    // Clean up timer
    if (loadingTimer) {
        loadingTimer->stop();
        delete loadingTimer;
        loadingTimer = nullptr;
    }
}

/*!
 * \internal
 */
void QtAntdButtonPrivate::init()
{
    Q_Q(QtAntdButton);

    buttonType = QtAntdButton::Default;
    buttonSize = QtAntdButton::Medium;
    buttonShape = QtAntdButton::DefaultShape;
    useThemeColors = true;
    isBlock = false;
    isGhost = false;
    isDanger = false;
    isLoading = false;
    isHovered = false;
    isPressed = false;
    
    // Initialize loading animation properties
    loadingAngle = 0;
    loadingTimer = new QTimer(q);
    QObject::connect(loadingTimer, &QTimer::timeout, q, [this]() {
        loadingAngle = (loadingAngle + 15) % 360; // Rotate by 15 degrees each time for faster rotation
        q_ptr->update();
    });

    // Set up default size policy
    QSizePolicy policy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    q->setSizePolicy(policy);
}

/*!
 * \internal
 */
void QtAntdButtonPrivate::updateGeometry()
{
    Q_Q(QtAntdButton);
    q->updateGeometry();
}





/*!
 * \internal
 */
void QtAntdButtonPrivate::startLoadingAnimation()
{
    if (!loadingTimer->isActive()) {
        loadingAngle = 0;
        loadingTimer->start(timerInterval);
    }
}

/*!
 * \internal
 */
void QtAntdButtonPrivate::stopLoadingAnimation()
{
    if (loadingTimer->isActive()) {
        loadingTimer->stop();
    }
}

/*!
 * \internal
 */
void QtAntdButtonPrivate::drawLoadingSpinner(QPainter *painter, const QRect &rect, const QColor &color)
{
    // Save the painter state
    painter->save();
    
    // Set pen properties for the spinner
    QPen pen(color);
    pen.setWidth(1.5); // loading spinner line width
    pen.setCapStyle(Qt::RoundCap); // Round ends for smoother look
    
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);
    
    int spinnerSize = getSpinnerSize();
    
    QPoint center = rect.center();
    
    painter->translate(center);
    painter->rotate(loadingAngle);
    
    // Draw circle with a gap to indicate rotation
    // Using a slightly smaller size than spinnerSize to keep it proportional
    int drawSize = spinnerSize - 2;
    int arcLength = 72; // loading spinner arc length (degrees)
    int startAngle = 0; // Start at the top
    
    painter->drawArc(QRect(-drawSize/2, -drawSize/2, drawSize, drawSize), 
                    startAngle * 16,
                    arcLength * 16);
    
    // Restore the painter state
    painter->restore();
}

/*!
 * \class QtAntdButton
 */

QtAntdButton::QtAntdButton(QWidget *parent)
    : QPushButton(parent)
    , d_ptr(new QtAntdButtonPrivate(this))
{
    Q_D(QtAntdButton);
    d->init();
}

QtAntdButton::QtAntdButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
    , d_ptr(new QtAntdButtonPrivate(this))
{
    Q_D(QtAntdButton);
    d->init();
}

QtAntdButton::QtAntdButton(const QIcon &icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent)
    , d_ptr(new QtAntdButtonPrivate(this))
{
    Q_D(QtAntdButton);
    d->init();
}

QtAntdButton::~QtAntdButton()
{
}

void QtAntdButton::setUseThemeColors(bool value)
{
    Q_D(QtAntdButton);
    if (d->useThemeColors != value) {
        d->useThemeColors = value;
        update();
    }
}

bool QtAntdButton::useThemeColors() const
{
    Q_D(const QtAntdButton);
    return d->useThemeColors;
}

void QtAntdButton::setButtonType(ButtonType type)
{
    Q_D(QtAntdButton);
    if (d->buttonType != type) {
        d->buttonType = type;
        update();
    }
}

QtAntdButton::ButtonType QtAntdButton::buttonType() const
{
    Q_D(const QtAntdButton);
    return d->buttonType;
}

void QtAntdButton::setButtonSize(ButtonSize size)
{
    Q_D(QtAntdButton);
    if (d->buttonSize != size) {
        d->buttonSize = size;
        d->updateGeometry();
        update();
    }
}

QtAntdButton::ButtonSize QtAntdButton::buttonSize() const
{
    Q_D(const QtAntdButton);
    return d->buttonSize;
}

void QtAntdButton::setButtonShape(ButtonShape shape)
{
    Q_D(QtAntdButton);
    if (d->buttonShape != shape) {
        d->buttonShape = shape;
        update();
    }
}

QtAntdButton::ButtonShape QtAntdButton::buttonShape() const
{
    Q_D(const QtAntdButton);
    return d->buttonShape;
}

void QtAntdButton::setBlock(bool block)
{
    Q_D(QtAntdButton);
    if (d->isBlock != block) {
        d->isBlock = block;
        QSizePolicy policy = sizePolicy();
        if (block) {
            policy.setHorizontalPolicy(QSizePolicy::Expanding);
        } else {
            policy.setHorizontalPolicy(QSizePolicy::Maximum);
        }
        setSizePolicy(policy);
        d->updateGeometry();
        update();
    }
}

bool QtAntdButton::isBlock() const
{
    Q_D(const QtAntdButton);
    return d->isBlock;
}

void QtAntdButton::setGhost(bool ghost)
{
    Q_D(QtAntdButton);
    if (d->isGhost != ghost) {
        d->isGhost = ghost;
        update();
    }
}

bool QtAntdButton::isGhost() const
{
    Q_D(const QtAntdButton);
    return d->isGhost;
}

void QtAntdButton::setDanger(bool danger)
{
    Q_D(QtAntdButton);
    if (d->isDanger != danger) {
        d->isDanger = danger;
        update();
    }
}

bool QtAntdButton::isDanger() const
{
    Q_D(const QtAntdButton);
    return d->isDanger;
}

void QtAntdButton::setLoading(bool loading)
{
    Q_D(QtAntdButton);
    if (d->isLoading != loading) {
        d->isLoading = loading;
        
        // Start or stop animation
        if (loading) {
            d->startLoadingAnimation();
        } else {
            d->stopLoadingAnimation();
        }
        
        // Update the button geometry since loading state affects size calculation
        d->updateGeometry();
        update();
    }
}

bool QtAntdButton::isLoading() const
{
    Q_D(const QtAntdButton);
    return d->isLoading;
}

QSize QtAntdButton::sizeHint() const
{
    Q_D(const QtAntdButton);
    
    QSize size = QPushButton::sizeHint();
    
    // Adjust based on button size
    int extraHeight = buttonSize2ExtraHeight.at(d->buttonSize);
    int extraWidth = buttonSize2ExtraWidth.at(d->buttonSize);

    int textWidth = 0;
    
    if (!text().isEmpty()) {
        textWidth = fontMetrics().horizontalAdvance(text());
    }
    
    // Account for icon if present
    if (!icon().isNull() && !d->isLoading) {
        textWidth += iconSize().width();
        if (!text().isEmpty()) {
            textWidth += iconTextSpacing;
        }
    }
    
    // Account for spinner if loading
    if (d->isLoading) {
        textWidth += d->getSpinnerSize() + 8;
        if (!text().isEmpty()) {
            textWidth += iconTextSpacing;
        }
    }
    
    // Ensure width is sufficient for content plus padding
    int minContentWidth = textWidth + horizontalPadding;
    
    size.setHeight(qMax(size.height() + extraHeight, buttonMinimumHeight));
    size.setWidth(qMax(size.width() + extraWidth, minContentWidth));
    
    return size;
}

QSize QtAntdButton::minimumSizeHint() const
{
    Q_D(const QtAntdButton);
    
    QSize size = QPushButton::minimumSizeHint();
    
    // Minimum heights for different sizes
    switch (d->buttonSize) {
        case Small:
            size.setHeight(qMax(size.height(), 24));
            
            // Calculate minimum width needed for text content
            if (!text().isEmpty() || !icon().isNull() || d->isLoading) {
                int minWidth = 0;
                int minPadding = 12; // Minimum horizontal padding for Small buttons
                
                if (!text().isEmpty()) {
                    minWidth += fontMetrics().horizontalAdvance(text());
                }
                
                if (!icon().isNull() && !d->isLoading) {
                    minWidth += iconSize().width() + (text().isEmpty() ? 0 : 8);
                }
                
                if (d->isLoading) {
                    minWidth += d->getSpinnerSize() + 8;
                }
                
                size.setWidth(qMax(size.width(), minWidth + minPadding));
            }
            break;
        case Medium:
            size.setHeight(qMax(size.height(), 32));
            break;
        case Large:
            size.setHeight(qMax(size.height(), 40));
            break;
    }
    
    return size;
}

void QtAntdButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    Q_D(QtAntdButton);

    if (!d->useThemeColors) {
        QPushButton::paintEvent(event);
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = this->rect().adjusted(1, 1, -1, -1);
    
    QtAntdStyle &style = QtAntdStyle::instance();
    
    // Function to safely get color from map or use default
    auto getColorFromMap = [&style](
        const std::unordered_map<ButtonTypeToChooseColor, QColor>& colorMap,
        const ButtonTypeToChooseColor& key,
        const QString& defaultColorName) -> QColor {
        
        auto it = colorMap.find(key);
        if (it != colorMap.end()) {
            return it->second;
        }
        return style.themeColor(defaultColorName);
    };
    
    // Get colors based on current state
    QColor currentBgColor;
    QColor currentBorderColor;
    QColor currentTextColor;
    
    if (!isEnabled()) {
        // Disabled state
        currentBgColor = style.themeColor("disabled-background");
        currentBorderColor = style.themeColor("border");
        currentTextColor = style.themeColor("disabled");
    } else {
        // Create color key based on current state
        ButtonTypeToChooseColor colorKey{ 
            d->buttonType, 
            d->isDanger,
            d->isHovered && !d->isPressed, // Only use hover when not pressed
            d->isPressed,
            d->isGhost 
        };
        
        // Get colors from maps
        currentBgColor = getColorFromMap(button2BackgroundColor, colorKey, "background");
        currentBorderColor = getColorFromMap(button2BorderColor, colorKey, "border");
        currentTextColor = getColorFromMap(button2TextColor, colorKey, "text");
    }
    // Draw background
    if (currentBgColor.alpha() > 0) {
        painter.setBrush(QBrush(currentBgColor));
    } else {
        painter.setBrush(Qt::NoBrush);
    }
    
    // Draw border
    QPen pen(currentBorderColor, 1);
    if (d->buttonType == Dashed) {
        pen.setStyle(Qt::DashLine);
    }
    painter.setPen(pen);
    
    // Draw shape
    int radius = 6; // Ant Design default border radius
    if (d->buttonShape == Round) {
        radius = rect.height() / 2;
    } else if (d->buttonShape == Circle) {
        int size = qMin(rect.width(), rect.height());
        rect = QRect(rect.x() + (rect.width() - size) / 2, 
                    rect.y() + (rect.height() - size) / 2, 
                    size, size);
        radius = size / 2;
    }
    
    if (currentBorderColor.alpha() > 0 || currentBgColor.alpha() > 0) {
        painter.drawRoundedRect(rect, radius, radius);
    }
    
    // Draw text and icon
    painter.setPen(currentTextColor);
    
    QStyleOptionButton option;
    option.initFrom(this);
    option.text = text();
    option.icon = d->isLoading ? QIcon() : icon(); // Don't use the icon when loading
    option.iconSize = iconSize();
    
    QRect textRect = rect;
    int contentWidth = 0;
    int spinnerSize = d->getSpinnerSize();
    int spinnerSpacing = 8;
    
    // Determine spinner size based on button size
    if (d->isLoading) {
        contentWidth += spinnerSize + spinnerSpacing;
    }
    
    // Calculate text width if there is text
    if (!option.text.isEmpty()) {
        contentWidth += fontMetrics().horizontalAdvance(option.text);
    }
    
    // Add icon width if present and not in loading state
    if (!option.icon.isNull() && !d->isLoading) {
        contentWidth += option.iconSize.width();
        if (!option.text.isEmpty()) {
            contentWidth += 8; // Spacing between icon and text
        }
    }
    
    // Calculate starting position for content
    int contentX = textRect.left() + (textRect.width() - contentWidth) / 2;
    
    // Ensure minimum padding for Small size
    if (d->buttonSize == Small && contentWidth > 0) {
        int minHPadding = 6; // Minimum horizontal padding for each side
        contentX = qMax(contentX, textRect.left() + minHPadding);
    }
    
    // Store initial content position to ensure proper centering
    int initialContentX = contentX;
    
    // Draw the icon if not in loading state
    if (!option.icon.isNull() && !d->isLoading) {
        QPixmap pixmap = option.icon.pixmap(option.iconSize, isEnabled() ? QIcon::Normal : QIcon::Disabled);
        
        int iconSpacing = 8;
        
        if (!option.text.isEmpty()) {
            // Icon + text
            painter.drawPixmap(contentX, textRect.center().y() - option.iconSize.height() / 2, pixmap);
            contentX += option.iconSize.width() + iconSpacing;
        } else {
            // Icon only
            painter.drawPixmap(textRect.center() - QPoint(option.iconSize.width() / 2, option.iconSize.height() / 2), pixmap);
        }
    }
    
    // In loading state, we need to draw both spinner and text as a single centered unit
    if (d->isLoading) {
        if (!option.text.isEmpty()) {
            // If we have text, calculate everything from the initial position
            contentX = initialContentX;
            
            // Create a rect that ensures the spinner is vertically centered with the text
            QRect spinnerRect(
                contentX,
                textRect.center().y() - spinnerSize / 2,
                spinnerSize,
                spinnerSize
            );
            d->drawLoadingSpinner(&painter, spinnerRect, currentTextColor);
            contentX += spinnerSize + spinnerSpacing;
            
            // Draw text right after spinner
            QRect adjustedTextRect = textRect;
            adjustedTextRect.setLeft(contentX);
            painter.drawText(adjustedTextRect, Qt::AlignLeft | Qt::AlignVCenter, option.text);
        } else {
            // Spinner only - center it in the button
            QRect spinnerRect(
                textRect.center().x() - spinnerSize / 2,
                textRect.center().y() - spinnerSize / 2,
                spinnerSize,
                spinnerSize
            );
            d->drawLoadingSpinner(&painter, spinnerRect, currentTextColor);
        }
    } else if (!option.text.isEmpty()) {
        // Draw text (when not loading)
        QRect adjustedTextRect = textRect;
        adjustedTextRect.setLeft(contentX);
        painter.drawText(adjustedTextRect, Qt::AlignLeft | Qt::AlignVCenter, option.text);
    }


    // Removed commented-out code
}


int QtAntdButtonPrivate::getSpinnerSize() const {
    int spinnerSize = 14; // Default value
    switch (buttonSize) {
        case QtAntdButton::Small:
            spinnerSize = 12;
            break;
        case QtAntdButton::Medium:
            spinnerSize = 14;
            break;
        case QtAntdButton::Large:
            spinnerSize = 16;
            break;
    }
    return spinnerSize;
}

void QtAntdButton::mousePressEvent(QMouseEvent *event)
{
    Q_D(QtAntdButton);
    if (event->button() == Qt::LeftButton) {
        d->isPressed = true;
        update();
    }
    QPushButton::mousePressEvent(event);
}

void QtAntdButton::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QtAntdButton);
    if (event->button() == Qt::LeftButton) {
        d->isPressed = false;
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void QtAntdButton::enterEvent(QEnterEvent *event)
{
    Q_D(QtAntdButton);
    d->isHovered = true;
    update();
    QPushButton::enterEvent(event);
}

void QtAntdButton::leaveEvent(QEvent *event)
{
    Q_D(QtAntdButton);
    d->isHovered = false;
    d->isPressed = false;
    update();
    QPushButton::leaveEvent(event);
}