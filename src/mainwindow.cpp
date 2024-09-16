#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    QHBoxLayout *mainLayput = new QHBoxLayout(centralWidget);

    model = new SizeTableModel;
    connect(model, &SizeTableModel::dataChanged, this, &MainWindow::updateScene);


    tableView = new QTableView(this);
    tableView->setModel(model);
    tableView->resizeRowsToContents();
    tableView->resizeColumnsToContents();
    splitter->addWidget(tableView);

    m_scene = new QGraphicsScene(this);
    m_view = new ScalableGraphicsView (this);
    m_view->setScene(m_scene);
    splitter->addWidget(m_view);

    tableView->setMinimumWidth(300);

    m_view->setMinimumWidth(300);

    QList<int> sizes;
    sizes << 300 << 400;
    splitter->setSizes(sizes);


    mainLayput->addWidget(splitter);
    setCentralWidget(centralWidget);


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

void MainWindow::updateScene()
{
    m_scene->clear();

    qreal currentX = 0;
    qreal maxHeight = 0;
    int rowCount = model->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = model->data(model->index(row, 0)).toInt() * 100;
        int height = model->data(model->index(row, 1)).toInt() * 50;
        int direction = model->data(model->index(row, 2)).toInt();
        QString support = model->data(model->index(row, 3)).toString();

        if(width == 0 || height == 0){
            continue;
        }

        if (direction != 0) {
            int arrowCount = width / 10;
            qreal arrowSpacing = width / arrowCount;
            qreal arrowY = 0;

            QLineF line(currentX, arrowY, currentX + width, arrowY);
            m_scene->addLine(line, QPen(Qt::red));

            for (int i = 0; i < arrowCount; ++i) {
                qreal arrowX = currentX + i * arrowSpacing;


                if (direction > 0) {
                    QLineF arrowHead1(arrowX + arrowSpacing, arrowY, arrowX + arrowSpacing - 5, arrowY - 5);
                    QLineF arrowHead2(arrowX + arrowSpacing, arrowY, arrowX + arrowSpacing - 5, arrowY + 5);
                    m_scene->addLine(arrowHead1, QPen(Qt::red));
                    m_scene->addLine(arrowHead2, QPen(Qt::red));
                } else {
                    QLineF arrowHead1(arrowX, arrowY, arrowX + 5, arrowY - 5);
                    QLineF arrowHead2(arrowX, arrowY, arrowX + 5, arrowY + 5);
                    m_scene->addLine(arrowHead1, QPen(Qt::red));
                    m_scene->addLine(arrowHead2, QPen(Qt::red));
                }
            }
        }

        if (height > maxHeight) {
            maxHeight = height;
        }

        QRectF rect(currentX, -height / 2, width, height);
        m_scene->addRect(rect, QPen(Qt::black));


        // Левая вертикальная опора
        if(row == 0 && !support.isEmpty()){
            qreal leftX = currentX;
            qreal topY = -height / 2 - 10;
            qreal bottomY = height / 2 + 10;

            m_scene->addLine(QLineF(leftX, topY, leftX, bottomY), QPen(Qt::green));

            int arrowCount = (height + 20) / 10;
            qreal arrowSpacing = (height + 20) / arrowCount;

            for (int i = 0; i <= arrowCount; ++i) {
                qreal arrowY = topY + i * arrowSpacing;

                QLineF arrowHead1(0, arrowY,  -5, arrowY + 5);
                m_scene->addLine(arrowHead1, QPen(Qt::green));
            }
        }
        // Правая вертикальная опора
        if(row == rowCount - 2 && !support.isEmpty()){
            qreal rightX = currentX + width;
            qreal topY = -height / 2 - 10;
            qreal bottomY = height / 2 + 10;
            m_scene->addLine(QLineF(rightX, topY, rightX, bottomY), QPen(Qt::green));

            int arrowCount = (height + 20) / 10;
            qreal arrowSpacing = (height + 20) / arrowCount;

            for (int i = 0; i <= arrowCount; ++i) {
                qreal arrowY = topY + i * arrowSpacing;

                QLineF arrowHead1(rightX, arrowY,  rightX + 5, arrowY - 5);
                m_scene->addLine(arrowHead1, QPen(Qt::green));
            }
        }

        currentX += width;
    }

    App* app = App::theApp();
    QSettings* settings = app->settings();
    if(settings->value("checkBoxNodeN", false).toBool())
    {
        drawNode(maxHeight);
    }
    if(settings->value("checkBoxKernelN", false).toBool())
    {
        drawKernel(maxHeight);
    }


}

void MainWindow::openSettings()
{
    SettingsDialog settingsDialog(this);

    connect(&settingsDialog, &SettingsDialog::settingsSaved, this, [this]() {
        updateScene();
    });

    settingsDialog.exec();
}

void MainWindow::showAboutDialog()
{
    QMessageBox::information(this, "О программе", "Эта программа демонстрирует работу с QGraphicsScene.");
}

// можно сделать красивее
void MainWindow::drawNode(qreal maxHeight)
{
    qreal currentX = 0;
    for (int row = 0; row < model->rowCount(); ++row) {
        int width = model->data(model->index(row, 0)).toInt() * 100;
        int height = model->data(model->index(row, 1)).toInt() * 100;
        if(width == 0 || height == 0){
            continue;
        }

        qreal numberX = currentX;
        qreal numberY = maxHeight / 2 + 10;

        QRectF square(numberX - 10, numberY, 20, 20);
        m_scene->addRect(square, QPen(Qt::black), QBrush(Qt::white));

        QString number = QString::number(row + 1);
        QGraphicsTextItem* textItem = m_scene->addText(number);
        QRectF textRect = textItem->boundingRect();
        textItem->setPos(numberX - textRect.width() / 2, numberY - textRect.height() / 2 + 10);

        currentX += width;

        {
            qreal numberX = currentX;

            QRectF square(numberX - 10, numberY, 20, 20);
            m_scene->addRect(square, QPen(Qt::black), QBrush(Qt::white));

            QString number = QString::number(row + 2);
            QGraphicsTextItem* textItem = m_scene->addText(number);
            QRectF textRect = textItem->boundingRect();
            textItem->setPos(numberX - textRect.width() / 2, numberY - textRect.height() / 2 + 10);
        }
    }
}

void MainWindow::drawKernel(qreal maxHeight)
{
    qreal currentX = 0;
    for (int row = 0; row < model->rowCount() - 1; ++row) {
        int width = model->data(model->index(row, 0)).toInt() * 100;
        int height = model->data(model->index(row, 1)).toInt() * 100;
        if(width == 0 || height == 0){
            continue;
        }

        qreal centerX = currentX + width / 2;
        qreal centerY = -maxHeight / 2 - 30;

        QRectF circle(centerX - 15, centerY - 15, 30, 30);
        m_scene->addEllipse(circle, QPen(Qt::black), QBrush(Qt::white));

        QString number = QString::number(row + 1);
        QGraphicsTextItem *text = m_scene->addText(number);
        QRectF textRect = text->boundingRect();
        text->setPos(centerX - textRect.width() / 2, centerY - textRect.height() / 2);

        currentX += width;
    }
}
