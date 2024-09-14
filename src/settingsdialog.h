#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include "App.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

signals:
    void settingsSaved();

private slots:
    void onSaveSettings();
    void onCancelSettings();
    void onLoadSettings();

private:
    QLabel *m_labelDisplay;
    QCheckBox *m_checkBoxNode;
    QCheckBox *m_checkBoxKernel;
    QDialogButtonBox *m_buttonBox;
    QVBoxLayout *m_layout;

    void saveSettings();
    void loadSettings();
};

#endif // SETTINGSDIALOG_H
