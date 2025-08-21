#ifndef QtAntdTheme_P_H
#define QtAntdTheme_P_H

#include <QColor>
#include <QHash>

class QtAntdTheme;

class QtAntdThemePrivate
{
    Q_DISABLE_COPY(QtAntdThemePrivate)
    Q_DECLARE_PUBLIC(QtAntdTheme)

public:
    QtAntdThemePrivate(QtAntdTheme *q);
    ~QtAntdThemePrivate();

    QColor rgba(int r, int g, int b, qreal a) const;

    QtAntdTheme *const q_ptr;
    QHash<QString, QColor> colors;
};

#endif  // QtAntdTheme_P_H
