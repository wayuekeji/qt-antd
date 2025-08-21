#ifndef AntdANTDDIVIDERSETTINGSEDITOR_H
#define AntdANTDDIVIDERSETTINGSEDITOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QCheckBox;
class QSpinBox;
class QComboBox;
class QPushButton;
class QLineEdit;
class QLabel;
QT_END_NAMESPACE

class QtAntdDivider;

class AntdDividerSettingsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AntdDividerSettingsEditor(QWidget *parent = 0);
    ~AntdDividerSettingsEditor();

protected slots:
    void setupForm();
    void updateWidget();
    void selectLineColor();
    void selectTextColor();

private:
    QtAntdDivider *const m_divider1;
    QtAntdDivider *const m_divider2;
    QtAntdDivider *const m_divider3;
    QtAntdDivider *m_divider4; // Divider with Left Text
    
    QVBoxLayout *m_settingsLayout;
    QCheckBox *m_disabledCheckBox;
    QCheckBox *m_useThemeColorsCheckBox;
    QLineEdit *m_textLineEdit;
    QComboBox *m_textAlignmentComboBox;
    QComboBox *m_lineStyleComboBox;
    QSpinBox *m_thicknessSpinBox;
    QPushButton *m_lineColorButton;
    QPushButton *m_textColorButton;
};

#endif // AntdANTDDIVIDERSETTINGSEDITOR_H
