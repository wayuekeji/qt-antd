#ifndef QtAntdStyle_P_H
#define QtAntdStyle_P_H

#include <QtGlobal>

class QtAntdStyle;
class QtAntdTheme;

class QtAntdStylePrivate
{
    Q_DISABLE_COPY(QtAntdStylePrivate)
    Q_DECLARE_PUBLIC(QtAntdStyle)

public:
    QtAntdStylePrivate(QtAntdStyle *q);
    ~QtAntdStylePrivate();

    void init();

    QtAntdStyle *const q_ptr;
    QtAntdTheme *theme;
};

#endif  // QtAntdStyle_P_H
