#ifndef QTANTDDIVIDER_P_H
#define QTANTDDIVIDER_P_H

#include <QtGlobal>
#include <QColor>

class QtAntdDivider;

class QtAntdDividerPrivate
{
    Q_DISABLE_COPY(QtAntdDividerPrivate)
    Q_DECLARE_PUBLIC(QtAntdDivider)

public:
    QtAntdDividerPrivate(QtAntdDivider *q);
    ~QtAntdDividerPrivate();

    void init();

    QtAntdDivider *const q_ptr;
    QString text;
    QtAntdDivider::TextAlignment textAlignment;
    QtAntdDivider::LineStyle lineStyle;
    int lineThickness;
    bool useThemeColors;
    QColor lineColor;
    QColor textColor;
};

#endif // QTANTDDIVIDER_P_H