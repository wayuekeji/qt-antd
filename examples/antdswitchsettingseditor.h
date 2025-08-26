#ifndef ANTDSWITCHSETTINGSEDITOR_H
#define ANTDSWITCHSETTINGSEDITOR_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include "qtantdswitch.h"

class AntdSwitchSettingsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AntdSwitchSettingsEditor(QWidget *parent = nullptr);

private slots:
    void onSizeChanged();
    void onLoadingToggled(bool loading);
    void onThemeToggled(bool useTheme);
    void onCheckedTextChanged();
    void onUncheckedTextChanged();
    void onDisabledToggled(bool disabled);

private:
    void createSwitchesGroup();
    void createSettingsGroup();
    void createOptionsGroup();

    // Switch examples
    QGroupBox *switchesGroup;
    QtAntdSwitch *basicSwitch;
    QtAntdSwitch *textSwitch;
    QtAntdSwitch *disabledSwitch;
    QtAntdSwitch *loadingSwitch;

    // Settings controls
    QGroupBox *settingsGroup;
    QComboBox *sizeCombo;
    QLineEdit *checkedTextEdit;
    QLineEdit *uncheckedTextEdit;

    // Options controls
    QGroupBox *optionsGroup;
    QCheckBox *loadingCheck;
    QCheckBox *useThemeCheck;
    QCheckBox *disabledCheck;
};

#endif // ANTDSWITCHSETTINGSEDITOR_H