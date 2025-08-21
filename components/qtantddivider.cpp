#include "qtantddivider.h"
#include "qtantddivider_p.h"
#include "lib/qtantdstyle.h"
#include <QPainter>
#include <QFontMetrics>

/*!
 * \class QtAntdDividerPrivate
 * \internal
 */

/*!
 * \internal
 */
QtAntdDividerPrivate::QtAntdDividerPrivate(QtAntdDivider *q)
    : q_ptr(q)
{
}

/*!
 * \internal
 */
QtAntdDividerPrivate::~QtAntdDividerPrivate()
{
}

/*!
 * \internal
 */
void QtAntdDividerPrivate::init()
{
    Q_Q(QtAntdDivider);

    textAlignment = QtAntdDivider::Center;
    lineStyle = QtAntdDivider::SolidLine;
    lineThickness = 1;
    useThemeColors = true;

    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    q->setSizePolicy(policy);
}

/*!
 * \class QtAntdDivider
 */

QtAntdDivider::QtAntdDivider(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new QtAntdDividerPrivate(this))
{
    d_func()->init();
}

QtAntdDivider::QtAntdDivider(const QString &text, QWidget *parent)
    : QWidget(parent)
    , d_ptr(new QtAntdDividerPrivate(this))
{
    d_func()->init();
    setText(text);
}

QtAntdDivider::~QtAntdDivider()
{
}

void QtAntdDivider::setUseThemeColors(bool value)
{
    Q_D(QtAntdDivider);

    if (d->useThemeColors == value) {
        return;
    }

    d->useThemeColors = value;
    update();
}

bool QtAntdDivider::useThemeColors() const
{
    Q_D(const QtAntdDivider);

    return d->useThemeColors;
}

void QtAntdDivider::setText(const QString &text)
{
    Q_D(QtAntdDivider);

    if (d->text == text) {
        return;
    }

    d->text = text;
    update();
}

QString QtAntdDivider::text() const
{
    Q_D(const QtAntdDivider);

    return d->text;
}

void QtAntdDivider::setTextAlignment(TextAlignment alignment)
{
    Q_D(QtAntdDivider);

    if (d->textAlignment == alignment) {
        return;
    }

    d->textAlignment = alignment;
    update();
}

QtAntdDivider::TextAlignment QtAntdDivider::textAlignment() const
{
    Q_D(const QtAntdDivider);

    return d->textAlignment;
}

void QtAntdDivider::setLineThickness(int thickness)
{
    Q_D(QtAntdDivider);

    if (d->lineThickness == thickness || thickness <= 0) {
        return;
    }

    d->lineThickness = thickness;
    update();
}

int QtAntdDivider::lineThickness() const
{
    Q_D(const QtAntdDivider);

    return d->lineThickness;
}

void QtAntdDivider::setLineColor(const QColor &color)
{
    Q_D(QtAntdDivider);

    d->lineColor = color;
    
    ANTD_DISABLE_THEME_COLORS
    update();
}

QColor QtAntdDivider::lineColor() const
{
    Q_D(const QtAntdDivider);

    if (d->useThemeColors || !d->lineColor.isValid()) {
        return QtAntdStyle::instance().themeColor("border");
    } else {
        return d->lineColor;
    }
}

void QtAntdDivider::setTextColor(const QColor &color)
{
    Q_D(QtAntdDivider);

    d->textColor = color;

    ANTD_DISABLE_THEME_COLORS
    update();
}

QColor QtAntdDivider::textColor() const
{
    Q_D(const QtAntdDivider);

    if (d->useThemeColors || !d->textColor.isValid()) {
        return QtAntdStyle::instance().themeColor("text");
    } else {
        return d->textColor;
    }
}

void QtAntdDivider::setLineStyle(LineStyle style)
{
    Q_D(QtAntdDivider);

    if (d->lineStyle == style) {
        return;
    }

    d->lineStyle = style;
    update();
}

QtAntdDivider::LineStyle QtAntdDivider::lineStyle() const
{
    Q_D(const QtAntdDivider);

    return d->lineStyle;
}

/*!
 * \reimp
 */
QSize QtAntdDivider::sizeHint() const
{
    Q_D(const QtAntdDivider);

    if (d->text.isEmpty()) {
        return QSize(-1, qMax(d->lineThickness, 16));
    } else {
        QFontMetrics fm(font());
        return QSize(-1, qMax(fm.height() + 8, d->lineThickness + 8));
    }
}

/*!
 * \reimp
 */
QSize QtAntdDivider::minimumSizeHint() const
{
    Q_D(const QtAntdDivider);

    if (d->text.isEmpty()) {
        return QSize(0, qMax(d->lineThickness, 1));
    } else {
        QFontMetrics fm(font());
        return QSize(fm.horizontalAdvance(d->text), qMax(fm.height(), d->lineThickness));
    }
}

/*!
 * \reimp
 */
void QtAntdDivider::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    Q_D(QtAntdDivider);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect r = rect();
    const int centerY = r.height() / 2;

    // Set up pen for the line
    QPen pen(lineColor());
    pen.setWidth(d->lineThickness);
    
    switch (d->lineStyle) {
        case DashLine:
            pen.setStyle(Qt::DashLine);
            break;
        case DotLine:
            pen.setStyle(Qt::DotLine);
            break;
        case SolidLine:
        default:
            pen.setStyle(Qt::SolidLine);
            break;
    }

    painter.setPen(pen);

    if (d->text.isEmpty()) {
        // Draw full width line
        painter.drawLine(r.left(), centerY, r.right(), centerY);
    } else {
        // Draw line with text
        QFontMetrics fm(font());
        const int textWidth = fm.horizontalAdvance(d->text);
        const int textHeight = fm.height();
        const int padding = 16; // Space between text and lines
        
        QRect textRect;
        int lineLeft = r.left();
        int lineRight = r.right();

        switch (d->textAlignment) {
            case Left: {
                textRect = QRect(r.left() + 2 * padding, centerY - textHeight/2, textWidth, textHeight);
                // left line: from r.left() to textRect.left() - padding
                if (r.left() < textRect.left() - padding) {
                    painter.drawLine(r.left(), centerY, textRect.left() - padding, centerY);
                }
                // right line: from textRect.right() + padding to r.right()
                if (textRect.right() + padding < r.right()) {
                    painter.drawLine(textRect.right() + padding, centerY, r.right(), centerY);
                }
                break;
            }
            case Right: {
                textRect = QRect(r.right() - textWidth - 2 * padding, centerY - textHeight/2, textWidth, textHeight);
                // left line: from r.left() to textRect.left() - padding
                if (r.left() < textRect.left() - padding) {
                    painter.drawLine(r.left(), centerY, textRect.left() - padding, centerY);
                }
                // right line: from textRect.right() + padding to r.right()
                if (textRect.right() + padding < r.right()) {
                    painter.drawLine(textRect.right() + padding, centerY, r.right(), centerY);
                }
                break;
            }
            case Center:
            default: {
                textRect = QRect((r.width() - textWidth) / 2, centerY - textHeight/2, textWidth, textHeight);
                int lineLeft = r.left();
                int lineRight = textRect.left() - padding;
                if (lineLeft < lineRight) {
                    painter.drawLine(lineLeft, centerY, lineRight, centerY);
                }
                int rightLineStart = textRect.right() + padding;
                if (rightLineStart < r.right()) {
                    painter.drawLine(rightLineStart, centerY, r.right(), centerY);
                }
                break;
            }
        }

        // Draw text
        painter.setPen(textColor());
        painter.drawText(textRect, Qt::AlignCenter, d->text);
    }
}
