#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <lib/qtantdtheme.h>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void toggleTheme();
    void applyTheme();

private:
    QtAntdTheme *theme;
    QPushButton *themeSwitchButton;
    bool isDarkMode;
};

#endif // MAINWINDOW_H
