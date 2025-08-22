#ifndef ANTDBUTTONSETTINGSEDITOR_H
#define ANTDBUTTONSETTINGSEDITOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QCheckBox;
class QComboBox;
class QPushButton;
class QLabel;
class QGroupBox;
QT_END_NAMESPACE

class QtAntdButton;

class AntdButtonSettingsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AntdButtonSettingsEditor(QWidget *parent = nullptr);

private slots:
    void onTypeChanged();
    void onSizeChanged();
    void onShapeChanged();
    void onBlockToggled(bool block);
    void onGhostToggled(bool ghost);
    void onDangerToggled(bool danger);
    void onLoadingToggled(bool loading);
    void onThemeToggled(bool useTheme);

private:
    void createButtonTypeGroup();
    void createButtonSizeGroup();
    void createButtonShapeGroup();
    void createButtonOptionsGroup();
    void createPreviewGroup();
    void setupLayout();

    QVBoxLayout *mainLayout;
    
    QGroupBox *typeGroup;
    QComboBox *typeCombo;
    
    QGroupBox *sizeGroup;
    QComboBox *sizeCombo;
    
    QGroupBox *shapeGroup;
    QComboBox *shapeCombo;
    
    QGroupBox *optionsGroup;
    QCheckBox *blockCheckBox;
    QCheckBox *ghostCheckBox;
    QCheckBox *dangerCheckBox;
    QCheckBox *loadingCheckBox;
    QCheckBox *useThemeCheckBox;
    
    QGroupBox *previewGroup;
    QtAntdButton *previewButton;
    QtAntdButton *iconButton;
    QtAntdButton *textOnlyButton;
};

#endif // ANTDBUTTONSETTINGSEDITOR_H