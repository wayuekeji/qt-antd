#include "antdswitchsettingseditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

AntdSwitchSettingsEditor::AntdSwitchSettingsEditor(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    createSwitchesGroup();
    createSettingsGroup();
    createOptionsGroup();
    
    mainLayout->addWidget(switchesGroup);
    mainLayout->addWidget(settingsGroup);
    mainLayout->addWidget(optionsGroup);
    mainLayout->addStretch();
    
    setLayout(mainLayout);
}

void AntdSwitchSettingsEditor::createSwitchesGroup()
{
    switchesGroup = new QGroupBox("Switch Examples", this);
    QGridLayout *layout = new QGridLayout(switchesGroup);
    
    // Basic switch
    layout->addWidget(new QLabel("Basic:"), 0, 0);
    basicSwitch = new QtAntdSwitch(this);
    basicSwitch->setChecked(true);
    layout->addWidget(basicSwitch, 0, 1, Qt::AlignLeft);
    
    // Text switch
    layout->addWidget(new QLabel("With Text:"), 1, 0);
    textSwitch = new QtAntdSwitch(this);
    textSwitch->setCheckedText("ON");
    textSwitch->setUncheckedText("OFF");
    textSwitch->setChecked(true);
    layout->addWidget(textSwitch, 1, 1, Qt::AlignLeft);
    
    // Disabled switch
    layout->addWidget(new QLabel("Disabled:"), 2, 0);
    disabledSwitch = new QtAntdSwitch(this);
    disabledSwitch->setEnabled(false);
    disabledSwitch->setChecked(true);
    layout->addWidget(disabledSwitch, 2, 1, Qt::AlignLeft);
    
    // Loading switch
    layout->addWidget(new QLabel("Loading:"), 3, 0);
    loadingSwitch = new QtAntdSwitch(this);
    loadingSwitch->setLoading(true);
    loadingSwitch->setChecked(true);
    layout->addWidget(loadingSwitch, 3, 1, Qt::AlignLeft);
    
    // Size examples
    layout->addWidget(new QLabel("Small:"), 4, 0);
    QtAntdSwitch *smallSwitch = new QtAntdSwitch(this);
    smallSwitch->setSwitchSize(QtAntdSwitch::Small);
    smallSwitch->setChecked(true);
    layout->addWidget(smallSwitch, 4, 1, Qt::AlignLeft);
    
    layout->addWidget(new QLabel("Medium:"), 5, 0);
    QtAntdSwitch *mediumSwitch = new QtAntdSwitch(this);
    mediumSwitch->setSwitchSize(QtAntdSwitch::Medium);
    mediumSwitch->setChecked(true);
    layout->addWidget(mediumSwitch, 5, 1, Qt::AlignLeft);
    
    layout->addWidget(new QLabel("Large:"), 6, 0);
    QtAntdSwitch *largeSwitch = new QtAntdSwitch(this);
    largeSwitch->setSwitchSize(QtAntdSwitch::Large);
    largeSwitch->setChecked(true);
    layout->addWidget(largeSwitch, 6, 1, Qt::AlignLeft);
    
    layout->setColumnStretch(2, 1); // Add stretch to push switches left
}

void AntdSwitchSettingsEditor::createSettingsGroup()
{
    settingsGroup = new QGroupBox("Settings", this);
    QVBoxLayout *layout = new QVBoxLayout(settingsGroup);
    
    // Size setting
    QHBoxLayout *sizeLayout = new QHBoxLayout;
    sizeLayout->addWidget(new QLabel("Size:"));
    sizeCombo = new QComboBox(this);
    sizeCombo->addItem("Small", static_cast<int>(QtAntdSwitch::Small));
    sizeCombo->addItem("Medium", static_cast<int>(QtAntdSwitch::Medium));
    sizeCombo->addItem("Large", static_cast<int>(QtAntdSwitch::Large));
    sizeCombo->setCurrentIndex(1); // Medium
    sizeLayout->addWidget(sizeCombo);
    sizeLayout->addStretch();
    layout->addLayout(sizeLayout);
    
    // Checked text setting
    QHBoxLayout *checkedTextLayout = new QHBoxLayout;
    checkedTextLayout->addWidget(new QLabel("Checked Text:"));
    checkedTextEdit = new QLineEdit(this);
    checkedTextEdit->setPlaceholderText("Text when checked (e.g., 'ON')");
    checkedTextLayout->addWidget(checkedTextEdit);
    layout->addLayout(checkedTextLayout);
    
    // Unchecked text setting
    QHBoxLayout *uncheckedTextLayout = new QHBoxLayout;
    uncheckedTextLayout->addWidget(new QLabel("Unchecked Text:"));
    uncheckedTextEdit = new QLineEdit(this);
    uncheckedTextEdit->setPlaceholderText("Text when unchecked (e.g., 'OFF')");
    uncheckedTextLayout->addWidget(uncheckedTextEdit);
    layout->addLayout(uncheckedTextLayout);
    
    // Connect settings
    connect(sizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AntdSwitchSettingsEditor::onSizeChanged);
    connect(checkedTextEdit, &QLineEdit::textChanged,
            this, &AntdSwitchSettingsEditor::onCheckedTextChanged);
    connect(uncheckedTextEdit, &QLineEdit::textChanged,
            this, &AntdSwitchSettingsEditor::onUncheckedTextChanged);
}

void AntdSwitchSettingsEditor::createOptionsGroup()
{
    optionsGroup = new QGroupBox("Options", this);
    QVBoxLayout *layout = new QVBoxLayout(optionsGroup);
    
    loadingCheck = new QCheckBox("Loading", this);
    loadingCheck->setChecked(false);
    
    useThemeCheck = new QCheckBox("Use Theme Colors", this);
    useThemeCheck->setChecked(true);
    
    disabledCheck = new QCheckBox("Disabled", this);
    disabledCheck->setChecked(false);
    
    layout->addWidget(loadingCheck);
    layout->addWidget(useThemeCheck);
    layout->addWidget(disabledCheck);
    
    connect(loadingCheck, &QCheckBox::toggled,
            this, &AntdSwitchSettingsEditor::onLoadingToggled);
    connect(useThemeCheck, &QCheckBox::toggled,
            this, &AntdSwitchSettingsEditor::onThemeToggled);
    connect(disabledCheck, &QCheckBox::toggled,
            this, &AntdSwitchSettingsEditor::onDisabledToggled);
}

void AntdSwitchSettingsEditor::onSizeChanged()
{
    auto size = static_cast<QtAntdSwitch::SwitchSize>(sizeCombo->currentData().toInt());
    basicSwitch->setSwitchSize(size);
    textSwitch->setSwitchSize(size);
    if (!disabledSwitch->isEnabled()) {
        disabledSwitch->setSwitchSize(size);
    }
    if (!loadingSwitch->isLoading()) {
        loadingSwitch->setSwitchSize(size);
    }
}

void AntdSwitchSettingsEditor::onLoadingToggled(bool loading)
{
    basicSwitch->setLoading(loading);
    textSwitch->setLoading(loading);
    if (!disabledSwitch->isEnabled()) {
        disabledSwitch->setLoading(loading);
    }
}

void AntdSwitchSettingsEditor::onThemeToggled(bool useTheme)
{
    basicSwitch->setUseThemeColors(useTheme);
    textSwitch->setUseThemeColors(useTheme);
    disabledSwitch->setUseThemeColors(useTheme);
    loadingSwitch->setUseThemeColors(useTheme);
}

void AntdSwitchSettingsEditor::onCheckedTextChanged()
{
    QString text = checkedTextEdit->text();
    basicSwitch->setCheckedText(text);
    textSwitch->setCheckedText(text.isEmpty() ? "ON" : text);
    disabledSwitch->setCheckedText(text);
    loadingSwitch->setCheckedText(text);
}

void AntdSwitchSettingsEditor::onUncheckedTextChanged()
{
    QString text = uncheckedTextEdit->text();
    basicSwitch->setUncheckedText(text);
    textSwitch->setUncheckedText(text.isEmpty() ? "OFF" : text);
    disabledSwitch->setUncheckedText(text);
    loadingSwitch->setUncheckedText(text);
}

void AntdSwitchSettingsEditor::onDisabledToggled(bool disabled)
{
    basicSwitch->setEnabled(!disabled);
    textSwitch->setEnabled(!disabled);
    // Don't affect the disabled example switch
    loadingSwitch->setEnabled(!disabled);
}