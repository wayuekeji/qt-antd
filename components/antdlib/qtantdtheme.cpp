#include "../include/qtantd/lib/qtantdtheme.h"

#include "qtantdtheme_p.h"

#include <QDebug>
#include <QString>
#include <QStringBuilder>

/*!Antd
 *  @class QtAntdThemePrivate
 *  @internal
 */

QtAntdThemePrivate::QtAntdThemePrivate(QtAntdTheme *q)
    : q_ptr(q), themeMode(Antd::Light)
{
    initializeColorPalette();
}

QtAntdThemePrivate::~QtAntdThemePrivate() {}

QColor QtAntdThemePrivate::rgba(int r, int g, int b, qreal a) const
{
    QColor color(r, g, b);
    color.setAlphaF(a);
    return color;
}

QColor QtAntdThemePrivate::hex(const QString &hex) const
{
    return QColor(hex);
}

void QtAntdThemePrivate::initializeColorPalette()
{
    // Blue palette (Primary)
    colorPalette[Antd::Blue1] = QPair<QColor, QColor>(hex("#f0f5ff"), hex("#111d2c"));
    colorPalette[Antd::Blue2] = QPair<QColor, QColor>(hex("#d6e4ff"), hex("#112a45"));
    colorPalette[Antd::Blue3] = QPair<QColor, QColor>(hex("#adc6ff"), hex("#15395b"));
    colorPalette[Antd::Blue4] = QPair<QColor, QColor>(hex("#85a5ff"), hex("#164c7e"));
    colorPalette[Antd::Blue5] = QPair<QColor, QColor>(hex("#597ef7"), hex("#1765ad"));
    colorPalette[Antd::Blue6] = QPair<QColor, QColor>(hex("#2f54eb"), hex("#177ddc"));
    colorPalette[Antd::Blue7] = QPair<QColor, QColor>(hex("#1d39c4"), hex("#3c9ae8"));
    colorPalette[Antd::Blue8] = QPair<QColor, QColor>(hex("#10239e"), hex("#65b7f3"));
    colorPalette[Antd::Blue9] = QPair<QColor, QColor>(hex("#061178"), hex("#8dcff8"));
    colorPalette[Antd::Blue10] = QPair<QColor, QColor>(hex("#030852"), hex("#b7e3fa"));
    
    // Gray palette (Neutral)
    colorPalette[Antd::Gray1] = QPair<QColor, QColor>(hex("#ffffff"), hex("#141414"));
    colorPalette[Antd::Gray2] = QPair<QColor, QColor>(hex("#fafafa"), hex("#1f1f1f"));
    colorPalette[Antd::Gray3] = QPair<QColor, QColor>(hex("#f5f5f5"), hex("#262626"));
    colorPalette[Antd::Gray4] = QPair<QColor, QColor>(hex("#f0f0f0"), hex("#434343"));
    colorPalette[Antd::Gray5] = QPair<QColor, QColor>(hex("#d9d9d9"), hex("#595959"));
    colorPalette[Antd::Gray6] = QPair<QColor, QColor>(hex("#bfbfbf"), hex("#8c8c8c"));
    colorPalette[Antd::Gray7] = QPair<QColor, QColor>(hex("#8c8c8c"), hex("#bfbfbf"));
    colorPalette[Antd::Gray8] = QPair<QColor, QColor>(hex("#595959"), hex("#d9d9d9"));
    colorPalette[Antd::Gray9] = QPair<QColor, QColor>(hex("#434343"), hex("#f0f0f0"));
    colorPalette[Antd::Gray10] = QPair<QColor, QColor>(hex("#262626"), hex("#f5f5f5"));
    colorPalette[Antd::Gray11] = QPair<QColor, QColor>(hex("#1f1f1f"), hex("#fafafa"));
    colorPalette[Antd::Gray12] = QPair<QColor, QColor>(hex("#141414"), hex("#ffffff"));
    colorPalette[Antd::Gray13] = QPair<QColor, QColor>(hex("#000000"), hex("#ffffff"));
    
    // Semantic colors
    colorPalette[Antd::Success] = QPair<QColor, QColor>(hex("#52c41a"), hex("#389e0d"));
    colorPalette[Antd::Warning] = QPair<QColor, QColor>(hex("#faad14"), hex("#d48806"));
    colorPalette[Antd::Error] = QPair<QColor, QColor>(hex("#ff4d4f"), hex("#cf1322"));
    colorPalette[Antd::Info] = QPair<QColor, QColor>(hex("#1890ff"), hex("#177ddc"));
}

QColor QtAntdThemePrivate::getAntdColor(Antd::Color color) const
{
    if (!colorPalette.contains(color)) {
        return QColor();
    }
    
    return themeMode == Antd::Light 
        ? colorPalette[color].first 
        : colorPalette[color].second;
}

void QtAntdThemePrivate::initializeLightTheme()
{
    // Primary colors
    colors["primary"] = getAntdColor(Antd::Blue6);
    colors["primary-hover"] = getAntdColor(Antd::Blue5);
    colors["primary-active"] = getAntdColor(Antd::Blue7);
    colors["primary-outline"] = getAntdColor(Antd::Blue1);
    
    // Secondary colors
    colors["secondary"] = getAntdColor(Antd::Gray6);
    colors["secondary-hover"] = getAntdColor(Antd::Gray5);
    colors["secondary-active"] = getAntdColor(Antd::Gray7);
    
    // Text colors
    colors["text"] = getAntdColor(Antd::Gray13);
    colors["text-secondary"] = getAntdColor(Antd::Gray8);
    colors["text-tertiary"] = getAntdColor(Antd::Gray7);
    colors["text-quaternary"] = getAntdColor(Antd::Gray6);
    colors["text-disabled"] = getAntdColor(Antd::Gray5);
    
    // Background colors
    colors["background"] = getAntdColor(Antd::Gray1);
    colors["background-secondary"] = getAntdColor(Antd::Gray2);
    colors["surface"] = getAntdColor(Antd::Gray1);
    colors["surface-variant"] = getAntdColor(Antd::Gray3);
    
    // Border colors
    colors["border"] = getAntdColor(Antd::Gray5);
    colors["border-secondary"] = getAntdColor(Antd::Gray4);
    colors["border-variant"] = getAntdColor(Antd::Gray3);
    
    // Status colors
    colors["success"] = getAntdColor(Antd::Success);
    colors["warning"] = getAntdColor(Antd::Warning);
    colors["error"] = getAntdColor(Antd::Error);
    colors["info"] = getAntdColor(Antd::Info);
    
    // Disabled state
    colors["disabled"] = getAntdColor(Antd::Gray5);
    colors["disabled-background"] = getAntdColor(Antd::Gray2);
    
}

void QtAntdThemePrivate::initializeDarkTheme()
{
    // Primary colors (inverted for dark mode)
    colors["primary"] = getAntdColor(Antd::Blue6);
    colors["primary-hover"] = getAntdColor(Antd::Blue7);
    colors["primary-active"] = getAntdColor(Antd::Blue5);
    colors["primary-outline"] = getAntdColor(Antd::Blue1);
    
    // Secondary colors
    colors["secondary"] = getAntdColor(Antd::Gray6);
    colors["secondary-hover"] = getAntdColor(Antd::Gray7);
    colors["secondary-active"] = getAntdColor(Antd::Gray5);
    
    // Text colors (inverted)
    colors["text"] = getAntdColor(Antd::Gray1);
    colors["text-secondary"] = getAntdColor(Antd::Gray8);
    colors["text-tertiary"] = getAntdColor(Antd::Gray7);
    colors["text-quaternary"] = getAntdColor(Antd::Gray6);
    colors["text-disabled"] = getAntdColor(Antd::Gray5);
    
    // Background colors (dark)
    colors["background"] = getAntdColor(Antd::Gray12);
    colors["background-secondary"] = getAntdColor(Antd::Gray11);
    colors["surface"] = getAntdColor(Antd::Gray11);
    colors["surface-variant"] = getAntdColor(Antd::Gray10);
    
    // Border colors (darker)
    colors["border"] = getAntdColor(Antd::Gray5);
    colors["border-secondary"] = getAntdColor(Antd::Gray4);
    colors["border-variant"] = getAntdColor(Antd::Gray3);
    
    // Status colors (adjusted for dark mode)
    colors["success"] = getAntdColor(Antd::Success);
    colors["warning"] = getAntdColor(Antd::Warning);
    colors["error"] = getAntdColor(Antd::Error);
    colors["info"] = getAntdColor(Antd::Info);
    
    // Disabled state
    colors["disabled"] = getAntdColor(Antd::Gray5);
    colors["disabled-background"] = getAntdColor(Antd::Gray11);
    
}

/*!
 *  @class QtAntdTheme
 */

QtAntdTheme::QtAntdTheme(QObject *parent)
    : QObject(parent)
    , d_ptr(new QtAntdThemePrivate(this))
{
    Q_D(QtAntdTheme);
    d->themeMode = Antd::Light;
    d->initializeLightTheme();
}

QtAntdTheme::QtAntdTheme(Antd::ThemeMode mode, QObject *parent)
    : QObject(parent)
    , d_ptr(new QtAntdThemePrivate(this))
{
    Q_D(QtAntdTheme);
    d->themeMode = mode;
    if (mode == Antd::Dark) {
        d->initializeDarkTheme();
    } else {
        d->initializeLightTheme();
    }
}

QtAntdTheme::~QtAntdTheme() {}

Antd::ThemeMode QtAntdTheme::themeMode() const
{
    Q_D(const QtAntdTheme);
    return d->themeMode;
}

void QtAntdTheme::setThemeMode(Antd::ThemeMode mode)
{
    Q_D(QtAntdTheme);
    if (d->themeMode != mode) {
        d->themeMode = mode;
        
        // Reinitialize colors based on new theme mode
        if (mode == Antd::Dark) {
            d->initializeDarkTheme();
        } else {
            d->initializeLightTheme();
        }
        
        emit themeModeChanged(mode);
    }
}

QColor QtAntdTheme::getColor(const QString &key) const
{
    Q_D(const QtAntdTheme);

    if (!d->colors.contains(key)) {
        qWarning() << "A theme color matching the key '" << key << "' could not be found.";
        return QColor();
    }
    return d->colors.value(key);
}

QColor QtAntdTheme::getColor(Antd::Color color) const
{
    Q_D(const QtAntdTheme);
    return d->getAntdColor(color);
}

void QtAntdTheme::setColor(const QString &key, const QColor &color)
{
    Q_D(QtAntdTheme);
    d->colors.insert(key, color);
}

void QtAntdTheme::setColor(const QString &key, Antd::Color color)
{
    Q_D(QtAntdTheme);
    d->colors.insert(key, d->getAntdColor(color));
}

// Predefined semantic color getters
QColor QtAntdTheme::primary() const
{
    return getColor("primary");
}

QColor QtAntdTheme::secondary() const
{
    return getColor("secondary");
}

QColor QtAntdTheme::success() const
{
    return getColor("success");
}

QColor QtAntdTheme::warning() const
{
    return getColor("warning");
}

QColor QtAntdTheme::error() const
{
    return getColor("error");
}

QColor QtAntdTheme::info() const
{
    return getColor("info");
}

QColor QtAntdTheme::text() const
{
    return getColor("text");
}

QColor QtAntdTheme::textSecondary() const
{
    return getColor("text-secondary");
}

QColor QtAntdTheme::background() const
{
    return getColor("background");
}

QColor QtAntdTheme::surface() const
{
    return getColor("surface");
}

QColor QtAntdTheme::border() const
{
    return getColor("border");
}

QColor QtAntdTheme::disabled() const
{
    return getColor("disabled");
}

// Static theme creation methods
QtAntdTheme* QtAntdTheme::lightTheme(QObject *parent)
{
    return new QtAntdTheme(Antd::Light, parent);
}

QtAntdTheme* QtAntdTheme::darkTheme(QObject *parent)
{
    return new QtAntdTheme(Antd::Dark, parent);
}