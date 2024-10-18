#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_stackWidget = new QStackedWidget(this);

    m_preprocessor = new Preprocessor(this);
    connect(m_preprocessor, &Preprocessor::clickedToProcessor, this, &MainWindow::switchToProcessor);
    m_stackWidget->addWidget(m_preprocessor);

    m_processor = new Processor(this);
    m_stackWidget->addWidget(m_processor);
    connect(m_processor, &Processor::clickedToPreprocessor, this, &MainWindow::switchToPreprocessor);
    connect(m_processor, &Processor::clickedToPostprocessor, this, &MainWindow::switchToPostprocessor);
    setCentralWidget(m_stackWidget);

    m_postProcessor = new PostProcessor(this);
    connect(m_postProcessor, &PostProcessor::clickedToProcessor, this, &MainWindow::switchToProcessor);
    m_stackWidget->addWidget(m_postProcessor);

    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *mainMenu = new QMenu("Меню", this);
    QAction *openSettings = new QAction("Настройки", this);
    connect(openSettings, &QAction::triggered, this, &MainWindow::openSettings);
    mainMenu->addAction(openSettings);

    QAction *clearAction = new QAction("Очистить", this);
    connect(clearAction, &QAction::triggered, m_preprocessor, &Preprocessor::clearData);
    mainMenu->addAction(clearAction);

    QAction *loadAction = new QAction("Загрузить", this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadFile);
    mainMenu->addAction(loadAction);

    QAction *saveAction = new QAction("Сохранить", this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    mainMenu->addAction(saveAction);

    QAction *saveAsAction = new QAction("Сохранить как", this);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    mainMenu->addAction(saveAsAction);

    QAction *exitAction = new QAction("Выход", this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    mainMenu->addAction(exitAction);
    menuBar->addMenu(mainMenu);

    QMenu *helpMenu = new QMenu("Справка", this);
    QAction *aboutAction = new QAction("О программе", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    helpMenu->addAction(aboutAction);
    menuBar->addMenu(helpMenu);


    setMenuBar(menuBar);
}



void MainWindow::openSettings()
{
    SettingsDialog settingsDialog(this);

    connect(&settingsDialog, &SettingsDialog::settingsSaved, m_preprocessor, &Preprocessor::updateScene);
    connect(&settingsDialog, &SettingsDialog::settingsSaved, this, [this](){
        m_postProcessor->draw(m_preprocessor->getSizeModel(), m_preprocessor->getNodeModel());
    });

    settingsDialog.exec();
}

void MainWindow::showAboutDialog()
{
    QMessageBox::information(this, "О программе", "Эта программа SAPR3000(остальное позже)");
}


void MainWindow::loadFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Загрузить файл", "", "All Files (*)");
    if (!filePath.isEmpty()) {
        m_preprocessor->loadModels(filePath);
        m_currentFilePath = filePath;
    }
}

void MainWindow::saveFile()
{
    QString filePath = m_currentFilePath;
    if (!filePath.isEmpty()) {
        m_preprocessor->saveModels(filePath);
    } else {
        saveAsFile();
    }
}

void MainWindow::saveAsFile()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить файл как", "", "All Files (*)");
    if (!filePath.isEmpty()) {
        m_preprocessor->saveModels(filePath);
        m_currentFilePath = filePath;
    }
}

void MainWindow::switchToProcessor()
{
    m_stackWidget->setCurrentWidget(m_processor);
    m_processor->calculate(m_preprocessor->getSizeModel(), m_preprocessor->getNodeModel());
}

void MainWindow::switchToPreprocessor()
{
    m_stackWidget->setCurrentWidget(m_preprocessor);
}

void MainWindow::switchToPostprocessor()
{
    m_stackWidget->setCurrentWidget(m_postProcessor);
    m_postProcessor->draw(m_preprocessor->getSizeModel(), m_preprocessor->getNodeModel());
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
}
