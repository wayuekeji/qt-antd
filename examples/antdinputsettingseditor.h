#ifndef ANTDINPUTSETTINGSEDITOR_H
#define ANTDINPUTSETTINGSEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class QtAntdInput;

class AntdInputSettingsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AntdInputSettingsEditor(QWidget *parent = nullptr);

private slots:
    void onSizeChanged();
    void onStatusChanged();
    void onClearButtonToggled(bool enabled);
    void onThemeToggled(bool useTheme);
    void onPrefixChanged();
    void onSuffixChanged();
    void onPlaceholderChanged();
    void onDisabledToggled(bool disabled);
    void onReadOnlyToggled(bool readOnly);

private:
    void createSizeGroup();
    void createStatusGroup();
    void createOptionsGroup();
    void createContentGroup();
    void createPreviewGroup();
    void setupLayout();

    QVBoxLayout *mainLayout;
    
    // Size group
    QGroupBox *sizeGroup;
    QComboBox *sizeCombo;
    
    // Status group
    QGroupBox *statusGroup;
    QComboBox *statusCombo;
    
    // Options group
    QGroupBox *optionsGroup;
    QCheckBox *clearButtonCheck;
    QCheckBox *useThemeCheck;
    QCheckBox *disabledCheck;
    QCheckBox *readOnlyCheck;
    
    // Content group
    QGroupBox *contentGroup;
    QLineEdit *placeholderEdit;
    QLineEdit *prefixEdit;
    QLineEdit *suffixEdit;
    
    // Preview group
    QGroupBox *previewGroup;
    QtAntdInput *basicInput;
    QtAntdInput *prefixInput;
    QtAntdInput *suffixInput;
    QtAntdInput *clearableInput;
};

#endif // ANTDINPUTSETTINGSEDITOR_H