#include <lib/QtAntdTheme.h>

#include "QtAntdTheme_p.h"

#include <QDebug>
#include <QString>
#include <QStringBuilder>

/*!Antd
 *  @class QtAntdThemePrivate
 *  @internal
 */

QtAntdThemePrivate::QtAntdThemePrivate(QtAntdTheme *q)
    : q_ptr(q)
{
}

QtAntdThemePrivate::~QtAntdThemePrivate() {}

QColor QtAntdThemePrivate::rgba(int r, int g, int b, qreal a) const
{
    QColor color(r, g, b);
    color.setAlphaF(a);
    return color;
}

/*!
 *  @class QtAntdTheme
 */

QtAntdTheme::QtAntdTheme(QObject *parent)
    : QObject(parent)
    , d_ptr(new QtAntdThemePrivate(this))
{
    setColor("primary1", Antd::lightWhite);
    setColor("primary2", Antd::lightWhite);
    setColor("primary3", Antd::lightWhite);
    setColor("accent1", Antd::lightWhite);
    setColor("accent2", Antd::lightWhite);
    setColor("accent3", Antd::lightWhite);
    setColor("text", Antd::lightWhite);
    setColor("alternateText", Antd::lightWhite);
    setColor("canvas", Antd::lightWhite);
    setColor("border", Antd::lightWhite);
    setColor("disabled", Antd::lightWhite);
    setColor("disabled2", Antd::lightWhite);
    setColor("disabled3", Antd::lightWhite);
}

QtAntdTheme::~QtAntdTheme() {}

QColor QtAntdTheme::getColor(const QString &key) const
{
    Q_D(const QtAntdTheme);

    if (!d->colors.contains(key)) {
        qWarning() << "A theme color matching the key '" << key << "' could not be found.";
        return QColor();
    }
    return d->colors.value(key);
}

void QtAntdTheme::setColor(const QString &key, const QColor &color)
{
    Q_D(QtAntdTheme);

    d->colors.insert(key, color);
}

void QtAntdTheme::setColor(const QString &key, Antd::Color color)
{
    Q_D(QtAntdTheme);

    static const QColor palette[] = {
        d->rgba(255, 255, 255, 0.54)};

    d->colors.insert(key, palette[color]);
}