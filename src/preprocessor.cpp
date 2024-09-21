#include "preprocessor.h"

Preprocessor::Preprocessor(QWidget *parent) {
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    QHBoxLayout *mainLayput = new QHBoxLayout(this);

    model = new SizeTableModel;
    connect(model, &SizeTableModel::dataChanged, this, &Preprocessor::updateScene);


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
    // setLayout(QSplitter);
}

Preprocessor::~Preprocessor()
{

}

void Preprocessor::updateScene()
{
    m_scene->clear();

    App* app = App::theApp();
    QSettings* settings = app->settings();

    qreal maxHeight = 50 * model->getMaxSection();

    if(settings->value("checkBoxKernel", false).toBool())
    {
        drawKernel();
    }
    if(settings->value("checkBoxNodeN", false).toBool())
    {
        drawNode(maxHeight);
    }
    if(settings->value("checkBoxKernelN", false).toBool())
    {
        drawKernelN(maxHeight);
    }if(settings->value("checkBoxDistributedLoad", false).toBool())
    {
        drawDistributedLoad();
    }if(settings->value("checkBoxSupport", false).toBool())
    {
        drawSupport();
    }


}

void Preprocessor::openSettings()
{
    SettingsDialog settingsDialog(this);

    connect(&settingsDialog, &SettingsDialog::settingsSaved, this, [this]() {
        updateScene();
    });

    settingsDialog.exec();
}

void Preprocessor::showAboutDialog()
{
    QMessageBox::information(this, "О программе", "Эта программа демонстрирует работу с QGraphicsScene.");
}

void Preprocessor::drawKernel()
{
    App* app = App::theApp();
    QSettings* settings = app->settings();
    qreal currentX = 0;
    qreal maxHeight = 50 * model->getMaxSection();
    int rowCount = model->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = model->data(model->index(row, 0)).toInt() * 100;
        int height = model->data(model->index(row, 1)).toInt() * 50;

        if(width == 0 || height == 0){
            continue;
        }

        QString kernelString = settings->value("kernelColor", QColor(Qt::black).name()).toString();
        QColor kernelColor(kernelString);

        QRectF rect(currentX, -height / 2, width, height);
        m_scene->addRect(rect, QPen(kernelColor));

        currentX += width;
    }

}

// можно сделать красивее
void Preprocessor::drawNode(qreal maxHeight)
{
    App* app = App::theApp();
    QSettings* settings = app->settings();

    QString nodeNString = settings->value("nodeColorN", QColor(Qt::black).name()).toString();
    QColor nodeColorN(nodeNString);

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
        m_scene->addRect(square, QPen(Qt::black), QBrush(nodeColorN));

        QString number = QString::number(row + 1);
        QGraphicsTextItem* textItem = m_scene->addText(number);
        QRectF textRect = textItem->boundingRect();
        textItem->setPos(numberX - textRect.width() / 2, numberY - textRect.height() / 2 + 10);

        currentX += width;

        {
            qreal numberX = currentX;

            QRectF square(numberX - 10, numberY, 20, 20);
            m_scene->addRect(square, QPen(Qt::black), QBrush(nodeColorN));

            QString number = QString::number(row + 2);
            QGraphicsTextItem* textItem = m_scene->addText(number);
            QRectF textRect = textItem->boundingRect();
            textItem->setPos(numberX - textRect.width() / 2, numberY - textRect.height() / 2 + 10);
        }
    }
}



void Preprocessor::drawKernelN(qreal maxHeight)
{
    App* app = App::theApp();
    QSettings* settings = app->settings();

    QString kernelNString = settings->value("kernelColorN", QColor(Qt::black).name()).toString();
    QColor kernelColorN(kernelNString);

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
        m_scene->addEllipse(circle, QPen(Qt::black), QBrush(kernelColorN));

        QString number = QString::number(row + 1);
        QGraphicsTextItem *text = m_scene->addText(number);
        QRectF textRect = text->boundingRect();
        text->setPos(centerX - textRect.width() / 2, centerY - textRect.height() / 2);

        currentX += width;
    }
}


void Preprocessor::drawDistributedLoad()
{
    App* app = App::theApp();
    QSettings* settings = app->settings();

    qreal currentX = 0;
    int rowCount = model->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = model->data(model->index(row, 0)).toInt() * 100;
        int height = model->data(model->index(row, 1)).toInt() * 50;
        int direction = model->data(model->index(row, 2)).toInt();

        if(width == 0 || height == 0){
            continue;
        }

        if (direction != 0) {
            QString distributedLoadString = settings->value("distributedLoadColor", QColor(Qt::black).name()).toString();
            QColor distributedLoadColor(distributedLoadString);

            int arrowCount = width / 10;
            qreal arrowSpacing = width / arrowCount;
            qreal arrowY = 0;

            QLineF line(currentX, arrowY, currentX + width, arrowY);
            m_scene->addLine(line, QPen(QColor(distributedLoadColor)));

            for (int i = 0; i < arrowCount; ++i) {
                qreal arrowX = currentX + i * arrowSpacing;


                if (direction > 0) {
                    QLineF arrowHead1(arrowX + arrowSpacing, arrowY, arrowX + arrowSpacing - 5, arrowY - 5);
                    QLineF arrowHead2(arrowX + arrowSpacing, arrowY, arrowX + arrowSpacing - 5, arrowY + 5);
                    m_scene->addLine(arrowHead1, QPen(QColor(distributedLoadColor)));
                    m_scene->addLine(arrowHead2, QPen(QColor(distributedLoadColor)));
                } else {
                    QLineF arrowHead1(arrowX, arrowY, arrowX + 5, arrowY - 5);
                    QLineF arrowHead2(arrowX, arrowY, arrowX + 5, arrowY + 5);
                    m_scene->addLine(arrowHead1, QPen(QColor(distributedLoadColor)));
                    m_scene->addLine(arrowHead2, QPen(QColor(distributedLoadColor)));
                }
            }
        }

        currentX += width;
    }
}

void Preprocessor::drawSupport()
{
    App* app = App::theApp();
    QSettings* settings = app->settings();

    qreal currentX = 0;
    int rowCount = model->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = model->data(model->index(row, 0)).toInt() * 100;
        int height = model->data(model->index(row, 1)).toInt() * 50;
        QString support = model->data(model->index(row, 3)).toString();

        if(width == 0 || height == 0){
            continue;
        }

        QString supportString = settings->value("supportColor", QColor(Qt::black).name()).toString();
        QColor supportColor(supportString);

        // Левая вертикальная опора
        if(row == 0 && !support.isEmpty()){
            qreal leftX = currentX;
            qreal topY = -height / 2 - 10;
            qreal bottomY = height / 2 + 10;

            m_scene->addLine(QLineF(leftX, topY, leftX, bottomY), QPen(supportColor));

            int arrowCount = (height + 20) / 10;
            qreal arrowSpacing = (height + 20) / arrowCount;

            for (int i = 0; i <= arrowCount; ++i) {
                qreal arrowY = topY + i * arrowSpacing;

                QLineF arrowHead1(0, arrowY,  -5, arrowY + 5);
                m_scene->addLine(arrowHead1, QPen(supportColor));
            }
        }
        // Правая вертикальная опора
        if(row == rowCount - 2 && !support.isEmpty()){
            qreal rightX = currentX + width;
            qreal topY = -height / 2 - 10;
            qreal bottomY = height / 2 + 10;
            m_scene->addLine(QLineF(rightX, topY, rightX, bottomY), QPen(supportColor));

            int arrowCount = (height + 20) / 10;
            qreal arrowSpacing = (height + 20) / arrowCount;

            for (int i = 0; i <= arrowCount; ++i) {
                qreal arrowY = topY + i * arrowSpacing;

                QLineF arrowHead1(rightX, arrowY,  rightX + 5, arrowY - 5);
                m_scene->addLine(arrowHead1, QPen(supportColor));
            }
        }

        currentX += width;
    }
}
