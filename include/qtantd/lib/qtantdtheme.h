#ifndef QtAntdTheme_H
#define QtAntdTheme_H

#include <QColor>
#include <QObject>
#include <QScopedPointer>
#include "components_global.h"

namespace Antd {

enum ThemeMode {
    Light,
    Dark
};

// Ant Design color system based on official specification
enum Color {
    // Primary colors (Blue)
    Blue1,   // #f0f5ff
    Blue2,   // #d6e4ff
    Blue3,   // #adc6ff
    Blue4,   // #85a5ff
    Blue5,   // #597ef7
    Blue6,   // #2f54eb (primary)
    Blue7,   // #1d39c4
    Blue8,   // #10239e
    Blue9,   // #061178
    Blue10,  // #030852
    
    // Neutral colors
    Gray1,   // #ffffff
    Gray2,   // #fafafa
    Gray3,   // #f5f5f5
    Gray4,   // #f0f0f0
    Gray5,   // #d9d9d9
    Gray6,   // #bfbfbf
    Gray7,   // #8c8c8c
    Gray8,   // #595959
    Gray9,   // #434343
    Gray10,  // #262626
    Gray11,  // #1f1f1f
    Gray12,  // #141414
    Gray13,  // #000000

    Red1,
    Red2,
    Red3,
    Red4,
    Red5,
    Red6,
    Red7,
    Red8,
    Red9,
    Red10,
    
    // Semantic colors
    Success,  // #52c41a
    Warning,  // #faad14
    Error,    // #ff4d4f
    Info,     // #1890ff
    
};

}  // namespace Antd

class QtAntdThemePrivate;

class COMPONENTS_EXPORT QtAntdTheme : public QObject
{
    Q_OBJECT

public:
    explicit QtAntdTheme(QObject *parent = 0);
    explicit QtAntdTheme(Antd::ThemeMode mode, QObject *parent = 0);
    ~QtAntdTheme();

    // Theme mode management
    Antd::ThemeMode themeMode() const;
    void setThemeMode(Antd::ThemeMode mode);

    // Color access
    QColor getColor(const QString &key) const;
    QColor getColor(Antd::Color color) const;

    // Color setting
    void setColor(const QString &key, const QColor &color);
    void setColor(const QString &key, Antd::Color color);

    // Predefined semantic color getters
    QColor primary() const;
    QColor secondary() const;
    QColor success() const;
    QColor warning() const;
    QColor error() const;
    QColor info() const;
    QColor text() const;
    QColor textSecondary() const;
    QColor background() const;
    QColor surface() const;
    QColor border() const;
    QColor disabled() const;

    // Static theme creation
    static QtAntdTheme* lightTheme(QObject *parent = 0);
    static QtAntdTheme* darkTheme(QObject *parent = 0);

signals:
    void themeModeChanged(Antd::ThemeMode mode);

protected:
    const QScopedPointer<QtAntdThemePrivate> d_ptr;

private:
    Q_DISABLE_COPY(QtAntdTheme)
    Q_DECLARE_PRIVATE(QtAntdTheme)
};

#endif  // QtAntdTheme_H
