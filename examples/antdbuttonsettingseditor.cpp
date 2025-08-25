#include "antdbuttonsettingseditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
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
    
    // Initialize the icon buttons
    updateIconButtons();
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
    ghostCheckBox->setDisabled(true); //
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
    layout->setAlignment(Qt::AlignCenter);
    
    // Create various button types
    iconLeftButton = new QtAntdButton(style()->standardIcon(QStyle::SP_DialogOpenButton), "Search", this);
    textOnlyButton = new QtAntdButton("Text Only", this);
    
    // Create icon-only button with no text
    iconOnlyButton = new QtAntdButton();
    iconOnlyButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    iconOnlyButton->setToolTip("Icon Only Button (No Text)");
    
    // Set a default icon size
    iconLeftButton->setIconSize(QSize(16, 16));
    iconOnlyButton->setIconSize(QSize(20, 20));
    
    // Add buttons to layout
    layout->addWidget(iconLeftButton);
    layout->addWidget(textOnlyButton);
    layout->addWidget(iconOnlyButton);
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
    iconLeftButton->setButtonType(type);
    textOnlyButton->setButtonType(type);
    iconOnlyButton->setButtonType(type);
}

void AntdButtonSettingsEditor::onSizeChanged()
{
    QtAntdButton::ButtonSize size = static_cast<QtAntdButton::ButtonSize>(sizeCombo->currentData().toInt());
    iconLeftButton->setButtonSize(size);
    textOnlyButton->setButtonSize(size);
    iconOnlyButton->setButtonSize(size);
}

void AntdButtonSettingsEditor::onShapeChanged()
{
    QtAntdButton::ButtonShape shape = static_cast<QtAntdButton::ButtonShape>(shapeCombo->currentData().toInt());
    iconLeftButton->setButtonShape(shape);
    textOnlyButton->setButtonShape(shape);
    iconOnlyButton->setButtonShape(shape);
}

void AntdButtonSettingsEditor::onBlockToggled(bool block)
{
    iconLeftButton->setBlock(block);
    textOnlyButton->setBlock(block);
    iconOnlyButton->setBlock(block);
}

void AntdButtonSettingsEditor::onGhostToggled(bool ghost)
{
    iconLeftButton->setGhost(ghost);
    textOnlyButton->setGhost(ghost);
    iconOnlyButton->setGhost(ghost);
}

void AntdButtonSettingsEditor::onDangerToggled(bool danger)
{
    iconLeftButton->setDanger(danger);
    textOnlyButton->setDanger(danger);
    iconOnlyButton->setDanger(danger);
}

void AntdButtonSettingsEditor::onLoadingToggled(bool loading)
{
    iconLeftButton->setLoading(loading);
    textOnlyButton->setLoading(loading);
    iconOnlyButton->setLoading(loading);
}

void AntdButtonSettingsEditor::onThemeToggled(bool useTheme)
{
    iconLeftButton->setUseThemeColors(useTheme);
    textOnlyButton->setUseThemeColors(useTheme);
    iconOnlyButton->setUseThemeColors(useTheme);
}

void AntdButtonSettingsEditor::onIconPositionChanged()
{
    updateIconButtons();
}

void AntdButtonSettingsEditor::updateIconButtons()
{
    // Set icons according to position
    // iconLeftButton->setIcon(iconOnLeft ? style()->standardIcon(QStyle::SP_DialogOpenButton) : QIcon());
    iconLeftButton->setIconSize(QSize(16, 16));
    
    // The icon-only button always has an icon
    iconOnlyButton->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    iconOnlyButton->setIconSize(QSize(20, 20));
    
    // Add a tooltip to clarify this is an icon-only button
    iconOnlyButton->setToolTip("Icon Only Button (No Text)");
}