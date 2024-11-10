#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QColor>
#include <QMessagebox.h>
#include <QColorDialog>
#include <QSpinBox>
#include "app.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
    void updateButtonColor(QPushButton *button, const QColor &color);

signals:
    void settingsSaved();

private slots:
    void onSaveSettings();
    void onCancelSettings();
    void onLoadSettings();

    void onKernelColorClicked();
    void onNodeColorNClicked();
    void onKernelColorNClicked();
    void onDistributedLoadColorClicked();
    void onFocusedLoadColorClicked();
    void onSupportColorClicked();


private:
    QLabel *m_labelDisplay;
    QLabel *m_labelColor;
    QLabel *m_labelDifferent;

    QLabel *m_labelFlyingTextCount;

    QCheckBox *m_checkBoxKernel;
    QCheckBox *m_checkBoxNodeN;
    QCheckBox *m_checkBoxKernelN;
    QCheckBox *m_checkBoxDistributedLoad;
    QCheckBox *m_checkBoxFocusedlLoad;
    QCheckBox *m_checkBoxSupport;

    QCheckBox *m_checkBoxWidget;

    QDialogButtonBox *m_buttonBox;

    QGridLayout *m_layout;

    QPushButton *m_buttonKernelColor;
    QPushButton *m_buttonNodeColorN;
    QPushButton *m_buttonKernelColorN;
    QPushButton *m_buttonDistributedLoadColor;
    QPushButton *m_buttonFocusedLoadColor;
    QPushButton *m_buttonSupportColor;

    QSpinBox *m_spinBoxFlyingTextCount;

    QColor m_kernelColor;
    QColor m_nodeColorN;
    QColor m_kernelColorN;
    QColor m_distributedLoadColor;
    QColor m_focusedLoadColor;
    QColor m_supportColor;

    void saveSettings();
    void loadSettings();
};

#endif // SETTINGSDIALOG_H
