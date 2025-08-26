#include "antdinputsettingseditor.h"
#include <qtantdinput.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>

AntdInputSettingsEditor::AntdInputSettingsEditor(QWidget *parent)
    : QWidget(parent),
      mainLayout(new QVBoxLayout(this))
{
    createSizeGroup();
    createStatusGroup();
    createOptionsGroup();
    createContentGroup();
    createPreviewGroup();
    setupLayout();
}

void AntdInputSettingsEditor::createSizeGroup()
{
    sizeGroup = new QGroupBox("Input Size", this);
    QVBoxLayout *layout = new QVBoxLayout(sizeGroup);
    
    sizeCombo = new QComboBox(this);
    sizeCombo->addItem("Small", static_cast<int>(QtAntdInput::Small));
    sizeCombo->addItem("Medium", static_cast<int>(QtAntdInput::Medium));
    sizeCombo->addItem("Large", static_cast<int>(QtAntdInput::Large));
    sizeCombo->setCurrentIndex(1); // Medium
    
    layout->addWidget(sizeCombo);
    
    connect(sizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AntdInputSettingsEditor::onSizeChanged);
}

void AntdInputSettingsEditor::createStatusGroup()
{
    statusGroup = new QGroupBox("Input Status", this);
    QVBoxLayout *layout = new QVBoxLayout(statusGroup);
    
    statusCombo = new QComboBox(this);
    statusCombo->addItem("Normal", static_cast<int>(QtAntdInput::Normal));
    statusCombo->addItem("Error", static_cast<int>(QtAntdInput::Error));
    statusCombo->addItem("Warning", static_cast<int>(QtAntdInput::Warning));
    statusCombo->addItem("Success", static_cast<int>(QtAntdInput::Success));
    statusCombo->setCurrentIndex(0); // Normal
    
    layout->addWidget(statusCombo);
    
    connect(statusCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AntdInputSettingsEditor::onStatusChanged);
}

void AntdInputSettingsEditor::createOptionsGroup()
{
    optionsGroup = new QGroupBox("Options", this);
    QVBoxLayout *layout = new QVBoxLayout(optionsGroup);
    
    clearButtonCheck = new QCheckBox("Clear Button", this);
    clearButtonCheck->setChecked(false);
    
    useThemeCheck = new QCheckBox("Use Theme Colors", this);
    useThemeCheck->setChecked(true);
    
    disabledCheck = new QCheckBox("Disabled", this);
    disabledCheck->setChecked(false);
    
    readOnlyCheck = new QCheckBox("Read Only", this);
    readOnlyCheck->setChecked(false);
    
    layout->addWidget(clearButtonCheck);
    layout->addWidget(useThemeCheck);
    layout->addWidget(disabledCheck);
    layout->addWidget(readOnlyCheck);
    
    connect(clearButtonCheck, &QCheckBox::toggled,
            this, &AntdInputSettingsEditor::onClearButtonToggled);
    connect(useThemeCheck, &QCheckBox::toggled,
            this, &AntdInputSettingsEditor::onThemeToggled);
    connect(disabledCheck, &QCheckBox::toggled,
            this, &AntdInputSettingsEditor::onDisabledToggled);
    connect(readOnlyCheck, &QCheckBox::toggled,
            this, &AntdInputSettingsEditor::onReadOnlyToggled);
}

void AntdInputSettingsEditor::createContentGroup()
{
    contentGroup = new QGroupBox("Content", this);
    QVBoxLayout *layout = new QVBoxLayout(contentGroup);
    
    // Placeholder
    QHBoxLayout *placeholderLayout = new QHBoxLayout();
    placeholderLayout->addWidget(new QLabel("Placeholder:", this));
    placeholderEdit = new QLineEdit("Enter text here", this);
    placeholderLayout->addWidget(placeholderEdit);
    layout->addLayout(placeholderLayout);
    
    // Prefix
    QHBoxLayout *prefixLayout = new QHBoxLayout();
    prefixLayout->addWidget(new QLabel("Prefix:", this));
    prefixEdit = new QLineEdit("", this);
    prefixEdit->setPlaceholderText("e.g., $");
    prefixLayout->addWidget(prefixEdit);
    layout->addLayout(prefixLayout);
    
    // Suffix
    QHBoxLayout *suffixLayout = new QHBoxLayout();
    suffixLayout->addWidget(new QLabel("Suffix:", this));
    suffixEdit = new QLineEdit("", this);
    suffixEdit->setPlaceholderText("e.g., .com");
    suffixLayout->addWidget(suffixEdit);
    layout->addLayout(suffixLayout);
    
    connect(placeholderEdit, &QLineEdit::textChanged,
            this, &AntdInputSettingsEditor::onPlaceholderChanged);
    connect(prefixEdit, &QLineEdit::textChanged,
            this, &AntdInputSettingsEditor::onPrefixChanged);
    connect(suffixEdit, &QLineEdit::textChanged,
            this, &AntdInputSettingsEditor::onSuffixChanged);
}

void AntdInputSettingsEditor::createPreviewGroup()
{
    previewGroup = new QGroupBox("Preview", this);
    QVBoxLayout *layout = new QVBoxLayout(previewGroup);
    
    // Basic input
    QLabel *basicLabel = new QLabel("Basic Input:", this);
    basicInput = new QtAntdInput(this);
    basicInput->setPlaceholderText("Enter text here");
    layout->addWidget(basicLabel);
    layout->addWidget(basicInput);
    
    // Input with prefix
    QLabel *prefixLabel = new QLabel("With Prefix:", this);
    prefixInput = new QtAntdInput(this);
    prefixInput->setPlaceholderText("0.00");
    prefixInput->setPrefixText("$");
    layout->addWidget(prefixLabel);
    layout->addWidget(prefixInput);
    
    // Input with suffix
    QLabel *suffixLabel = new QLabel("With Suffix:", this);
    suffixInput = new QtAntdInput(this);
    suffixInput->setPlaceholderText("mysite");
    suffixInput->setSuffixText(".com");
    layout->addWidget(suffixLabel);
    layout->addWidget(suffixInput);
    
    // Clearable input
    QLabel *clearLabel = new QLabel("Clearable Input:", this);
    clearableInput = new QtAntdInput(this);
    clearableInput->setPlaceholderText("Type something...");
    clearableInput->setClearButtonEnabled(true);
    clearableInput->setText("Sample text");
    layout->addWidget(clearLabel);
    layout->addWidget(clearableInput);
}

void AntdInputSettingsEditor::setupLayout()
{
    mainLayout->addWidget(sizeGroup);
    mainLayout->addWidget(statusGroup);
    mainLayout->addWidget(optionsGroup);
    mainLayout->addWidget(contentGroup);
    mainLayout->addWidget(previewGroup);
    mainLayout->addStretch();
    
    setLayout(mainLayout);
}

void AntdInputSettingsEditor::onSizeChanged()
{
    QtAntdInput::InputSize size = static_cast<QtAntdInput::InputSize>(sizeCombo->currentData().toInt());
    basicInput->setInputSize(size);
    prefixInput->setInputSize(size);
    suffixInput->setInputSize(size);
    clearableInput->setInputSize(size);
}

void AntdInputSettingsEditor::onStatusChanged()
{
    QtAntdInput::InputStatus status = static_cast<QtAntdInput::InputStatus>(statusCombo->currentData().toInt());
    basicInput->setInputStatus(status);
    prefixInput->setInputStatus(status);
    suffixInput->setInputStatus(status);
    clearableInput->setInputStatus(status);
}

void AntdInputSettingsEditor::onClearButtonToggled(bool enabled)
{
    basicInput->setClearButtonEnabled(enabled);
    prefixInput->setClearButtonEnabled(enabled);
    suffixInput->setClearButtonEnabled(enabled);
    // clearableInput always has it enabled
}

void AntdInputSettingsEditor::onThemeToggled(bool useTheme)
{
    basicInput->setUseThemeColors(useTheme);
    prefixInput->setUseThemeColors(useTheme);
    suffixInput->setUseThemeColors(useTheme);
    clearableInput->setUseThemeColors(useTheme);
}

void AntdInputSettingsEditor::onPrefixChanged()
{
    QString prefix = prefixEdit->text();
    prefixInput->setPrefixText(prefix);
    if (!prefix.isEmpty()) {
        basicInput->setPrefixText(prefix);
    } else {
        basicInput->setPrefixText("");
    }
}

void AntdInputSettingsEditor::onSuffixChanged()
{
    QString suffix = suffixEdit->text();
    suffixInput->setSuffixText(suffix);
    if (!suffix.isEmpty()) {
        basicInput->setSuffixText(suffix);
    } else {
        basicInput->setSuffixText("");
    }
}

void AntdInputSettingsEditor::onPlaceholderChanged()
{
    QString placeholder = placeholderEdit->text();
    basicInput->setPlaceholderText(placeholder);
    // Keep the other inputs with their specific placeholders
}

void AntdInputSettingsEditor::onDisabledToggled(bool disabled)
{
    basicInput->setDisabled(disabled);
    prefixInput->setDisabled(disabled);
    suffixInput->setDisabled(disabled);
    clearableInput->setDisabled(disabled);
}

void AntdInputSettingsEditor::onReadOnlyToggled(bool readOnly)
{
    basicInput->setReadOnly(readOnly);
    prefixInput->setReadOnly(readOnly);
    suffixInput->setReadOnly(readOnly);
    clearableInput->setReadOnly(readOnly);
}