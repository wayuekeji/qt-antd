#include "qtantdswitch.h"
#include "qtantdswitch_p.h"

#include "qtantdstyle.h"
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

namespace {
// Ant Design switch dimensions (based on official specs)
static const int gSmallTrackWidth = 28;
static const int gSmallTrackHeight = 16;
static const int gSmallHandleSize = 12;

static const int gMediumTrackWidth = 44;
static const int gMediumTrackHeight = 22;
static const int gMediumHandleSize = 18;

static const int gLargeTrackWidth = 56;
static const int gLargeTrackHeight = 28;
static const int gLargeHandleSize = 24;

static const int gHandlePadding = 2; // Padding around handle inside track
static const int gAnimationDuration = 200; // milliseconds
static const int gLoadingSpinnerSpeed = 16; // ms per frame (~60fps)
}

QtAntdSwitchPrivate::QtAntdSwitchPrivate(QtAntdSwitch *q)
    : q_ptr(q)
    , switchSize(QtAntdSwitch::Medium)
    , useThemeColors(true)
    , isLoading(false)
    , isHovered(false)
    , isPressed(false)
    , hasFocus(false)
    , toggleAnimation(nullptr)
    , animationValue(0.0)
    , loadingTimer(nullptr)
    , loadingAngle(0)
    , trackWidth(gMediumTrackWidth)
    , trackHeight(gMediumTrackHeight)
    , handleSize(gMediumHandleSize)
    , handlePadding(gHandlePadding)
{
}

QtAntdSwitchPrivate::~QtAntdSwitchPrivate()
{
    if (loadingTimer) {
        loadingTimer->stop();
    }
}

void QtAntdSwitchPrivate::init()
{
    Q_Q(QtAntdSwitch);
    
    // Setup animation
    toggleAnimation = new QPropertyAnimation(q, "animationValue");
    toggleAnimation->setDuration(gAnimationDuration);
    toggleAnimation->setEasingCurve(QEasingCurve::OutCubic);
    
    // Setup loading timer
    loadingTimer = new QTimer(q);
    QObject::connect(loadingTimer, &QTimer::timeout, [this]() {
        loadingAngle = (loadingAngle + 30) % 360;
        q_ptr->update();
    });
    
    updateGeometry();
    
    // Set initial animation value based on checked state
    animationValue = q->isChecked() ? 1.0 : 0.0;
}

void QtAntdSwitchPrivate::updateGeometry()
{
    switch (switchSize) {
    case QtAntdSwitch::Small:
        trackWidth = gSmallTrackWidth;
        trackHeight = gSmallTrackHeight;
        handleSize = gSmallHandleSize;
        break;
    case QtAntdSwitch::Medium:
        trackWidth = gMediumTrackWidth;
        trackHeight = gMediumTrackHeight;
        handleSize = gMediumHandleSize;
        break;
    case QtAntdSwitch::Large:
        trackWidth = gLargeTrackWidth;
        trackHeight = gLargeTrackHeight;
        handleSize = gLargeHandleSize;
        break;
    }
    
    q_ptr->updateGeometry();
}

QColor QtAntdSwitchPrivate::getTrackColor() const
{
    Q_Q(const QtAntdSwitch);
    
    if (!useThemeColors) {
        if (q->isChecked()) {
            return QColor("#1890ff"); // Default blue
        } else {
            return QColor("#d9d9d9"); // Default gray
        }
    }
    
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q->isEnabled()) {
        return style.themeColor("disabled-background");
    }
    
    if (q->isChecked()) {
        if (isPressed) {
            return style.themeColor("primary-active");
        } else if (isHovered) {
            return style.themeColor("primary-hover");
        } else {
            return style.themeColor("primary");
        }
    } else {
        QColor borderColor = style.themeColor("border");
        if (isPressed) {
            return borderColor.darker(120);
        } else if (isHovered) {
            return borderColor.darker(110);
        } else {
            return borderColor;
        }
    }
}

QColor QtAntdSwitchPrivate::getHandleColor() const
{
    Q_Q(const QtAntdSwitch);
    
    if (!useThemeColors) {
        return QColor("#ffffff");
    }
    
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q->isEnabled()) {
        return style.themeColor("disabled");
    }
    
    return style.themeColor("background");
}

QColor QtAntdSwitchPrivate::getTextColor() const
{
    Q_Q(const QtAntdSwitch);
    
    if (!useThemeColors) {
        return QColor("#ffffff");
    }
    
    QtAntdStyle &style = QtAntdStyle::instance();
    
    if (!q->isEnabled()) {
        return style.themeColor("text-disabled");
    }
    
    return style.themeColor("background");
}

QRect QtAntdSwitchPrivate::getTrackRect() const
{
    Q_Q(const QtAntdSwitch);
    QRect rect = q->rect();
    int x = (rect.width() - trackWidth) / 2;
    int y = (rect.height() - trackHeight) / 2;
    return QRect(x, y, trackWidth, trackHeight);
}

QRect QtAntdSwitchPrivate::getHandleRect() const
{
    QRect trackRect = getTrackRect();
    int handlePos = static_cast<int>(animationValue * (trackWidth - handleSize - 2 * handlePadding));
    int x = trackRect.x() + handlePadding + handlePos;
    int y = trackRect.y() + (trackHeight - handleSize) / 2;
    return QRect(x, y, handleSize, handleSize);
}

QRect QtAntdSwitchPrivate::getTextRect() const
{
    QRect trackRect = getTrackRect();
    // Text area is the track minus handle area
    int textAreaWidth = trackWidth - handleSize - 3 * handlePadding;
    int x, width;
    
    Q_Q(const QtAntdSwitch);
    if (q->isChecked()) {
        // Text on the left when checked
        x = trackRect.x() + handlePadding;
        width = textAreaWidth;
    } else {
        // Text on the right when unchecked  
        x = trackRect.x() + handleSize + 2 * handlePadding;
        width = textAreaWidth;
    }
    
    return QRect(x, trackRect.y(), width, trackRect.height());
}

void QtAntdSwitchPrivate::startToggleAnimation()
{
    Q_Q(QtAntdSwitch);
    
    if (toggleAnimation->state() == QAbstractAnimation::Running) {
        toggleAnimation->stop();
    }
    
    qreal targetValue = q->isChecked() ? 1.0 : 0.0;
    
    toggleAnimation->setStartValue(animationValue);
    toggleAnimation->setEndValue(targetValue);
    toggleAnimation->start();
}

void QtAntdSwitchPrivate::startLoadingAnimation()
{
    if (loadingTimer && !loadingTimer->isActive()) {
        loadingAngle = 0;
        loadingTimer->start(gLoadingSpinnerSpeed);
    }
}

void QtAntdSwitchPrivate::stopLoadingAnimation()
{
    if (loadingTimer) {
        loadingTimer->stop();
    }
}

void QtAntdSwitchPrivate::drawLoadingSpinner(QPainter *painter, const QRect &rect, const QColor &color)
{
    painter->save();
    
    QRect spinnerRect = rect.adjusted(2, 2, -2, -2);
    QPoint center = spinnerRect.center();
    int radius = qMin(spinnerRect.width(), spinnerRect.height()) / 2 - 2;
    
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(center);
    painter->rotate(loadingAngle);
    
    // Draw spinning arc
    QPen pen(color, 1.5);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    
    // Draw 3/4 circle arc
    QRect arcRect(-radius, -radius, radius * 2, radius * 2);
    painter->drawArc(arcRect, 0, 270 * 16); // 270 degrees in 1/16th degree units
    
    painter->restore();
}

// QtAntdSwitch Implementation
QtAntdSwitch::QtAntdSwitch(QWidget *parent)
    : QAbstractButton(parent)
    , d_ptr(new QtAntdSwitchPrivate(this))
{
    Q_D(QtAntdSwitch);
    d->init();
    
    setCheckable(true);
    setFocusPolicy(Qt::TabFocus);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    connect(this, &QAbstractButton::toggled, this, [d](bool checked) {
        Q_UNUSED(checked);
        d->startToggleAnimation();
    });
}

QtAntdSwitch::QtAntdSwitch(const QString &text, QWidget *parent)
    : QtAntdSwitch(parent)
{
    setText(text);
}

QtAntdSwitch::~QtAntdSwitch()
{
}

void QtAntdSwitch::setUseThemeColors(bool value)
{
    Q_D(QtAntdSwitch);
    if (d->useThemeColors != value) {
        d->useThemeColors = value;
        update();
    }
}

bool QtAntdSwitch::useThemeColors() const
{
    Q_D(const QtAntdSwitch);
    return d->useThemeColors;
}

void QtAntdSwitch::setSwitchSize(SwitchSize size)
{
    Q_D(QtAntdSwitch);
    if (d->switchSize != size) {
        d->switchSize = size;
        d->updateGeometry();
        update();
    }
}

QtAntdSwitch::SwitchSize QtAntdSwitch::switchSize() const
{
    Q_D(const QtAntdSwitch);
    return d->switchSize;
}

void QtAntdSwitch::setCheckedText(const QString &text)
{
    Q_D(QtAntdSwitch);
    if (d->checkedText != text) {
        d->checkedText = text;
        update();
    }
}

QString QtAntdSwitch::checkedText() const
{
    Q_D(const QtAntdSwitch);
    return d->checkedText;
}

void QtAntdSwitch::setUncheckedText(const QString &text)
{
    Q_D(QtAntdSwitch);
    if (d->uncheckedText != text) {
        d->uncheckedText = text;
        update();
    }
}

QString QtAntdSwitch::uncheckedText() const
{
    Q_D(const QtAntdSwitch);
    return d->uncheckedText;
}

void QtAntdSwitch::setCheckedIcon(const QIcon &icon)
{
    Q_D(QtAntdSwitch);
    d->checkedIcon = icon;
    update();
}

QIcon QtAntdSwitch::checkedIcon() const
{
    Q_D(const QtAntdSwitch);
    return d->checkedIcon;
}

void QtAntdSwitch::setUncheckedIcon(const QIcon &icon)
{
    Q_D(QtAntdSwitch);
    d->uncheckedIcon = icon;
    update();
}

QIcon QtAntdSwitch::uncheckedIcon() const
{
    Q_D(const QtAntdSwitch);
    return d->uncheckedIcon;
}

void QtAntdSwitch::setLoading(bool loading)
{
    Q_D(QtAntdSwitch);
    if (d->isLoading != loading) {
        d->isLoading = loading;
        if (loading) {
            d->startLoadingAnimation();
            setEnabled(false);
        } else {
            d->stopLoadingAnimation();
            setEnabled(true);
        }
        update();
    }
}

bool QtAntdSwitch::isLoading() const
{
    Q_D(const QtAntdSwitch);
    return d->isLoading;
}

QSize QtAntdSwitch::sizeHint() const
{
    Q_D(const QtAntdSwitch);
    return QSize(d->trackWidth + 4, d->trackHeight + 4); // Small padding around track
}

QSize QtAntdSwitch::minimumSizeHint() const
{
    return sizeHint();
}

qreal QtAntdSwitch::animationValue() const
{
    Q_D(const QtAntdSwitch);
    return d->animationValue;
}

void QtAntdSwitch::setAnimationValue(qreal value)
{
    Q_D(QtAntdSwitch);
    if (d->animationValue != value) {
        d->animationValue = value;
        update();
    }
}

void QtAntdSwitch::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    Q_D(QtAntdSwitch);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QRect trackRect = d->getTrackRect();
    QRect handleRect = d->getHandleRect();
    
    // Draw track
    QColor trackColor = d->getTrackColor();
    painter.setBrush(trackColor);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(trackRect, trackRect.height() / 2, trackRect.height() / 2);
    
    // Draw text/icon in track
    painter.save(); // save and restore painter state around text drawing
    if (isChecked() && !d->checkedText.isEmpty()) {
        painter.setPen(d->getTextColor());
        QFont font = painter.font();
        font.setPixelSize(d->trackHeight * 0.5); // Scale font to track
        painter.setFont(font);
        QRect textRect = d->getTextRect();
        painter.drawText(textRect, Qt::AlignCenter, d->checkedText);
    } else if (!isChecked() && !d->uncheckedText.isEmpty()) {
        painter.setPen(d->getTextColor());
        QFont font = painter.font();
        font.setPixelSize(d->trackHeight * 0.5);
        painter.setFont(font);
        QRect textRect = d->getTextRect();
        painter.drawText(textRect, Qt::AlignCenter, d->uncheckedText);
    }
    painter.restore();
    
    // Draw icons
    painter.save(); // save and restore painter state around icon drawing
    if (isChecked() && !d->checkedIcon.isNull()) {
        QRect textRect = d->getTextRect();
        QSize iconSize(textRect.height() * 0.6, textRect.height() * 0.6);
        QRect iconRect(textRect.center() - QPoint(iconSize.width()/2, iconSize.height()/2), iconSize);
        d->checkedIcon.paint(&painter, iconRect);
    } else if (!isChecked() && !d->uncheckedIcon.isNull()) {
        QRect textRect = d->getTextRect();
        QSize iconSize(textRect.height() * 0.6, textRect.height() * 0.6);
        QRect iconRect(textRect.center() - QPoint(iconSize.width()/2, iconSize.height()/2), iconSize);
        d->uncheckedIcon.paint(&painter, iconRect);
    }
    painter.restore();
    
    // Draw handle
    QColor handleColor = d->getHandleColor();
    painter.setBrush(handleColor);
    
    // Add subtle shadow for handle
    if (isEnabled()) {
        QColor shadowColor = Qt::black;
        shadowColor.setAlpha(30);
        painter.setBrush(shadowColor);
        painter.drawEllipse(handleRect.adjusted(1, 1, 1, 1));
    }
    
    painter.setBrush(handleColor);
    painter.drawEllipse(handleRect);
    
    // Draw loading spinner if loading
    if (d->isLoading) {
        d->drawLoadingSpinner(&painter, handleRect, trackColor.darker(150));
    }
    
    // Draw focus outline
    if (d->hasFocus && !d->isLoading) {
        QPen focusPen(d->getTrackColor(), 2);
        focusPen.setStyle(Qt::DashLine);
        painter.setPen(focusPen);
        painter.setBrush(Qt::NoBrush);
        QRect focusRect = trackRect.adjusted(-2, -2, 2, 2);
        painter.drawRoundedRect(focusRect, focusRect.height() / 2, focusRect.height() / 2);
    }
}

void QtAntdSwitch::mousePressEvent(QMouseEvent *event)
{
    Q_D(QtAntdSwitch);
    if (event->button() == Qt::LeftButton && !d->isLoading) {
        d->isPressed = true;
        update();
    }
    QAbstractButton::mousePressEvent(event);
}

void QtAntdSwitch::mouseReleaseEvent(QMouseEvent *event)
{
    Q_D(QtAntdSwitch);
    if (event->button() == Qt::LeftButton && d->isPressed) {
        d->isPressed = false;
        update();
    }
    QAbstractButton::mouseReleaseEvent(event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QtAntdSwitch::enterEvent(QEnterEvent *event)
#else
void QtAntdSwitch::enterEvent(QEvent *event)
#endif
{
    Q_D(QtAntdSwitch);
    if (!d->isLoading) {
        d->isHovered = true;
        update();
    }
    QAbstractButton::enterEvent(event);
}

void QtAntdSwitch::leaveEvent(QEvent *event)
{
    Q_D(QtAntdSwitch);
    d->isHovered = false;
    d->isPressed = false;
    update();
    QAbstractButton::leaveEvent(event);
}

void QtAntdSwitch::focusInEvent(QFocusEvent *event)
{
    Q_D(QtAntdSwitch);
    d->hasFocus = true;
    update();
    QAbstractButton::focusInEvent(event);
}

void QtAntdSwitch::focusOutEvent(QFocusEvent *event)
{
    Q_D(QtAntdSwitch);
    d->hasFocus = false;
    update();
    QAbstractButton::focusOutEvent(event);
}

void QtAntdSwitch::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Space || event->key() == Qt::Key_Return) && !isLoading()) {
        toggle();
        event->accept();
        return;
    }
    QAbstractButton::keyPressEvent(event);
}