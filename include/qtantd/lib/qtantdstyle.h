#ifndef QTANTDSTYLE_H
#define QTANTDSTYLE_H

#include "antdlib/qtantdstyle_p.h"
#include "components_global.h"
#include <QtWidgets/QCommonStyle>

#define ANTD_DISABLE_THEME_COLORS                                                                                            \
    if (d->useThemeColors == true) {                                                                                             \
        d->useThemeColors = false;                                                                                               \
    }

class QtAntdTheme;

class COMPONENTS_EXPORT QtAntdStyle : public QCommonStyle
{
    Q_OBJECT

public:
    inline static QtAntdStyle &instance();

    void setTheme(QtAntdTheme *theme);
    QColor themeColor(const QString &key) const;

protected:
    const QScopedPointer<QtAntdStylePrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(QtAntdStyle)

    QtAntdStyle();

    QtAntdStyle(QtAntdStyle const &);
    void operator=(QtAntdStyle const &);
};

inline QtAntdStyle &QtAntdStyle::instance()
{
    static QtAntdStyle instance;
    return instance;
}

#endif  // QTANTDSTYLE_H
