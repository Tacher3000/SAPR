#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    m_labelDisplay = new QLabel("Отображения", this);
    m_labelDisplay->setStyleSheet("font-size: 16px; font-weight: bold;");
    m_labelDisplay->setAlignment(Qt::AlignCenter);

    m_checkBoxKernel = new QCheckBox("Стержни", this);
    m_checkBoxNodeN = new QCheckBox("Номера узлов", this);
    m_checkBoxKernelN = new QCheckBox("Номера стержней", this);
    m_checkBoxDistributedLoad = new QCheckBox("Распределенная продольная нагрузка", this);
    m_checkBoxFocusedlLoad = new QCheckBox("Сосредоточенная продольная нагрузка", this);
    m_checkBoxSupport = new QCheckBox("Опора", this);

    m_labelColor = new QLabel("Цвет", this);
    m_labelColor->setStyleSheet("font-size: 16px; font-weight: bold;");
    m_labelColor->setAlignment(Qt::AlignCenter);

    m_buttonKernelColor = new QPushButton(this);
    m_buttonNodeColorN = new QPushButton(this);
    m_buttonKernelColorN = new QPushButton(this);
    m_buttonDistributedLoadColor = new QPushButton(this);
    m_buttonFocusedLoadColor = new QPushButton(this);
    m_buttonSupportColor = new QPushButton(this);

    m_labelDifferent = new QLabel("Разное", this);
    m_labelDifferent->setStyleSheet("font-size: 16px; font-weight: bold;");
    m_labelDifferent->setAlignment(Qt::AlignCenter);

    m_checkBoxWidget = new QCheckBox("Виджеты", this);
    m_labelFlyingTextCount = new QLabel("Количество летающего текста", this);
    m_spinBoxFlyingTextCount = new QSpinBox(this);
    m_spinBoxFlyingTextCount->setMinimumWidth(75);
    m_spinBoxFlyingTextCount->setRange(1, 100);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);

    m_layout = new QGridLayout(this);
    m_layout->addWidget(m_labelDisplay, 0, 0, 1, 1);
    m_layout->addWidget(m_labelColor, 0, 1, 1, 1);
    m_layout->addWidget(m_checkBoxKernel, 1, 0, 1, 1);
    m_layout->addWidget(m_buttonKernelColor, 1, 1, 1, 1);
    m_layout->addWidget(m_checkBoxNodeN, 2, 0, 1, 1);
    m_layout->addWidget(m_buttonNodeColorN, 2, 1, 1, 1);
    m_layout->addWidget(m_checkBoxKernelN, 3, 0, 1, 1);
    m_layout->addWidget(m_buttonKernelColorN, 3, 1, 1, 1);
    m_layout->addWidget(m_checkBoxDistributedLoad, 4, 0, 1, 1);
    m_layout->addWidget(m_buttonDistributedLoadColor, 4, 1, 1, 1);
    m_layout->addWidget(m_checkBoxFocusedlLoad, 5, 0, 1, 1);
    m_layout->addWidget(m_buttonFocusedLoadColor, 5, 1, 1, 1);
    m_layout->addWidget(m_checkBoxSupport, 6, 0, 1, 1);
    m_layout->addWidget(m_buttonSupportColor, 6, 1, 1, 1);
    m_layout->addWidget(m_buttonBox, 7, 0, 1, 2);

    m_layout->addWidget(m_labelDifferent, 0, 3, 1, 2);
    m_layout->addWidget(m_checkBoxWidget, 1, 3, 1, 2);
    m_layout->addWidget(m_spinBoxFlyingTextCount, 2, 3, 1, 1);
    m_layout->addWidget(m_labelFlyingTextCount, 2, 4, 1, 1);


    setLayout(m_layout);

    onLoadSettings();

    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::onSaveSettings);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::onCancelSettings);

    connect(m_buttonKernelColor, &QPushButton::clicked, this, &SettingsDialog::onKernelColorClicked);
    connect(m_buttonNodeColorN, &QPushButton::clicked, this, &SettingsDialog::onNodeColorNClicked);
    connect(m_buttonKernelColorN, &QPushButton::clicked, this, &SettingsDialog::onKernelColorNClicked);
    connect(m_buttonDistributedLoadColor, &QPushButton::clicked, this, &SettingsDialog::onDistributedLoadColorClicked);
    connect(m_buttonFocusedLoadColor, &QPushButton::clicked, this, &SettingsDialog::onFocusedLoadColorClicked);
    connect(m_buttonSupportColor, &QPushButton::clicked, this, &SettingsDialog::onSupportColorClicked);
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

    settings->setValue("checkBoxKernel", m_checkBoxKernel->isChecked());
    settings->setValue("checkBoxNodeN", m_checkBoxNodeN->isChecked());
    settings->setValue("checkBoxKernelN", m_checkBoxKernelN->isChecked());
    settings->setValue("checkBoxDistributedLoad", m_checkBoxDistributedLoad->isChecked());
    settings->setValue("checkBoxFocusedLoad", m_checkBoxFocusedlLoad->isChecked());
    settings->setValue("checkBoxSupport", m_checkBoxSupport->isChecked());
    settings->setValue("checkBoxWidget", m_checkBoxWidget->isChecked());

    settings->setValue("flyingTextCount", m_spinBoxFlyingTextCount->value());

    settings->setValue("kernelColor", m_kernelColor.name());
    settings->setValue("nodeColorN", m_nodeColorN.name());
    settings->setValue("kernelColorN", m_kernelColorN.name());
    settings->setValue("distributedLoadColor", m_distributedLoadColor.name());
    settings->setValue("FocusedLoadColor", m_focusedLoadColor.name());
    settings->setValue("supportColor", m_supportColor.name());
}

void SettingsDialog::loadSettings()
{
    App* app = App::theApp();
    QSettings* settings = app->settings();

    m_checkBoxKernel->setChecked(settings->value("checkBoxKernel", false).toBool());
    m_checkBoxNodeN->setChecked(settings->value("checkBoxNodeN", false).toBool());
    m_checkBoxKernelN->setChecked(settings->value("checkBoxKernelN", false).toBool());
    m_checkBoxDistributedLoad->setChecked(settings->value("checkBoxDistributedLoad", false).toBool());
    m_checkBoxFocusedlLoad->setChecked(settings->value("checkBoxFocusedLoad", false).toBool());
    m_checkBoxSupport->setChecked(settings->value("checkBoxSupport", false).toBool());
    m_checkBoxWidget->setChecked(settings->value("checkBoxWidget", false).toBool());

    m_spinBoxFlyingTextCount->setValue(settings->value("flyingTextCount", 1).toInt());

    m_kernelColor = QColor(settings->value("kernelColor", QColor(Qt::black).name()).toString());
    m_nodeColorN = QColor(settings->value("nodeColorN", QColor(Qt::black).name()).toString());
    m_kernelColorN = QColor(settings->value("kernelColorN", QColor(Qt::black).name()).toString());
    m_distributedLoadColor = QColor(settings->value("distributedLoadColor", QColor(Qt::black).name()).toString());
    m_focusedLoadColor = QColor(settings->value("FocusedLoadColor", QColor(Qt::black).name()).toString());
    m_supportColor = QColor(settings->value("supportColor", QColor(Qt::black).name()).toString());

    updateButtonColor(m_buttonKernelColor, m_kernelColor);
    updateButtonColor(m_buttonNodeColorN, m_nodeColorN);
    updateButtonColor(m_buttonKernelColorN, m_kernelColorN);
    updateButtonColor(m_buttonDistributedLoadColor, m_distributedLoadColor);
    updateButtonColor(m_buttonFocusedLoadColor, m_focusedLoadColor);
    updateButtonColor(m_buttonSupportColor, m_supportColor);
}

void SettingsDialog::onKernelColorClicked()
{
    QColor color = QColorDialog::getColor(m_kernelColor, this, "Выбор цвета для стержней");
    if (color.isValid()) {
        m_kernelColor = color;
        updateButtonColor(m_buttonKernelColor, m_kernelColor);
    }
}

void SettingsDialog::onNodeColorNClicked()
{
    QColor color = QColorDialog::getColor(m_nodeColorN, this, "Выбор цвета для номера узлов");
    if (color.isValid()) {
        m_nodeColorN = color;
        updateButtonColor(m_buttonNodeColorN, m_nodeColorN);
    }
}

void SettingsDialog::onKernelColorNClicked()
{
    QColor color = QColorDialog::getColor(m_kernelColorN, this, "Выбор цвета для номера стержней");
    if (color.isValid()) {
        m_kernelColorN = color;
        updateButtonColor(m_buttonKernelColorN, m_kernelColorN);
    }
}

void SettingsDialog::onDistributedLoadColorClicked()
{
    QColor color = QColorDialog::getColor(m_distributedLoadColor, this, "Выбор цвета для распределенной нагрузки");
    if (color.isValid()) {
        m_distributedLoadColor = color;
        updateButtonColor(m_buttonDistributedLoadColor, m_distributedLoadColor);
    }
}

void SettingsDialog::onFocusedLoadColorClicked()
{
    QColor color = QColorDialog::getColor(m_focusedLoadColor, this, "Выбор цвета для продольной нагрузки");
    if (color.isValid()) {
        m_focusedLoadColor = color;
        updateButtonColor(m_buttonFocusedLoadColor, m_focusedLoadColor);
    }
}

void SettingsDialog::onSupportColorClicked()
{
    QColor color = QColorDialog::getColor(m_supportColor, this, "Выбор цвета для опор");
    if (color.isValid()) {
        m_supportColor = color;
        updateButtonColor(m_buttonSupportColor, m_supportColor);
    }
}

void SettingsDialog::updateButtonColor(QPushButton* button, const QColor& color)
{
    QString style = QString("background-color: %1").arg(color.name());
    button->setStyleSheet(style);
}
