#include "antdselectsettingseditor.h"
#include <QApplication>
#include <QScrollArea>
#include <QSplitter>
#include <QTextEdit>
#include <QInputDialog>

AntdSelectSettingsEditor::AntdSelectSettingsEditor(QWidget *parent)
    : QWidget(parent)
{
    createControls();
    createSelectExamples();
    connectSignals();
    populateExampleData();
    
    // Initial setup
    onSelectionChanged();
}

void AntdSelectSettingsEditor::createControls()
{
    setMinimumSize(800, 600);
    
    auto *mainLayout = new QHBoxLayout(this);
    auto *splitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(splitter);
    
    // Left side - Controls
    auto *controlsWidget = new QWidget();
    auto *controlsLayout = new QVBoxLayout(controlsWidget);
    
    // Settings group
    auto *settingsGroup = new QGroupBox("Settings");
    auto *settingsLayout = new QVBoxLayout(settingsGroup);
    
    m_useThemeColorsCheck = new QCheckBox("Use Theme Colors");
    m_useThemeColorsCheck->setChecked(true);
    settingsLayout->addWidget(m_useThemeColorsCheck);
    
    // Size selection
    auto *sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(new QLabel("Size:"));
    m_sizeCombo = new QComboBox();
    m_sizeCombo->addItems({"Small", "Medium", "Large"});
    m_sizeCombo->setCurrentText("Medium");
    sizeLayout->addWidget(m_sizeCombo);
    sizeLayout->addStretch();
    settingsLayout->addLayout(sizeLayout);
    
    // Placeholder
    auto *placeholderLayout = new QHBoxLayout();
    placeholderLayout->addWidget(new QLabel("Placeholder:"));
    m_placeholderEdit = new QLineEdit("Please select");
    placeholderLayout->addWidget(m_placeholderEdit);
    settingsLayout->addLayout(placeholderLayout);
    
    m_allowClearCheck = new QCheckBox("Allow Clear");
    settingsLayout->addWidget(m_allowClearCheck);
    
    m_disabledCheck = new QCheckBox("Disabled");
    settingsLayout->addWidget(m_disabledCheck);
    
    m_loadingCheck = new QCheckBox("Loading");
    settingsLayout->addWidget(m_loadingCheck);
    
    m_showSearchCheck = new QCheckBox("Show Search");
    settingsLayout->addWidget(m_showSearchCheck);
    
    m_multipleCheck = new QCheckBox("Multiple Selection");
    settingsLayout->addWidget(m_multipleCheck);
    
    controlsLayout->addWidget(settingsGroup);
    
    // Options management group
    auto *optionsGroup = new QGroupBox("Options Management");
    auto *optionsLayout = new QVBoxLayout(optionsGroup);
    
    m_addOptionButton = new QPushButton("Add Option");
    optionsLayout->addWidget(m_addOptionButton);
    
    m_clearOptionsButton = new QPushButton("Clear All Options");
    optionsLayout->addWidget(m_clearOptionsButton);
    
    controlsLayout->addWidget(optionsGroup);
    
    // Status group
    auto *statusGroup = new QGroupBox("Status");
    auto *statusLayout = new QVBoxLayout(statusGroup);
    
    m_statusLabel = new QLabel("Ready");
    statusLayout->addWidget(m_statusLabel);
    
    m_selectedValueLabel = new QLabel("Selected: None");
    statusLayout->addWidget(m_selectedValueLabel);
    
    controlsLayout->addWidget(statusGroup);
    
    controlsLayout->addStretch();
    splitter->addWidget(controlsWidget);
    
    // Right side - Examples
    auto *examplesWidget = new QWidget();
    auto *examplesLayout = new QVBoxLayout(examplesWidget);
    
    auto *examplesGroup = new QGroupBox("Examples");
    auto *examplesGroupLayout = new QVBoxLayout(examplesGroup);
    
    // Basic select
    examplesGroupLayout->addWidget(new QLabel("Basic Select:"));
    m_basicSelect = new QtAntdSelect();
    m_basicSelect->setMinimumWidth(200);
    examplesGroupLayout->addWidget(m_basicSelect);
    
    examplesGroupLayout->addSpacing(10);
    
    // Search select
    examplesGroupLayout->addWidget(new QLabel("Search Select:"));
    m_searchSelect = new QtAntdSelect();
    m_searchSelect->setShowSearch(true);
    m_searchSelect->setMinimumWidth(200);
    examplesGroupLayout->addWidget(m_searchSelect);
    
    examplesGroupLayout->addSpacing(10);
    
    // Multiple select
    examplesGroupLayout->addWidget(new QLabel("Multiple Select:"));
    m_multipleSelect = new QtAntdSelect();
    m_multipleSelect->setMultiple(true);
    m_multipleSelect->setAllowClear(true);
    m_multipleSelect->setMinimumWidth(200);
    examplesGroupLayout->addWidget(m_multipleSelect);
    
    examplesGroupLayout->addSpacing(10);
    
    // Disabled select
    examplesGroupLayout->addWidget(new QLabel("Disabled Select:"));
    m_disabledSelect = new QtAntdSelect();
    m_disabledSelect->setDisabled(true);
    m_disabledSelect->setMinimumWidth(200);
    examplesGroupLayout->addWidget(m_disabledSelect);
    
    examplesGroupLayout->addSpacing(10);
    
    // Loading select
    examplesGroupLayout->addWidget(new QLabel("Loading Select:"));
    m_loadingSelect = new QtAntdSelect();
    m_loadingSelect->setLoading(true);
    m_loadingSelect->setMinimumWidth(200);
    examplesGroupLayout->addWidget(m_loadingSelect);
    
    examplesGroupLayout->addStretch();
    examplesLayout->addWidget(examplesGroup);
    
    splitter->addWidget(examplesWidget);
    splitter->setSizes({300, 500});
}

void AntdSelectSettingsEditor::createSelectExamples()
{
    // This is handled in createControls
}

void AntdSelectSettingsEditor::connectSignals()
{
    // Connect settings controls to basic select
    connect(m_useThemeColorsCheck, &QCheckBox::toggled, m_basicSelect, &QtAntdSelect::setUseThemeColors);
    connect(m_allowClearCheck, &QCheckBox::toggled, m_basicSelect, &QtAntdSelect::setAllowClear);
    connect(m_disabledCheck, &QCheckBox::toggled, m_basicSelect, &QtAntdSelect::setDisabled);
    connect(m_loadingCheck, &QCheckBox::toggled, m_basicSelect, &QtAntdSelect::setLoading);
    connect(m_showSearchCheck, &QCheckBox::toggled, m_basicSelect, &QtAntdSelect::setShowSearch);
    connect(m_multipleCheck, &QCheckBox::toggled, m_basicSelect, &QtAntdSelect::setMultiple);
    connect(m_placeholderEdit, &QLineEdit::textChanged, m_basicSelect, &QtAntdSelect::setPlaceholder);
    
    connect(m_sizeCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), 
            [this](const QString &text) {
        QtAntdSelect::SelectSize size = QtAntdSelect::Medium;
        if (text == "Small") size = QtAntdSelect::Small;
        else if (text == "Large") size = QtAntdSelect::Large;
        m_basicSelect->setSelectSize(size);
    });
    
    // Connect basic select signals
    connect(m_basicSelect, &QtAntdSelect::selectionChanged, this, &AntdSelectSettingsEditor::onSelectionChanged);
    connect(m_basicSelect, &QtAntdSelect::valueSelected, this, &AntdSelectSettingsEditor::onValueSelected);
    connect(m_basicSelect, &QtAntdSelect::dropdownOpened, this, &AntdSelectSettingsEditor::onDropdownOpened);
    connect(m_basicSelect, &QtAntdSelect::dropdownClosed, this, &AntdSelectSettingsEditor::onDropdownClosed);
    connect(m_basicSelect, &QtAntdSelect::searchTextChanged, this, &AntdSelectSettingsEditor::onSearchTextChanged);
    
    // Connect other example selects for demonstration
    connect(m_searchSelect, &QtAntdSelect::selectionChanged, [this]() {
        m_statusLabel->setText("Search select changed: " + m_searchSelect->selectedText());
    });
    
    connect(m_multipleSelect, &QtAntdSelect::selectionChanged, [this]() {
        QStringList texts = m_multipleSelect->selectedTexts();
        m_statusLabel->setText(QString("Multiple select: %1 items").arg(texts.size()));
    });
    
    // Option management
    connect(m_addOptionButton, &QPushButton::clicked, [this]() {
        bool ok;
        QString label = QInputDialog::getText(this, "Add Option", "Option label:", QLineEdit::Normal, "", &ok);
        if (ok && !label.isEmpty()) {
            QString value = QInputDialog::getText(this, "Add Option", "Option value (optional):", QLineEdit::Normal, label, &ok);
            if (!ok) value = label;
            
            QtAntdSelectOption option(label, value);
            m_basicSelect->addOption(option);
            m_searchSelect->addOption(option);
            m_multipleSelect->addOption(option);
            m_disabledSelect->addOption(option);
            m_loadingSelect->addOption(option);
        }
    });
    
    connect(m_clearOptionsButton, &QPushButton::clicked, [this]() {
        m_basicSelect->clearOptions();
        m_searchSelect->clearOptions();
        m_multipleSelect->clearOptions();
        m_disabledSelect->clearOptions();
        m_loadingSelect->clearOptions();
        populateExampleData();
    });
}

void AntdSelectSettingsEditor::populateExampleData()
{
    QList<QtAntdSelectOption> options = {
        QtAntdSelectOption("Apple", "apple"),
        QtAntdSelectOption("Banana", "banana"),
        QtAntdSelectOption("Cherry", "cherry"),
        QtAntdSelectOption("Date", "date"),
        QtAntdSelectOption("Elderberry", "elderberry"),
        QtAntdSelectOption("Fig", "fig"),
        QtAntdSelectOption("Grape", "grape"),
        QtAntdSelectOption("Honeydew", "honeydew"),
    };
    
    m_basicSelect->addOptions(options);
    m_searchSelect->addOptions(options);
    m_multipleSelect->addOptions(options);
    m_disabledSelect->addOptions(options);
    m_loadingSelect->addOptions(options);
    
    // Set some initial selections
    m_disabledSelect->setValue("apple");
    m_multipleSelect->setValues({"banana", "cherry"});
}

void AntdSelectSettingsEditor::onSelectionChanged()
{
    QVariant value = m_basicSelect->value();
    QString text = m_basicSelect->selectedText();
    
    if (value.isValid()) {
        m_selectedValueLabel->setText(QString("Selected: %1 (value: %2)")
                                     .arg(text)
                                     .arg(value.toString()));
    } else {
        m_selectedValueLabel->setText("Selected: None");
    }
}

void AntdSelectSettingsEditor::onValueSelected(const QVariant &value)
{
    m_statusLabel->setText(QString("Value selected: %1").arg(value.toString()));
}

void AntdSelectSettingsEditor::onDropdownOpened()
{
    m_statusLabel->setText("Dropdown opened");
}

void AntdSelectSettingsEditor::onDropdownClosed()
{
    m_statusLabel->setText("Dropdown closed");
}

void AntdSelectSettingsEditor::onSearchTextChanged(const QString &text)
{
    m_statusLabel->setText(QString("Search: '%1'").arg(text));
}