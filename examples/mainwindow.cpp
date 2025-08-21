#include "mainwindow.h"
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QListWidget>
#include "antddividersettingseditor.h"
#include <QMap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    widget->setMaximumHeight(800);
    QHBoxLayout *layout = new QHBoxLayout;

    widget->setLayout(layout);

    QStackedLayout *stack = new QStackedLayout;
    QListWidget *list = new QListWidget;

    layout->addWidget(list);
    layout->addLayout(stack);

    layout->setStretch(1, 2);

    setCentralWidget(widget);

    AntdDividerSettingsEditor *divider = new AntdDividerSettingsEditor;

    QMap<QString, QWidget*> pageMap = {
        {"Divider", divider}
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
}

MainWindow::~MainWindow()
{
}
