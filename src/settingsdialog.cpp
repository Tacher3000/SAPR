#include "SettingsDialog.h"
#include <qmessagebox.h>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_labelDisplay = new QLabel("Отображения", this);
    m_checkBoxNode = new QCheckBox("Номера узлов", this);
    m_checkBoxKernel = new QCheckBox("Номера стержней ", this);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);

    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_labelDisplay);
    m_layout->addWidget(m_checkBoxNode);
    m_layout->addWidget(m_checkBoxKernel);
    m_layout->addWidget(m_buttonBox);

    setLayout(m_layout);

    onLoadSettings();

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveSettings);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onCancelSettings);
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::onSaveSettings()
{
    saveSettings();
    emit settingsSaved();
    accept();
}

void SettingsDialog::onCancelSettings()
{
    reject();
}


void SettingsDialog::onLoadSettings()
{
    loadSettings();
}

void SettingsDialog::saveSettings()
{
    App* app = App::theApp();
    QSettings* settings = app->settings();

    settings->setValue("checkBoxNode", m_checkBoxNode->isChecked());
    settings->setValue("checkBoxKernel", m_checkBoxKernel->isChecked());
}

void SettingsDialog::loadSettings()
{
    App* app = App::theApp();
    QSettings* settings = app->settings();

    m_checkBoxNode->setChecked(settings->value("checkBoxNode", false).toBool());
    m_checkBoxKernel->setChecked(settings->value("checkBoxKernel", false).toBool());
}
