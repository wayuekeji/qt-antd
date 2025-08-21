#include <QtWidgets/QApplication>
#include <QDebug>
#include <lib/QtAntdTheme.h>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    window.show();

    return a.exec();
}
