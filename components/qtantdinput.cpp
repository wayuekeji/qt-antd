#include "qtantdinput.h"
#include "qtantdinput_p.h"
#include "../include/qtantd/lib/qtantdstyle.h"
#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QResizeEvent>
#include <QToolButton>
#include <QHBoxLayout>
#include <QFontMetrics>

// Static constants for Ant Design dimensions
static const int gAntdInputHeightSmall = 24;
static const int gAntdInputHeightMedium = 32;
static const int gAntdInputHeightLarge = 40;

static const int gAntdInputPaddingSmall = 4;
static const int gAntdInputPaddingMedium = 8;
static const int gAntdInputPaddingLarge = 12;

static const int gAntdBorderRadius = 6;
static const int gAntdBorderWidth = 1;

static const int gAntdClearButtonSize = 16;
static const int gAntdIconSpacing = 4;

QtAntdInputPrivate::QtAntdInputPrivate(QtAntdInput *q)
    : q_ptr(q),
      inputSize(QtAntdInput::Medium),
      inputStatus(QtAntdInput::Normal),
      useThemeColors(true),
      clearButtonEnabled(false),
      isHovered(false),
      isFocused(false),
      clearButtonHovered(false),
      clearButtonPressed(false),
      clearButton(nullptr)
{
}

QtAntdInputPrivate::~QtAntdInputPrivate()
{
    delete clearButton;
}

void QtAntdInputPrivate::init()
{
    Q_Q(QtAntdInput);
    
    // Set up the input field
    q->setFrame(false); // We'll draw our own border
    q->setAttribute(Qt::WA_Hover, true);
    
    // Create clear button (initially hidden)
    clearButton = new QToolButton(q);
    clearButton->setCursor(Qt::PointingHandCursor);
    clearButton->setFocusPolicy(Qt::NoFocus);
    clearButton->setText("×"); // Simple × character for now
    clearButton->setVisible(false);
    clearButton->setFixedSize(gAntdClearButtonSize, gAntdClearButtonSize);
    
    // Connect clear button
    QObject::connect(clearButton, &QToolButton::clicked,
                     q, &QtAntdInput::onClearButtonClicked);
    
    updateGeometry();
    updateTextMargins();
}

void QtAntdInputPrivate::updateGeometry()
{
    Q_Q(QtAntdInput);
    
    int height = getInputHeight();
    q->setFixedHeight(height);
    
    updateClearButton();
}

void QtAntdInputPrivate::updateTextMargins()
{
    Q_Q(QtAntdInput);
    
    int leftMargin = getHorizontalPadding();
    int rightMargin = getHorizontalPadding();
    
    // Adjust for prefix
    if (!prefixText.isEmpty() || !prefixIcon.isNull()) {
        QFontMetrics fm(q->font());
        int prefixWidth = prefixText.isEmpty() ? 16 : fm.horizontalAdvance(prefixText);
        leftMargin += prefixWidth + gAntdIconSpacing;
    }
    
    // Adjust for suffix or clear button
    if (!suffixText.isEmpty() || !suffixIcon.isNull()) {
        QFontMetrics fm(q->font());
        int suffixWidth = suffixText.isEmpty() ? 16 : fm.horizontalAdvance(suffixText);
        rightMargin += suffixWidth + gAntdIconSpacing;
    } else if (clearButtonEnabled && !q->text().isEmpty()) {
        rightMargin += gAntdClearButtonSize + gAntdIconSpacing;
    }
    
    q->setTextMargins(leftMargin, 0, rightMargin, 0);
}

void QtAntdInputPrivate::updateClearButton()
{
    Q_Q(QtAntdInput);
    
    if (!clearButton) return;
    
    bool shouldShow = clearButtonEnabled && !q->text().isEmpty() && !q->isReadOnly();
    clearButton->setVisible(shouldShow);
    
    if (shouldShow) {
        QRect buttonRect = clearButtonRect();
        clearButton->move(buttonRect.topLeft());
    }
}

QRect QtAntdInputPrivate::prefixRect() const
{
    Q_Q(const QtAntdInput);
    
    if (prefixText.isEmpty() && prefixIcon.isNull()) {
        return QRect();
    }
    
    int padding = getHorizontalPadding();
    int height = getInputHeight();
    int width = 16; // Default icon size
    
    if (!prefixText.isEmpty()) {
        QFontMetrics fm(q->font());
        width = fm.horizontalAdvance(prefixText);
    }
    
    return QRect(padding, (height - 16) / 2, width, 16);
}

QRect QtAntdInputPrivate::suffixRect() const
{
    Q_Q(const QtAntdInput);
    
    if (suffixText.isEmpty() && suffixIcon.isNull()) {
        return QRect();
    }
    
    int padding = getHorizontalPadding();
    int height = getInputHeight();
    int width = 16; // Default icon size
    
    if (!suffixText.isEmpty()) {
        QFontMetrics fm(q->font());
        width = fm.horizontalAdvance(suffixText);
    }
    
    QRect rect = q->rect();
    return QRect(rect.width() - padding - width, (height - 16) / 2, width, 16);
}

QRect QtAntdInputPrivate::clearButtonRect() const
{
    Q_Q(const QtAntdInput);
    
    int padding = getHorizontalPadding();
    int height = getInputHeight();
    QRect rect = q->rect();
    
    return QRect(rect.width() - padding - gAntdClearButtonSize,
                 (height - gAntdClearButtonSize) / 2,
                 gAntdClearButtonSize,
                 gAntdClearButtonSize);
}

QRect QtAntdInputPrivate::textRect() const
{
    Q_Q(const QtAntdInput);
    
    QRect rect = q->rect();
    int leftMargin = getHorizontalPadding();
    int rightMargin = getHorizontalPadding();
    
    // Account for prefix
    QRect prefixR = prefixRect();
    if (!prefixR.isEmpty()) {
        leftMargin = prefixR.right() + gAntdIconSpacing;
    }
    
    // Account for suffix or clear button
    QRect suffixR = suffixRect();
    if (!suffixR.isEmpty()) {
        rightMargin = rect.width() - suffixR.left() + gAntdIconSpacing;
    } else if (clearButtonEnabled && !q->text().isEmpty()) {
        QRect clearR = clearButtonRect();
        rightMargin = rect.width() - clearR.left() + gAntdIconSpacing;
    }
    
    return QRect(leftMargin, 0, rect.width() - leftMargin - rightMargin, rect.height());
}

int QtAntdInputPrivate::getInputHeight() const
{
    switch (inputSize) {
        case QtAntdInput::Small:  return gAntdInputHeightSmall;
        case QtAntdInput::Large:  return gAntdInputHeightLarge;
        case QtAntdInput::Medium:
        default:                  return gAntdInputHeightMedium;
    }
}

int QtAntdInputPrivate::getHorizontalPadding() const
{
    switch (inputSize) {
        case QtAntdInput::Small:  return gAntdInputPaddingSmall;
        case QtAntdInput::Large:  return gAntdInputPaddingLarge;
        case QtAntdInput::Medium:
        default:                  return gAntdInputPaddingMedium;
    }
}

int QtAntdInputPrivate::getBorderRadius() const
{
    return gAntdBorderRadius;
}

QColor QtAntdInputPrivate::getBorderColor() const
{
    if (!useThemeColors) {
        return QColor("#d9d9d9"); // Default Ant Design border color
    }
    
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (isFocused) {
        switch (inputStatus) {
            case QtAntdInput::Error:   return style.themeColor("error");
            case QtAntdInput::Warning: return style.themeColor("warning");
            case QtAntdInput::Success: return style.themeColor("success");
            case QtAntdInput::Normal:
            default:                   return style.themeColor("primary");
        }
    }
    
    if (isHovered) {
        switch (inputStatus) {
            case QtAntdInput::Error:   return style.themeColor("error");
            case QtAntdInput::Warning: return style.themeColor("warning");
            case QtAntdInput::Success: return style.themeColor("success");
            case QtAntdInput::Normal:
            default:                   return style.themeColor("primary");
        }
    }
    
    // Default/normal state
    switch (inputStatus) {
        case QtAntdInput::Error:   return style.themeColor("error");
        case QtAntdInput::Warning: return style.themeColor("warning"); 
        case QtAntdInput::Success: return style.themeColor("success");
        case QtAntdInput::Normal:
        default:                   return style.themeColor("border");
    }
}

QColor QtAntdInputPrivate::getBackgroundColor() const
{
    Q_Q(const QtAntdInput);
    
    if (!useThemeColors) {
        return q->isEnabled() ? Qt::white : QColor("#f5f5f5");
    }
    
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q->isEnabled()) {
        return style.themeColor("disabled-background");
    }
    
    return style.themeColor("background");
}

QColor QtAntdInputPrivate::getTextColor() const
{
    Q_Q(const QtAntdInput);
    
    if (!useThemeColors) {
        return q->isEnabled() ? Qt::black : QColor("#bfbfbf");
    }
    
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q->isEnabled()) {
        return style.themeColor("disabled");
    }
    
    return style.themeColor("text");
}

// QtAntdInput implementation
QtAntdInput::QtAntdInput(QWidget *parent)
    : QLineEdit(parent), d_ptr(new QtAntdInputPrivate(this))
{
    Q_D(QtAntdInput);
    d->init();
    
    connect(this, &QLineEdit::textChanged, this, &QtAntdInput::onTextChanged);
}

QtAntdInput::QtAntdInput(const QString &placeholder, QWidget *parent)
    : QLineEdit(parent), d_ptr(new QtAntdInputPrivate(this))
{
    Q_D(QtAntdInput);
    d->init();
    setPlaceholderText(placeholder);
    
    connect(this, &QLineEdit::textChanged, this, &QtAntdInput::onTextChanged);
}

QtAntdInput::~QtAntdInput()
{
}

void QtAntdInput::setUseThemeColors(bool value)
{
    Q_D(QtAntdInput);
    if (d->useThemeColors != value) {
        d->useThemeColors = value;
        update();
    }
}

bool QtAntdInput::useThemeColors() const
{
    Q_D(const QtAntdInput);
    return d->useThemeColors;
}

void QtAntdInput::setInputSize(InputSize size)
{
    Q_D(QtAntdInput);
    if (d->inputSize != size) {
        d->inputSize = size;
        d->updateGeometry();
        d->updateTextMargins();
        update();
    }
}

QtAntdInput::InputSize QtAntdInput::inputSize() const
{
    Q_D(const QtAntdInput);
    return d->inputSize;
}

void QtAntdInput::setInputStatus(InputStatus status)
{
    Q_D(QtAntdInput);
    if (d->inputStatus != status) {
        d->inputStatus = status;
        update();
    }
}

QtAntdInput::InputStatus QtAntdInput::inputStatus() const
{
    Q_D(const QtAntdInput);
    return d->inputStatus;
}

void QtAntdInput::setClearButtonEnabled(bool enabled)
{
    Q_D(QtAntdInput);
    if (d->clearButtonEnabled != enabled) {
        d->clearButtonEnabled = enabled;
        d->updateClearButton();
        d->updateTextMargins();
        update();
    }
}

bool QtAntdInput::isClearButtonEnabled() const
{
    Q_D(const QtAntdInput);
    return d->clearButtonEnabled;
}

void QtAntdInput::setPrefixText(const QString &text)
{
    Q_D(QtAntdInput);
    if (d->prefixText != text) {
        d->prefixText = text;
        d->prefixIcon = QIcon(); // Clear icon if text is set
        d->updateTextMargins();
        update();
    }
}

void QtAntdInput::setPrefixIcon(const QIcon &icon)
{
    Q_D(QtAntdInput);
    if (d->prefixIcon.name() != icon.name()) {
        d->prefixIcon = icon;
        d->prefixText.clear(); // Clear text if icon is set
        d->updateTextMargins();
        update();
    }
}

QString QtAntdInput::prefixText() const
{
    Q_D(const QtAntdInput);
    return d->prefixText;
}

QIcon QtAntdInput::prefixIcon() const
{
    Q_D(const QtAntdInput);
    return d->prefixIcon;
}

void QtAntdInput::setSuffixText(const QString &text)
{
    Q_D(QtAntdInput);
    if (d->suffixText != text) {
        d->suffixText = text;
        d->suffixIcon = QIcon(); // Clear icon if text is set
        d->updateTextMargins();
        update();
    }
}

void QtAntdInput::setSuffixIcon(const QIcon &icon)
{
    Q_D(QtAntdInput);
    if (d->suffixIcon.name() != icon.name()) {
        d->suffixIcon = icon;
        d->suffixText.clear(); // Clear text if icon is set
        d->updateTextMargins();
        update();
    }
}

QString QtAntdInput::suffixText() const
{
    Q_D(const QtAntdInput);
    return d->suffixText;
}

QIcon QtAntdInput::suffixIcon() const
{
    Q_D(const QtAntdInput);
    return d->suffixIcon;
}

QSize QtAntdInput::sizeHint() const
{
    Q_D(const QtAntdInput);
    
    QSize size = QLineEdit::sizeHint();
    size.setHeight(d->getInputHeight());
    
    // Add space for prefix/suffix if needed
    if (!d->prefixText.isEmpty() || !d->prefixIcon.isNull()) {
        size.setWidth(size.width() + 16 + gAntdIconSpacing);
    }
    
    if (!d->suffixText.isEmpty() || !d->suffixIcon.isNull()) {
        size.setWidth(size.width() + 16 + gAntdIconSpacing);
    } else if (d->clearButtonEnabled) {
        size.setWidth(size.width() + gAntdClearButtonSize + gAntdIconSpacing);
    }
    
    return size;
}

QSize QtAntdInput::minimumSizeHint() const
{
    Q_D(const QtAntdInput);
    
    QSize size = QLineEdit::minimumSizeHint();
    size.setHeight(d->getInputHeight());
    
    return size;
}

void QtAntdInput::paintEvent(QPaintEvent *event)
{
    Q_D(QtAntdInput);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QRect rect = this->rect().adjusted(0, 0, -1, -1); // Adjust for border
    
    // Draw shadow effect when focused
    if (d->isFocused) {
        QColor shadowColor = d->getBorderColor();
        shadowColor.setAlpha(25); // 10% opacity for subtle shadow
        
        // Draw multiple shadow rings for softer effect
        for (int i = 0; i < 3; ++i) {
            QRect shadowRect = rect.adjusted(-i-1, -i-1, i+1, i+1);
            painter.setPen(QPen(shadowColor, 1));
            painter.drawRoundedRect(shadowRect, d->getBorderRadius() + i, d->getBorderRadius() + i);
            shadowColor.setAlpha(shadowColor.alpha() * 0.6); // Fade each ring
        }
    }
    
    // Draw background
    QColor bgColor = d->getBackgroundColor();
    painter.fillRect(rect, bgColor);
    
    // Draw border
    QColor borderColor = d->getBorderColor();
    painter.setPen(QPen(borderColor, gAntdBorderWidth));
    painter.drawRoundedRect(rect, d->getBorderRadius(), d->getBorderRadius());
    
    // Draw prefix
    QRect prefixRect = d->prefixRect();
    if (!prefixRect.isEmpty()) {
        painter.setPen(d->getTextColor());
        if (!d->prefixText.isEmpty()) {
            painter.drawText(prefixRect, Qt::AlignCenter, d->prefixText);
        } else if (!d->prefixIcon.isNull()) {
            QPixmap pixmap = d->prefixIcon.pixmap(16, 16);
            painter.drawPixmap(prefixRect.center() - QPoint(8, 8), pixmap);
        }
    }
    
    // Draw suffix
    QRect suffixRect = d->suffixRect();
    if (!suffixRect.isEmpty()) {
        painter.setPen(d->getTextColor());
        if (!d->suffixText.isEmpty()) {
            painter.drawText(suffixRect, Qt::AlignCenter, d->suffixText);
        } else if (!d->suffixIcon.isNull()) {
            QPixmap pixmap = d->suffixIcon.pixmap(16, 16);
            painter.drawPixmap(suffixRect.center() - QPoint(8, 8), pixmap);
        }
    }
    
    // Draw the text content
    painter.end();
    QLineEdit::paintEvent(event);
}

void QtAntdInput::focusInEvent(QFocusEvent *event)
{
    Q_D(QtAntdInput);
    d->isFocused = true;
    update();
    QLineEdit::focusInEvent(event);
}

void QtAntdInput::focusOutEvent(QFocusEvent *event)
{
    Q_D(QtAntdInput);
    d->isFocused = false;
    update();
    QLineEdit::focusOutEvent(event);
}

void QtAntdInput::mousePressEvent(QMouseEvent *event)
{
    Q_D(QtAntdInput);
    
    // Check if clear button was clicked
    if (d->clearButtonEnabled && !text().isEmpty()) {
        QRect clearRect = d->clearButtonRect();
        if (clearRect.contains(event->pos())) {
            d->clearButtonPressed = true;
            update();
            return;
        }
    }
    
    QLineEdit::mousePressEvent(event);
}

void QtAntdInput::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QtAntdInput);
    
    if (d->clearButtonPressed) {
        d->clearButtonPressed = false;
        
        // Check if still over clear button
        QRect clearRect = d->clearButtonRect();
        if (clearRect.contains(event->pos())) {
            onClearButtonClicked();
        }
        
        update();
        return;
    }
    
    QLineEdit::mouseReleaseEvent(event);
}

void QtAntdInput::mouseMoveEvent(QMouseEvent *event)
{
    Q_D(QtAntdInput);
    
    // Check if mouse is over clear button
    bool overClearButton = false;
    if (d->clearButtonEnabled && !text().isEmpty()) {
        QRect clearRect = d->clearButtonRect();
        overClearButton = clearRect.contains(event->pos());
    }
    
    if (d->clearButtonHovered != overClearButton) {
        d->clearButtonHovered = overClearButton;
        setCursor(overClearButton ? Qt::PointingHandCursor : Qt::IBeamCursor);
        update();
    }
    
    QLineEdit::mouseMoveEvent(event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QtAntdInput::enterEvent(QEnterEvent *event)
#else
void QtAntdInput::enterEvent(QEvent *event)
#endif
{
    Q_D(QtAntdInput);
    d->isHovered = true;
    update();
    QLineEdit::enterEvent(event);
}

void QtAntdInput::leaveEvent(QEvent *event)
{
    Q_D(QtAntdInput);
    d->isHovered = false;
    d->clearButtonHovered = false;
    setCursor(Qt::IBeamCursor);
    update();
    QLineEdit::leaveEvent(event);
}

void QtAntdInput::resizeEvent(QResizeEvent *event)
{
    Q_D(QtAntdInput);
    QLineEdit::resizeEvent(event);
    d->updateClearButton();
}

void QtAntdInput::onTextChanged()
{
    Q_D(QtAntdInput);
    d->updateClearButton();
    d->updateTextMargins();
}

void QtAntdInput::onClearButtonClicked()
{
    clear();
    emit clearButtonClicked();
}