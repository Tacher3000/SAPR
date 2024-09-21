#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QStackedWidget *stackWidget = new QStackedWidget(this);
    m_preprocessor = new Preprocessor(this);
    stackWidget->addWidget(m_preprocessor);

    setCentralWidget(stackWidget);


    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *mainMenu = new QMenu("Меню", this);
    QAction *openSettings = new QAction("Настройки", this);
    connect(openSettings, &QAction::triggered, this, &MainWindow::openSettings);
    mainMenu->addAction(openSettings);
    QAction *exitAction = new QAction("Выход", this);
    mainMenu->addAction(exitAction);
    menuBar->addMenu(mainMenu);

    QMenu *helpMenu = new QMenu("Справка", this);
    QAction *aboutAction = new QAction("О программе", this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
    helpMenu->addAction(aboutAction);
    menuBar->addMenu(helpMenu);

    setMenuBar(menuBar);
}

MainWindow::~MainWindow() {}


void MainWindow::openSettings()
{
    SettingsDialog settingsDialog(this);

    connect(&settingsDialog, &SettingsDialog::settingsSaved, m_preprocessor, &Preprocessor::updateScene);

    settingsDialog.exec();
}

void MainWindow::showAboutDialog()
{
    QMessageBox::information(this, "О программе", "Эта программа демонстрирует работу с QGraphicsScene.");
}
