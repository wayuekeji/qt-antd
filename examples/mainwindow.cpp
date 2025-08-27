#include "mainwindow.h"
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QListWidget>
#include "antddividersettingseditor.h"
#include "antdbuttonsettingseditor.h"
#include "antdinputsettingseditor.h"
#include "antdswitchsettingseditor.h"
#include "antdselectsettingseditor.h"
#include <QMap>
#include <lib/qtantdtheme.h>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isDarkMode(false)
{
    theme = QtAntdTheme::lightTheme(this);
    QWidget *widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    widget->setLayout(layout);

    QStackedLayout *stack = new QStackedLayout;
    QListWidget *list = new QListWidget;

    layout->addWidget(list);
    layout->addLayout(stack);

    layout->setStretch(1, 4);

    setCentralWidget(widget);

    AntdDividerSettingsEditor *divider = new AntdDividerSettingsEditor;
    AntdButtonSettingsEditor *button = new AntdButtonSettingsEditor;
    AntdInputSettingsEditor *input = new AntdInputSettingsEditor;
    AntdSwitchSettingsEditor *switchEditor = new AntdSwitchSettingsEditor;
    AntdSelectSettingsEditor *selectEditor = new AntdSelectSettingsEditor;

    QMap<QString, QWidget*> pageMap = {
        {"Divider", divider},
        {"Button", button},
        {"Input", input},
        {"Switch", switchEditor},
        {"Select", selectEditor}
    };

    for (auto it = pageMap.begin(); it != pageMap.end(); ++it) {
        list->addItem(it.key());
        stack->addWidget(it.value());
    }

    list->setCurrentRow(0);

    QObject::connect(list, &QListWidget::currentTextChanged, [=](const QString &text){
        QWidget *w = pageMap.value(text, nullptr);
        if (w) stack->setCurrentWidget(w);
    });

    applyTheme();
}

void MainWindow::toggleTheme()
{
    isDarkMode = !isDarkMode;
    if (isDarkMode) {
        theme->setThemeMode(Antd::Dark);
    } else {
        theme->setThemeMode(Antd::Light);
    }
    applyTheme();
}

void MainWindow::applyTheme()
{
    // Apply theme to main window and widgets
    QString bgColor = theme->background().name();
    QString textColor = theme->text().name();
    QString primaryColor = theme->primary().name();
    QString borderColor = theme->border().name();
    setStyleSheet(QString(
        "QMainWindow { background-color: %1; color: %2; } "
        "QListWidget { background-color: %1; color: %2; border: 1px solid %3; } "
        "QListWidget::item:selected { background-color: %4; } "
        "QPushButton { background-color: %4; color: white; border: none; "
        "  padding: 8px 16px; border-radius: 4px; } "
        "QPushButton:hover { background-color: %5; }"
    ).arg(bgColor, textColor, borderColor, primaryColor, theme->getColor("primary-hover").name()));
}

MainWindow::~MainWindow()
{
}
