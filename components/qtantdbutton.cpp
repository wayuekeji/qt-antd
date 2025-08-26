#include "qtantdbutton.h"
#include "qtantdbutton_p.h"

#include "qtantdstyle.h"
#include <QPainter>
#include <QFontMetrics>
#include <QApplication>
#include <QMouseEvent>
#include <QStyleOptionButton>
#include <QEnterEvent>
#include <QTimer>
#include <QtMath>
#include <QDebug>
#include <QPainterPath>
#include <QElapsedTimer>
#include <unordered_map>

namespace {
// --- Start outer ripple overlay implementation ---

// Global expansion distance (px) the outer ripple expands beyond the button
static int gAntdWaveExpandDistance = 6;

class QtAntdRippleOverlay : public QWidget {
public:
    QtAntdRippleOverlay(QWidget* parent,
                        const QRect& innerRectInOverlay,
                        int cornerRadius,
                        bool isCircle,
                        const QColor& color,
                        int extra)
        : QWidget(parent)
        , m_innerRect(innerRectInOverlay)
        , m_cornerRadius(cornerRadius)
        , m_isCircle(isCircle)
        , m_color(color)
        , m_extra(extra)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
        setAttribute(Qt::WA_NoSystemBackground, true);
        setAttribute(Qt::WA_TranslucentBackground, true);
        m_timer = new QTimer(this);
        QObject::connect(m_timer, &QTimer::timeout, this, [this]() {
            if (m_clock.isValid()) {
                const int elapsed = int(m_clock.elapsed());
                if (elapsed >= m_duration) {
                    m_timer->stop();
                    this->deleteLater();
                    return;
                }
            }
            update();
        });
    }

    void start() {
        m_clock.start();
        m_timer->start(16); // ~60 FPS
        show();
        raise();
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);

        const qreal t = m_clock.isValid() ? qBound<qreal>(0.0, m_clock.elapsed() / qreal(m_duration), 1.0) : 0.0;
        const qreal ease = 1.0 - std::pow(1.0 - t, 3.0);
        const qreal grow = m_extra * ease;
        const qreal alpha = (1.0 - t) * 0.35;

        QRectF outer = m_innerRect.adjusted(-grow, -grow, grow, grow);
        const qreal outerRadius = m_isCircle ? outer.height() / 2.0 : m_cornerRadius + grow;

        QPainterPath innerPath;
        if (m_isCircle) {
            innerPath.addEllipse(m_innerRect);
        } else {
            innerPath.addRoundedRect(m_innerRect, m_cornerRadius, m_cornerRadius);
        }

        QPainterPath outerPath;
        if (m_isCircle) {
            outerPath.addEllipse(outer);
        } else {
            outerPath.addRoundedRect(outer, outerRadius, outerRadius);
        }

        QPainterPath ring = outerPath.subtracted(innerPath);
        QColor c = m_color;
        c.setAlphaF(alpha);
        p.fillPath(ring, c);
    }

private:
    QRectF m_innerRect;
    int m_cornerRadius;
    bool m_isCircle;
    QColor m_color;
    int m_extra;
    int m_duration { 450 };
    QElapsedTimer m_clock;
    QTimer* m_timer { nullptr };
};

static void StartOuterRipple(QtAntdButton* btn, const QColor& color, int extra)
{
    // Compute inner rect like paintEvent
    QRect rect = btn->rect().adjusted(1, 1, -1, -1);
    int radius = 6;
    bool isCircle = false;
    if (btn->buttonShape() == QtAntdButton::Round) {
        radius = rect.height() / 2;
    } else if (btn->buttonShape() == QtAntdButton::Circle) {
        int size = qMin(rect.width(), rect.height());
        rect = QRect(rect.x() + (rect.width() - size) / 2,
                     rect.y() + (rect.height() - size) / 2,
                     size, size);
        radius = size / 2;
        isCircle = true;
    }

    QWidget* win = btn->window();
    const QPoint topLeftInWin = btn->mapTo(win, rect.topLeft());
    const QRect innerInWin(topLeftInWin, rect.size());

    // Overlay geometry expanded by extra pixels around
    const QRect overlayRect = innerInWin.adjusted(-extra, -extra, extra, extra);
    const QRect innerInOverlay(innerInWin.topLeft() - overlayRect.topLeft(), innerInWin.size());

    auto overlay = new QtAntdRippleOverlay(win,
                                           innerInOverlay,
                                           radius,
                                           isCircle,
                                           color,
                                           extra);
    overlay->setGeometry(overlayRect);
    overlay->start();
}
} // namespace
// --- End outer ripple overlay implementation ---

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
static const std::unordered_map<QtAntdButton::ColorPreset, QColor> preset2brandBaseColor = {
    // { QtAntdButton::PresetDefault, QColor("#000000") },
    { QtAntdButton::PresetPrimary, style.themeColor("primary") },
    { QtAntdButton::PresetDanger,  style.themeColor("error") },
    { QtAntdButton::PresetDefault, style.themeColor("primary") }, // Default unified to primary
    { QtAntdButton::PresetPink,   QColor("#eb2f96") },
    { QtAntdButton::PresetPurple, QColor("#722ed1") },
    { QtAntdButton::PresetCyan,   QColor("#13c2c2") }
};

static const std::unordered_map<QtAntdButton::ColorPreset, QColor> preset2brandHoverColor = {
    // { QtAntdButton::PresetDefault, QColor("#404040") },
    { QtAntdButton::PresetPrimary, style.themeColor("primary-hover") },
    { QtAntdButton::PresetDanger,  style.themeColor("error-hover") },
    { QtAntdButton::PresetDefault, style.themeColor("primary-hover") },
    { QtAntdButton::PresetPink,   QColor("#ff85c0") },
    { QtAntdButton::PresetPurple, QColor("#9254de") },
    { QtAntdButton::PresetCyan,   QColor("#36cfc9") }
};

static const std::unordered_map<QtAntdButton::ColorPreset, QColor> preset2brandActiveColor = {
    // { QtAntdButton::PresetDefault, QColor("#0d0d0d") },
    { QtAntdButton::PresetPrimary, style.themeColor("primary-active") },
    { QtAntdButton::PresetDanger,  style.themeColor("error-active") },
    { QtAntdButton::PresetDefault, style.themeColor("primary-active") },
    { QtAntdButton::PresetPink,   QColor("#c41d7f") },
    { QtAntdButton::PresetPurple, QColor("#531dab") },
    { QtAntdButton::PresetCyan,   QColor("#08979c") }
};

static const std::unordered_map<QtAntdButton::ColorPreset, QColor> preset2brandWeakColor = {
    // { QtAntdButton::PresetDefault, QColor("#e5e5e5") },
    { QtAntdButton::PresetDanger,  QColor("#fff2f0") },
    { QtAntdButton::PresetPink,    QColor("#fff0f6") },
    { QtAntdButton::PresetPurple,  QColor("#f9f0ff") },
    { QtAntdButton::PresetCyan,    QColor("#e6fffb") },
    { QtAntdButton::PresetPrimary, QColor("#f0f5ff") },
    { QtAntdButton::PresetDefault, QColor("#f0f5ff") }
};

// Helper: compute base brand color according to preset
static QColor brandBaseForPreset(QtAntdButton::ColorPreset preset) {
    if(preset2brandBaseColor.find(preset) != preset2brandBaseColor.end()) {
        return preset2brandBaseColor.at(preset);
    }
    return QtAntdStyle::instance().themeColor("primary");
}

static QColor brandHoverForPreset(QtAntdButton::ColorPreset preset) {
    if(preset2brandHoverColor.find(preset) != preset2brandHoverColor.end()) {
        return preset2brandHoverColor.at(preset);
    }
    return QtAntdStyle::instance().themeColor("primary-hover");
}

static QColor brandActiveForPreset(QtAntdButton::ColorPreset preset) {
    if(preset2brandActiveColor.find(preset) != preset2brandActiveColor.end()) {
        return preset2brandActiveColor.at(preset);
    }
    return QtAntdStyle::instance().themeColor("primary-active");
}

static QColor brandWeakBgForPreset(QtAntdButton::ColorPreset preset) {
    if(preset2brandWeakColor.find(preset) != preset2brandWeakColor.end()) {
        return preset2brandWeakColor.at(preset);
    }
    return QColor(255,255,255,0);
}

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
    colorPreset = QtAntdButton::PresetDefault;
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

void QtAntdButton::setColorPreset(ColorPreset preset)
{
    Q_D(QtAntdButton);
    if (d->colorPreset != preset) {
        d->colorPreset = preset;
        update();
    }
}

QtAntdButton::ColorPreset QtAntdButton::colorPreset() const
{
    Q_D(const QtAntdButton);
    return d->colorPreset;
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
    
    // Helper: compute dynamic colors based on type, preset and states
    auto computeColors = [&](QColor &bg, QColor &border, QColor &text) {
        const bool disabled = !isEnabled();
        if (disabled) {
            bg = style.themeColor("disabled-background");
            border = style.themeColor("border");
            text = style.themeColor("disabled");
            return;
        }

        const bool hover = d->isHovered && !d->isPressed;
        const bool pressed = d->isPressed;
        const auto preset = d->colorPreset;

        // Unified preset-based logic for all presets (Default treated as Primary)
        const QColor brand = brandBaseForPreset(preset);
        const QColor brandHover = brandHoverForPreset(preset);
        const QColor brandActive = brandActiveForPreset(preset);
        const QColor weakBg = brandWeakBgForPreset(preset);

        const bool filled = (d->buttonType == Primary);
        const bool borderOnly = (d->buttonType == Default || d->buttonType == Dashed);
        const bool textLike = (d->buttonType == Text || d->buttonType == Link);

        if (filled) {
            bg = pressed ? brandActive : (hover ? brandHover : brand);
            border = bg;
            text = Qt::white;
        } else if (borderOnly) {
            bg = QColor(255,255,255,0);
            if (hover) bg = weakBg;
            if (pressed) bg = weakBg.darker(105);
            border = pressed ? brandActive : (hover ? brandHover : brand);
            text = border;
        } else if (textLike) {
            bg = QColor(255,255,255,0);
            if (hover && d->buttonType == Text) bg = weakBg;
            if (pressed && d->buttonType == Text) bg = weakBg.darker(105);
            border = QColor(255,255,255,0);
            text = pressed ? brandActive : (hover ? brandHover : brand);
        }

        // Danger flag overrides preset for simplicity
        if (d->isDanger) {
            const QColor dBrand = style.themeColor("error");
            const QColor dHover = style.themeColor("error-hover");
            const QColor dActive = style.themeColor("error-active");
            const QColor dWeak = QColor("#fff2f0");
            if (filled) {
                bg = pressed ? dActive : (hover ? dHover : dBrand);
                border = bg;
                text = Qt::white;
            } else if (borderOnly) {
                bg = QColor(255,255,255,0);
                if (hover) bg = dWeak;
                if (pressed) bg = QColor("#ffccc7");
                border = pressed ? dActive : (hover ? dHover : dBrand);
                text = border;
            } else if (textLike) {
                bg = QColor(255,255,255,0);
                if (hover) bg = dWeak;
                if (pressed) bg = QColor("#ffccc7");
                border = QColor(255,255,255,0);
                text = pressed ? dActive : (hover ? dHover : dBrand);
            }
        }
    };
    
    QColor currentBgColor, currentBorderColor, currentTextColor;
    computeColors(currentBgColor, currentBorderColor, currentTextColor);

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

        // Start outer ripple when released inside and enabled
        const bool inside = rect().contains(event->pos());
        if (inside && isEnabled() && d->buttonType != Text && d->buttonType != Link) {
            auto getRippleBrand = [&]() -> QColor {
                QtAntdStyle &style = QtAntdStyle::instance();
                if (d->isDanger) {
                    return style.themeColor("error-hover");
                }
                return brandHoverForPreset(d->colorPreset);
            };

            QColor rippleColor = getRippleBrand();

            int extra = gAntdWaveExpandDistance;
            StartOuterRipple(this, rippleColor, extra);
        }
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QtAntdButton::enterEvent(QEnterEvent *event)
#else
void QtAntdButton::enterEvent(QEvent *event)
#endif
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