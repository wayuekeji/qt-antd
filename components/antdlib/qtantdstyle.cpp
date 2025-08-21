#include <QFontDatabase>
#include <lib/QtAntdStyle.h>
#include <lib/QtAntdTheme.h>

/*!
 *  @class QtAntdStylePrivate
 *  @internal
 */

QtAntdStylePrivate::QtAntdStylePrivate(QtAntdStyle *q)
    : q_ptr(q)
{
}

QtAntdStylePrivate::~QtAntdStylePrivate() {}

void QtAntdStylePrivate::init()
{
    Q_Q(QtAntdStyle);

    q->setTheme(new QtAntdTheme);
}

/*!
 *  @class QtAntdStyle
 *  @internal
 */

void QtAntdStyle::setTheme(QtAntdTheme *theme)
{
    Q_D(QtAntdStyle);

    d->theme = theme;
    theme->setParent(this);
}

QColor QtAntdStyle::themeColor(const QString &key) const
{
    Q_D(const QtAntdStyle);

    Q_ASSERT(d->theme);

    return d->theme->getColor(key);
}

QtAntdStyle::QtAntdStyle()
    : QCommonStyle()
    , d_ptr(new QtAntdStylePrivate(this))
{
    d_func()->init();
}
