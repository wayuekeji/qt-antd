#ifndef ANTDSELECTSETTINGSEDITOR_H
#define ANTDSELECTSETTINGSEDITOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <qtantdselect.h>

class AntdSelectSettingsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AntdSelectSettingsEditor(QWidget *parent = nullptr);

private slots:
    void onSelectionChanged();
    void onValueSelected(const QVariant &value);
    void onDropdownOpened();
    void onDropdownClosed();
    void onSearchTextChanged(const QString &text);

private:
    void createControls();
    void createSelectExamples();
    void connectSignals();
    void populateExampleData();

    // Settings controls
    QCheckBox *m_useThemeColorsCheck;
    QComboBox *m_sizeCombo;
    QCheckBox *m_allowClearCheck;
    QCheckBox *m_disabledCheck;
    QCheckBox *m_loadingCheck;
    QCheckBox *m_showSearchCheck;
    QCheckBox *m_multipleCheck;
    QLineEdit *m_placeholderEdit;
    QPushButton *m_addOptionButton;
    QPushButton *m_clearOptionsButton;

    // Example selects
    QtAntdSelect *m_basicSelect;
    QtAntdSelect *m_searchSelect;
    QtAntdSelect *m_multipleSelect;
    QtAntdSelect *m_disabledSelect;
    QtAntdSelect *m_loadingSelect;

    // Status labels
    QLabel *m_statusLabel;
    QLabel *m_selectedValueLabel;
};

#endif // ANTDSELECTSETTINGSEDITOR_H