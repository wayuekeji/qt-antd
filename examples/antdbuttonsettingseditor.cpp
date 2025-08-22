#include "antdbuttonsettingseditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <qtantdbutton.h>

AntdButtonSettingsEditor::AntdButtonSettingsEditor(QWidget *parent)
    : QWidget(parent),
      mainLayout(new QVBoxLayout(this))
{
    createButtonTypeGroup();
    createButtonSizeGroup();
    createButtonShapeGroup();
    createButtonOptionsGroup();
    createPreviewGroup();
    setupLayout();
}

void AntdButtonSettingsEditor::createButtonTypeGroup()
{
    typeGroup = new QGroupBox("Button Type", this);
    QVBoxLayout *layout = new QVBoxLayout(typeGroup);
    
    typeCombo = new QComboBox(this);
    typeCombo->addItem("Primary", static_cast<int>(QtAntdButton::Primary));
    typeCombo->addItem("Default", static_cast<int>(QtAntdButton::Default));
    typeCombo->addItem("Dashed", static_cast<int>(QtAntdButton::Dashed));
    typeCombo->addItem("Text", static_cast<int>(QtAntdButton::Text));
    typeCombo->addItem("Link", static_cast<int>(QtAntdButton::Link));
    typeCombo->setCurrentIndex(1); // Default
    
    layout->addWidget(typeCombo);
    
    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AntdButtonSettingsEditor::onTypeChanged);
}

void AntdButtonSettingsEditor::createButtonSizeGroup()
{
    sizeGroup = new QGroupBox("Button Size", this);
    QVBoxLayout *layout = new QVBoxLayout(sizeGroup);
    
    sizeCombo = new QComboBox(this);
    sizeCombo->addItem("Small", static_cast<int>(QtAntdButton::Small));
    sizeCombo->addItem("Medium", static_cast<int>(QtAntdButton::Medium));
    sizeCombo->addItem("Large", static_cast<int>(QtAntdButton::Large));
    sizeCombo->setCurrentIndex(1); // Medium
    
    layout->addWidget(sizeCombo);
    
    connect(sizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AntdButtonSettingsEditor::onSizeChanged);
}

void AntdButtonSettingsEditor::createButtonShapeGroup()
{
    shapeGroup = new QGroupBox("Button Shape", this);
    QVBoxLayout *layout = new QVBoxLayout(shapeGroup);
    
    shapeCombo = new QComboBox(this);
    shapeCombo->addItem("Default", static_cast<int>(QtAntdButton::DefaultShape));
    shapeCombo->addItem("Circle", static_cast<int>(QtAntdButton::Circle));
    shapeCombo->addItem("Round", static_cast<int>(QtAntdButton::Round));
    shapeCombo->setCurrentIndex(0); // Default
    
    layout->addWidget(shapeCombo);
    
    connect(shapeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AntdButtonSettingsEditor::onShapeChanged);
}

void AntdButtonSettingsEditor::createButtonOptionsGroup()
{
    optionsGroup = new QGroupBox("Options", this);
    QVBoxLayout *layout = new QVBoxLayout(optionsGroup);
    
    blockCheckBox = new QCheckBox("Block (full width)", this);
    ghostCheckBox = new QCheckBox("Ghost", this);
    dangerCheckBox = new QCheckBox("Danger", this);
    loadingCheckBox = new QCheckBox("Loading", this);
    useThemeCheckBox = new QCheckBox("Use Theme Colors", this);
    useThemeCheckBox->setChecked(true);
    
    layout->addWidget(blockCheckBox);
    layout->addWidget(ghostCheckBox);
    layout->addWidget(dangerCheckBox);
    layout->addWidget(loadingCheckBox);
    layout->addWidget(useThemeCheckBox);
    
    connect(blockCheckBox, &QCheckBox::toggled, this, &AntdButtonSettingsEditor::onBlockToggled);
    connect(ghostCheckBox, &QCheckBox::toggled, this, &AntdButtonSettingsEditor::onGhostToggled);
    connect(dangerCheckBox, &QCheckBox::toggled, this, &AntdButtonSettingsEditor::onDangerToggled);
    connect(loadingCheckBox, &QCheckBox::toggled, this, &AntdButtonSettingsEditor::onLoadingToggled);
    connect(useThemeCheckBox, &QCheckBox::toggled, this, &AntdButtonSettingsEditor::onThemeToggled);
}

void AntdButtonSettingsEditor::createPreviewGroup()
{
    previewGroup = new QGroupBox("Preview", this);
    QVBoxLayout *layout = new QVBoxLayout(previewGroup);
    
    previewButton = new QtAntdButton("Sample Button", this);
    iconButton = new QtAntdButton("🎉 With Icon", this);
    textOnlyButton = new QtAntdButton("Text Only", this);
    
    layout->addWidget(previewButton);
    layout->addWidget(iconButton);
    layout->addWidget(textOnlyButton);
}

void AntdButtonSettingsEditor::setupLayout()
{
    mainLayout->addWidget(typeGroup);
    mainLayout->addWidget(sizeGroup);
    mainLayout->addWidget(shapeGroup);
    mainLayout->addWidget(optionsGroup);
    mainLayout->addWidget(previewGroup);
    mainLayout->addStretch();
    
    setLayout(mainLayout);
}

void AntdButtonSettingsEditor::onTypeChanged()
{
    QtAntdButton::ButtonType type = static_cast<QtAntdButton::ButtonType>(typeCombo->currentData().toInt());
    previewButton->setButtonType(type);
    iconButton->setButtonType(type);
    textOnlyButton->setButtonType(type);
}

void AntdButtonSettingsEditor::onSizeChanged()
{
    QtAntdButton::ButtonSize size = static_cast<QtAntdButton::ButtonSize>(sizeCombo->currentData().toInt());
    previewButton->setButtonSize(size);
    iconButton->setButtonSize(size);
    textOnlyButton->setButtonSize(size);
}

void AntdButtonSettingsEditor::onShapeChanged()
{
    QtAntdButton::ButtonShape shape = static_cast<QtAntdButton::ButtonShape>(shapeCombo->currentData().toInt());
    previewButton->setButtonShape(shape);
    iconButton->setButtonShape(shape);
    textOnlyButton->setButtonShape(shape);
}

void AntdButtonSettingsEditor::onBlockToggled(bool block)
{
    previewButton->setBlock(block);
    iconButton->setBlock(block);
    textOnlyButton->setBlock(block);
}

void AntdButtonSettingsEditor::onGhostToggled(bool ghost)
{
    previewButton->setGhost(ghost);
    iconButton->setGhost(ghost);
    textOnlyButton->setGhost(ghost);
}

void AntdButtonSettingsEditor::onDangerToggled(bool danger)
{
    previewButton->setDanger(danger);
    iconButton->setDanger(danger);
    textOnlyButton->setDanger(danger);
}

void AntdButtonSettingsEditor::onLoadingToggled(bool loading)
{
    previewButton->setLoading(loading);
    iconButton->setLoading(loading);
    textOnlyButton->setLoading(loading);
}

void AntdButtonSettingsEditor::onThemeToggled(bool useTheme)
{
    previewButton->setUseThemeColors(useTheme);
    iconButton->setUseThemeColors(useTheme);
    textOnlyButton->setUseThemeColors(useTheme);
}