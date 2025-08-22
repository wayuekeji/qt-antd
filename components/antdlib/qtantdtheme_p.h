#ifndef QtAntdTheme_P_H
#define QtAntdTheme_P_H

#include <QColor>
#include <QHash>
#include "../include/qtantd/lib/qtantdtheme.h"

class QtAntdTheme;

class QtAntdThemePrivate
{
    Q_DISABLE_COPY(QtAntdThemePrivate)
    Q_DECLARE_PUBLIC(QtAntdTheme)

public:
    QtAntdThemePrivate(QtAntdTheme *q);
    ~QtAntdThemePrivate();

    QColor rgba(int r, int g, int b, qreal a = 1.0) const;
    QColor hex(const QString &hex) const;
    
    void initializeLightTheme();
    void initializeDarkTheme();
    void initializeColorPalette();
    
    QColor getAntdColor(Antd::Color color) const;

    QtAntdTheme *const q_ptr;
    QHash<QString, QColor> colors;
    QHash<Antd::Color, QPair<QColor, QColor>> colorPalette; // Light, Dark pairs
    Antd::ThemeMode themeMode;
};

#endif  // QtAntdTheme_P_H
