#include "qtantdbutton.h"
#include "qtantdbutton_p.h"
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
    
    // Initial color update
    updateColors();
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
void QtAntdButtonPrivate::updateColors()
{
    if (!useThemeColors) {
        return;
    }

    backgroundColor = getBackgroundColor();
    borderColor = getBorderColor();
    textColor = getTextColor();
    hoverBackgroundColor = getHoverBackgroundColor();
    hoverBorderColor = getHoverBorderColor();
    hoverTextColor = getHoverTextColor();
    pressedBackgroundColor = getPressedBackgroundColor();
    pressedBorderColor = getPressedBorderColor();
    pressedTextColor = getPressedTextColor();
}

/*!
 * \internal
 */
QColor QtAntdButtonPrivate::getBackgroundColor() const
{
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q_ptr->isEnabled()) {
        return style.themeColor("disabled-background");
    }

    switch (buttonType) {
        case QtAntdButton::Primary:
            return isDanger ? style.themeColor("error") : style.themeColor("primary");
        case QtAntdButton::Default:
            return isGhost ? QColor(255, 255, 255, 0) : style.themeColor("background");
        case QtAntdButton::Dashed:
            return style.themeColor("background");
        case QtAntdButton::Text:
        case QtAntdButton::Link:
            return QColor(255, 255, 255, 0); // Transparent
    }
    return style.themeColor("background");
}

/*!
 * \internal
 */
QColor QtAntdButtonPrivate::getBorderColor() const
{
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q_ptr->isEnabled()) {
        return style.themeColor("border");
    }

    switch (buttonType) {
        case QtAntdButton::Primary:
            return isDanger ? style.themeColor("error") : style.themeColor("primary");
        case QtAntdButton::Default:
            return style.themeColor("border");
        case QtAntdButton::Dashed:
            return style.themeColor("border");
        case QtAntdButton::Text:
        case QtAntdButton::Link:
            return QColor(255, 255, 255, 0); // Transparent
    }
    return style.themeColor("border");
}

/*!
 * \internal
 */
QColor QtAntdButtonPrivate::getTextColor() const
{
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q_ptr->isEnabled()) {
        return style.themeColor("disabled");
    }

    switch (buttonType) {
        case QtAntdButton::Primary:
            return QColor(255, 255, 255); // White text on colored background
        case QtAntdButton::Default:
        case QtAntdButton::Dashed:
            return style.themeColor("text");
        case QtAntdButton::Text:
            return isDanger ? style.themeColor("error") : style.themeColor("text");
        case QtAntdButton::Link:
            return isDanger ? style.themeColor("error") : style.themeColor("primary");
    }
    return style.themeColor("text");
}

/*!
 * \internal
 */
QColor QtAntdButtonPrivate::getHoverBackgroundColor() const
{
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q_ptr->isEnabled()) {
        return getBackgroundColor();
    }

    switch (buttonType) {
        case QtAntdButton::Primary:
            return isDanger ? style.themeColor("error-hover") : style.themeColor("primary-hover");
        case QtAntdButton::Default:
        case QtAntdButton::Dashed:
            return isDanger ? style.themeColor("error-background") : style.themeColor("primary-background");
        case QtAntdButton::Text:
            return style.themeColor("surface");
        case QtAntdButton::Link:
            return QColor(255, 255, 255, 0); // Transparent
    }
    return getBackgroundColor();
}

/*!
 * \internal
 */
QColor QtAntdButtonPrivate::getHoverBorderColor() const
{
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q_ptr->isEnabled()) {
        return getBorderColor();
    }

    switch (buttonType) {
        case QtAntdButton::Primary:
            return isDanger ? style.themeColor("error-hover") : style.themeColor("primary-hover");
        case QtAntdButton::Default:
        case QtAntdButton::Dashed:
            return isDanger ? style.themeColor("error") : style.themeColor("primary");
        case QtAntdButton::Text:
        case QtAntdButton::Link:
            return QColor(255, 255, 255, 0); // Transparent
    }
    return getBorderColor();
}

/*!
 * \internal
 */
QColor QtAntdButtonPrivate::getHoverTextColor() const
{
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q_ptr->isEnabled()) {
        return getTextColor();
    }

    switch (buttonType) {
        case QtAntdButton::Primary:
            return QColor(255, 255, 255); // White text on colored background
        case QtAntdButton::Default:
        case QtAntdButton::Dashed:
            return isDanger ? style.themeColor("error") : style.themeColor("primary");
        case QtAntdButton::Text:
            return isDanger ? style.themeColor("error-hover") : style.themeColor("primary-hover");
        case QtAntdButton::Link:
            return isDanger ? style.themeColor("error-hover") : style.themeColor("primary-hover");
    }
    return getTextColor();
}

/*!
 * \internal
 */
QColor QtAntdButtonPrivate::getPressedBackgroundColor() const
{
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q_ptr->isEnabled()) {
        return getBackgroundColor();
    }

    switch (buttonType) {
        case QtAntdButton::Primary:
            return isDanger ? style.themeColor("error-active") : style.themeColor("primary-active");
        case QtAntdButton::Default:
        case QtAntdButton::Dashed:
            return isDanger ? style.themeColor("error-background") : style.themeColor("primary-background");
        case QtAntdButton::Text:
            return style.themeColor("surface-variant");
        case QtAntdButton::Link:
            return QColor(255, 255, 255, 0); // Transparent
    }
    return getBackgroundColor();
}

/*!
 * \internal
 */
QColor QtAntdButtonPrivate::getPressedBorderColor() const
{
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q_ptr->isEnabled()) {
        return getBorderColor();
    }

    switch (buttonType) {
        case QtAntdButton::Primary:
            return isDanger ? style.themeColor("error-active") : style.themeColor("primary-active");
        case QtAntdButton::Default:
        case QtAntdButton::Dashed:
            return isDanger ? style.themeColor("error") : style.themeColor("primary");
        case QtAntdButton::Text:
        case QtAntdButton::Link:
            return QColor(255, 255, 255, 0); // Transparent
    }
    return getBorderColor();
}

/*!
 * \internal
 */
QColor QtAntdButtonPrivate::getPressedTextColor() const
{
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q_ptr->isEnabled()) {
        return getTextColor();
    }

    switch (buttonType) {
        case QtAntdButton::Primary:
            return QColor(255, 255, 255); // White text on colored background
        case QtAntdButton::Default:
        case QtAntdButton::Dashed:
            return isDanger ? style.themeColor("error") : style.themeColor("primary");
        case QtAntdButton::Text:
            return isDanger ? style.themeColor("error-active") : style.themeColor("primary-active");
        case QtAntdButton::Link:
            return isDanger ? style.themeColor("error-active") : style.themeColor("primary-active");
    }
    return getTextColor();
}

/*!
 * \internal
 */
void QtAntdButtonPrivate::startLoadingAnimation()
{
    if (!loadingTimer->isActive()) {
        loadingAngle = 0;
        loadingTimer->start(30); // Update every 30ms for faster animation
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
    pen.setWidth(1.5); // Slightly thicker line for better visibility with shorter arc
    pen.setCapStyle(Qt::RoundCap); // Round ends for smoother look
    
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);
    
    int spinnerSize = getSpinnerSize();
    
    // Get the center point of the provided rectangle
    QPoint center = rect.center();
    
    // Draw a simple rotating circle centered exactly at the provided center point
    painter->translate(center);
    painter->rotate(loadingAngle);
    
    // Draw circle with a gap to indicate rotation
    // Using a slightly smaller size than spinnerSize to keep it proportional
    int drawSize = spinnerSize - 2;
    int arcLength = 72; // Approximately 1/5 of a circle (360/5)
    int startAngle = 0; // Start at the top
    
    painter->drawArc(QRect(-drawSize/2, -drawSize/2, drawSize, drawSize), 
                    startAngle * 16, // Qt uses 1/16 of a degree units
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
        if (value) {
            d->updateColors();
        }
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
        d->updateColors();
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
        d->updateColors();
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
        d->updateColors();
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
            // setEnabled(false); // Disable button while loading
        } else {
            d->stopLoadingAnimation();
            // setEnabled(true);
        }
        
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
    
    // Base size from QPushButton
    QSize size = QPushButton::sizeHint();
    
    // Adjust based on button size
    int extraHeight = 0;
    int extraWidth = 0;
    
    switch (d->buttonSize) {
        case Small:
            extraHeight = -6; // Reduced negative height adjustment
            extraWidth = -12; // Reduced negative width adjustment
            break;
        case Medium:
            // Default size
            break;
        case Large:
            extraHeight = 8;
            extraWidth = 16;
            break;
    }
    
    // Calculate minimum size needed for text content
    int textWidth = 0;
    int horizontalPadding = 16; // Minimum horizontal padding
    
    if (!text().isEmpty()) {
        textWidth = fontMetrics().horizontalAdvance(text());
    }
    
    // Account for icon if present
    if (!icon().isNull() && !d->isLoading) {
        textWidth += iconSize().width();
        if (!text().isEmpty()) {
            textWidth += 8; // Spacing between icon and text
        }
    }
    
    // Account for spinner if loading
    if (d->isLoading) {
        textWidth += d->getSpinnerSize() + 8; // Spinner + spacing
    }
    
    // Ensure width is sufficient for content plus padding
    int minContentWidth = textWidth + horizontalPadding;
    
    size.setHeight(qMax(size.height() + extraHeight, 24));
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
    
    // Determine current colors based on state
    QColor currentBgColor = d->backgroundColor;
    QColor currentBorderColor = d->borderColor;
    QColor currentTextColor = d->textColor;
    
    if (d->isPressed) {
        currentBgColor = d->pressedBackgroundColor;
        currentBorderColor = d->pressedBorderColor;
        currentTextColor = d->pressedTextColor;
    } else if (d->isHovered) {
        currentBgColor = d->hoverBackgroundColor;
        currentBorderColor = d->hoverBorderColor;
        currentTextColor = d->hoverTextColor;
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
    
    // Draw loading spinner if in loading state
    if (d->isLoading) {
        // Create a rect that ensures the spinner is vertically centered with the text
        QRect spinnerRect(
            contentX,
            textRect.center().y() - spinnerSize / 2,
            spinnerSize,
            spinnerSize
        );
        d->drawLoadingSpinner(&painter, spinnerRect, currentTextColor);
        contentX += spinnerSize + spinnerSpacing;
    }
    
    // Draw text
    if (!option.text.isEmpty()) {
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