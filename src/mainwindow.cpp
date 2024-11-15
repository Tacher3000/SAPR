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

    QAction *saveReportAction = new QAction("Сохранить отчет", this);
    connect(saveReportAction, &QAction::triggered, this, &MainWindow::saveReport);
    mainMenu->addAction(saveReportAction);

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
        if(m_stackWidget->currentWidget() == m_processor){
            m_processor->updateScene();
        }
    });
    connect(&settingsDialog, &SettingsDialog::settingsSaved, this, [this](){
        m_postProcessor->draw(m_processor, m_preprocessor->getSizeModel(), m_preprocessor->getNodeModel());
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

void MainWindow::saveReport()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить файл как", "", "All Files (*)");
    if (!filePath.isEmpty()) {

        // m_preprocessor->exportModelToPdf(filePath);
        // m_postProcessor->exportSceneToPdf(filePath);
        // mergePdfFiles("C:\\Users\\pyanc\\Desktop\\1231.pdf", "C:\\Users\\pyanc\\Desktop\\1232.pdf", filePath);
        saveReportPDF(filePath);
    }
}

void MainWindow::switchToProcessor()
{
    m_stackWidget->setCurrentWidget(m_processor);
    m_processor->setSizeModel(m_preprocessor->getSizeModel());
    m_processor->setNodeModel(m_preprocessor->getNodeModel());
    m_processor->calculate();
    m_processor->updateScene();
}

void MainWindow::switchToPreprocessor()
{
    m_stackWidget->setCurrentWidget(m_preprocessor);
}

void MainWindow::switchToPostprocessor()
{
    m_stackWidget->setCurrentWidget(m_postProcessor);
    m_postProcessor->draw(m_processor, m_preprocessor->getSizeModel(), m_preprocessor->getNodeModel());
}


void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
}

bool MainWindow::mergePdfFiles(const QString &file1Path, const QString &file2Path, const QString &outputFilePath) {
    QPdfDocument pdf1, pdf2;

    pdf1.load(file1Path);
    pdf2.load(file2Path);

    if (pdf1.status() == QPdfDocument::Status::Error || pdf2.status() == QPdfDocument::Status::Error) {
        qWarning("Could not load one of the PDF files.");
        return false;
    }

    QFile outputFile(outputFilePath);
    if (!outputFile.open(QIODevice::WriteOnly)) {
        qWarning("Could not open output file for writing.");
        return false;
    }

    QPdfWriter writer(&outputFile);
    writer.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&writer);

    QSize imageSize(writer.width() / 4, writer.height() / 4);

    for (int i = 0; i < pdf1.pageCount(); ++i) {
        QImage image = pdf1.render(i, imageSize);
        if (!image.isNull()) {
            painter.drawImage(QRectF(0, 0, writer.width(), writer.height()), image);
        }
        if (i < pdf1.pageCount() - 1) {
            writer.newPage();
        }
    }

    for (int i = 0; i < pdf2.pageCount(); ++i) {
        writer.newPage();
        QImage image = pdf2.render(i, imageSize);
        if (!image.isNull()) {
            painter.drawImage(QRectF(0, 0, writer.width(), writer.height()), image);
        }
    }

    painter.end();
    return true;
}


void MainWindow::saveReportPDF(const QString &filePath)
{
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setResolution(300);

    QPainter painter(&pdfWriter);

    auto printModel = [&](QAbstractItemModel *model, const QString &title, int &y, bool reduceRowCount = false) {
        int rowCount = model->rowCount();
        if (reduceRowCount) {
            rowCount--;
        }
        int columnCount = model->columnCount();

        int cellWidth = 300;
        int headerHeight = 120;
        int cellHeight = 80;

        int tableWidth = (columnCount + 1) * cellWidth;
        int pageWidth = pdfWriter.width();
        int pageHeight = pdfWriter.height();
        int x = (pageWidth - tableWidth) / 2;

        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.drawText(QRect(x, y, tableWidth, headerHeight), Qt::AlignCenter, title);
        y += headerHeight + 20;

        painter.setFont(QFont("Arial", 8, QFont::Bold));
        QRect rect(x, y, cellWidth, headerHeight);
        painter.drawRect(rect);
        painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "№");

        for (int col = 0; col < columnCount; ++col) {
            QRect rect(x + (col + 1) * cellWidth, y, cellWidth, headerHeight);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, model->headerData(col, Qt::Horizontal).toString());
        }
        y += headerHeight;

        painter.setFont(QFont("Arial", 8));
        for (int row = 0; row < rowCount; ++row) {
            if (y + cellHeight > pageHeight) {
                pdfWriter.newPage();
                y = 100;
            }

            QRect rect(x, y, cellWidth, cellHeight);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, QString::number(row + 1));

            for (int col = 0; col < columnCount; ++col) {
                QRect rect(x + (col + 1) * cellWidth, y, cellWidth, cellHeight);
                painter.drawRect(rect);
                painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, model->data(model->index(row, col)).toString());
            }
            y += cellHeight;
        }

        y += 50;
    };

    int y = 100;

    printModel(m_preprocessor->getSizeModel(), "Таблица стержней", y, true);
    printModel(m_preprocessor->getNodeModel(), "Таблица узлов", y);

    pdfWriter.newPage();
    m_preprocessor->getScene()->render(&painter);

    y = 100;

    pdfWriter.newPage();
    printModel(m_processor->getTableModel(), "Таблица расчетов", y);

    pdfWriter.newPage();
    m_postProcessor->getScene()->render(&painter);

    painter.end();
}
