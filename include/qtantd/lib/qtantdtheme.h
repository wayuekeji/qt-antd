#ifndef QtAntdTheme_H
#define QtAntdTheme_H

#include <QColor>
#include <QObject>
#include <QScopedPointer>
#include "components_global.h"

namespace Antd {
enum Color {
    lightWhite
};
}  // namespace 

class QtAntdThemePrivate;

class COMPONENTS_EXPORT QtAntdTheme : public QObject
{
    Q_OBJECT

public:
    explicit QtAntdTheme(QObject *parent = 0);
    ~QtAntdTheme();

    QColor getColor(const QString &key) const;

    void setColor(const QString &key, const QColor &color);
    void setColor(const QString &key, Antd::Color color);

protected:
    const QScopedPointer<QtAntdThemePrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtAntdTheme)
    Q_DECLARE_PRIVATE(QtAntdTheme)
};

#endif  // QtAntdTheme_H
