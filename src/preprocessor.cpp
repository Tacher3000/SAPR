#include "preprocessor.h"

Preprocessor::Preprocessor(QWidget *parent) {
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    QHBoxLayout *mainLayput = new QHBoxLayout(this);
    QVBoxLayout *tablesLayput = new QVBoxLayout(this);
    QWidget *tablesWidget = new QWidget(this);

    m_sizeModel = new SizeTableModel;
    connect(m_sizeModel, &SizeTableModel::dataChanged, this, &Preprocessor::updateNodeModel);
    m_sizeTableView = new QTableView(this);
    m_sizeTableView->setModel(m_sizeModel);
    m_sizeTableView->resizeRowsToContents();
    m_sizeTableView->resizeColumnsToContents();
    tablesLayput->addWidget(m_sizeTableView);

    m_nodeModel = new NodeModel;
    connect(m_nodeModel, &SizeTableModel::dataChanged, this, &Preprocessor::updateScene);
    m_nodeTableView = new QTableView(this);
    m_nodeTableView->setModel(m_nodeModel);
    m_nodeTableView->resizeRowsToContents();
    m_nodeTableView->resizeColumnsToContents();
    tablesLayput->addWidget(m_nodeTableView);

    tablesWidget->setLayout(tablesLayput);
    splitter->addWidget(tablesWidget);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 1450, 900);

    m_view = new ScalableGraphicsView (this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setFrameStyle(0);
    m_view->setScene(m_scene);
    splitter->addWidget(m_view);

    tablesWidget->setMinimumWidth(400);

    m_view->setMinimumWidth(400);

    QList<int> sizes;
    sizes << 400 << 1500;
    splitter->setSizes(sizes);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, m_scene, &QGraphicsScene::advance);
    m_timer->start(16);

    flyText();

    mainLayput->addWidget(splitter);
}

Preprocessor::~Preprocessor()
{

}

void Preprocessor::flyText()
{
    App* app = App::theApp();
    QSettings* settings = app->settings();
    int count = settings->value("flyingTextCount", 1).toInt();

    for (int i = 0; i < count; ++i) {
        BouncingText* textItem = new BouncingText("SAPR3000");
        QFont font = textItem->font();
        font.setPointSize(20);
        textItem->setFont(font);

        qreal startX = QRandomGenerator::global()->bounded(m_scene->width() - textItem->boundingRect().width());
        qreal startY = QRandomGenerator::global()->bounded(m_scene->height() - textItem->boundingRect().height());
        textItem->setPos(startX, startY);

        m_scene->addItem(textItem);
    }
}

void Preprocessor::updateNodeModel()
{
    m_nodeModel->setRowCount(m_sizeModel->rowCount());
    updateScene();
}

void Preprocessor::updateScene()
{
    m_scene->clear();
    m_scene->setSceneRect(0, 0, 0, 0);


    if(m_nodeModel->isEmpty() && m_sizeModel->isEmpty()){
        m_scene->setSceneRect(0, 0, 1450, 900);
        flyText();
        return;
    }


    struct DrawFunction {
        QString setting;
        std::function<void()> function;
    };

    App* app = App::theApp();
    QSettings* settings = app->settings();
    qreal maxHeight = RECT_HEIGHT_MULTIPLIER * m_sizeModel->getMaxSection();

    if(settings->value("checkBoxWidget", false).toBool()){
        std::vector<DrawFunction> drawFunctions = {
                                                    {"checkBoxFocusedLoad", std::bind(&Preprocessor::drawFocusedLoad, this)},
                                                    {"checkBoxKernelN", std::bind(&Preprocessor::drawKernelN, this, maxHeight)},
                                                    {"checkBoxKernel", std::bind(&Preprocessor::drawKernelWidget, this)},
                                                    {"checkBoxDistributedLoad", std::bind(&Preprocessor::drawDistributedLoadWidget, this)},
                                                    {"checkBoxSupport", std::bind(&Preprocessor::drawSupport, this)},
                                                    {"checkBoxNodeN", std::bind(&Preprocessor::drawNode, this, maxHeight)},
                                                    };
        for (const auto& drawFunction : drawFunctions) {
            if (settings->value(drawFunction.setting, false).toBool()) {
                drawFunction.function();
            }
        }
    }else{
        std::vector<DrawFunction> drawFunctions = {
                                                    {"checkBoxFocusedLoad", std::bind(&Preprocessor::drawFocusedLoad, this)},
                                                    {"checkBoxKernelN", std::bind(&Preprocessor::drawKernelN, this, maxHeight)},
                                                    {"checkBoxDistributedLoad", std::bind(&Preprocessor::drawDistributedLoad, this)},
                                                    {"checkBoxKernel", std::bind(&Preprocessor::drawKernel, this)},
                                                    {"checkBoxSupport", std::bind(&Preprocessor::drawSupport, this)},
                                                    {"checkBoxNodeN", std::bind(&Preprocessor::drawNode, this, maxHeight)},
                                                    };
        for (const auto& drawFunction : drawFunctions) {
            if (settings->value(drawFunction.setting, false).toBool()) {
                drawFunction.function();
            }
        }
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
    int rowCount = m_sizeModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;

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

void Preprocessor::drawKernelWidget() {
    qreal currentX = 0;
    int rowCount = m_sizeModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;

        if(width == 0 || height == 0) {
            continue;
        }

        QTextEdit* textField = new QTextEdit();
        textField->setFixedSize(width, height);
        textField->setText(QString("Row %1").arg(row + 1));

        QGraphicsProxyWidget* proxyWidget = m_scene->addWidget(textField);

        proxyWidget->setPos(currentX, -height / 2);

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
    for (int row = 0; row < m_sizeModel->rowCount(); ++row) {
        int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
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
    for (int row = 0; row < m_sizeModel->rowCount() - 1; ++row) {
        int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
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
    int rowCount = m_sizeModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
        int direction = m_sizeModel->data(m_sizeModel->index(row, 2)).toInt();

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

//доделать минус
void Preprocessor::drawDistributedLoadWidget()
{
    qreal currentX = 0;
    int rowCount = m_sizeModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
        int direction = m_sizeModel->data(m_sizeModel->index(row, 2)).toInt();

        if(width == 0 || height == 0) {
            continue;
        }
        if (direction != 0) {
            QProgressBar* progressBar = new QProgressBar();
            progressBar->setRange(0, 100);
            progressBar->setValue(0);
            progressBar->setTextVisible(true);
            progressBar->setFixedWidth(width);
            progressBar->setFixedHeight(15);
            progressBar->setStyleSheet(
                "QProgressBar {"
                "   border: 2px solid grey;"
                "   border-radius: 5px;"
                "   text-align: center;"
                "}"
                "QProgressBar::chunk {"
                "   background-color: #00FF00;"
                "   width: 20px;"
                "}"
                );


            QGraphicsProxyWidget* proxy = m_scene->addWidget(progressBar);
            proxy->setPos(currentX, -7.5);

            QPropertyAnimation* animation = new QPropertyAnimation(progressBar, "value");
            animation->setDuration(2000);
            animation->setStartValue(0);
            animation->setEndValue(100);
            animation->setLoopCount(-1);
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }

        currentX += width;
    }
}


//можно сделать красивее код(говнокод)
void Preprocessor::drawSupport()
{
    App* app = App::theApp();
    QSettings* settings = app->settings();

    qreal currentX = 0;
    int rowCount = m_nodeModel->rowCount();
    int lastHeight = 0;

    for (int row = 0; row < rowCount; ++row) {
        int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
        QString support = m_nodeModel->data(m_nodeModel->index(row, 1)).toString();

        if(height != 0){
            lastHeight = height;
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
        if((row == (rowCount - 1)) && !support.isEmpty()){
            qreal rightX = currentX + width;
            qreal topY = -lastHeight / 2 - 10;
            qreal bottomY = lastHeight / 2 + 10;
            m_scene->addLine(QLineF(rightX, topY, rightX, bottomY), QPen(supportColor));

            int arrowCount = (lastHeight + 20) / 10;
            qreal arrowSpacing = (lastHeight + 20) / arrowCount;

            for (int i = 0; i <= arrowCount; ++i) {
                qreal arrowY = topY + i * arrowSpacing;

                QLineF arrowHead1(rightX, arrowY,  rightX + 5, arrowY - 5);
                m_scene->addLine(arrowHead1, QPen(supportColor));
            }
        }

        currentX += width;
    }
}

void Preprocessor::drawFocusedLoad()
{
    App* app = App::theApp();
    QSettings* settings = app->settings();

    QString FocusedColorString = settings->value("focusedLoadColor", QColor(Qt::black).name()).toString();
    QColor focusedColor(FocusedColorString);

    qreal currentX = 0;

    int rowCount = m_sizeModel->rowCount();

    qreal arrowY = 0;
    qreal arrowX = 0;

    for (int row = 0; row < rowCount; ++row) {
        int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int focusedDirection = m_nodeModel->data(m_nodeModel->index(row, 0)).toInt();
        int direction = m_sizeModel->data(m_sizeModel->index(row, 2)).toInt();

        if (focusedDirection != 0) {
            // if(direction != 0){
            //     m_scene->addLine(currentX, arrowY, currentX + (focusedDirection > 0 ? 50 : -50), arrowY, QPen(focusedColor));

            //     m_scene->addLine(currentX + (focusedDirection > 0 ? 50 : -50), arrowY, currentX + (focusedDirection > 0 ? 20 : -20), arrowY - 15, QPen(focusedColor));
            //     m_scene->addLine(currentX + (focusedDirection > 0 ? 50 : -50), arrowY, currentX + (focusedDirection > 0 ? 20 : -20), arrowY + 15, QPen(focusedColor));
            // }else{
                m_scene->addLine(currentX, arrowY, currentX + (focusedDirection > 0 ? 50 : -50), arrowY, QPen(focusedColor));

                m_scene->addLine(currentX + (focusedDirection > 0 ? 50 : -50), arrowY, currentX + (focusedDirection > 0 ? 20 : -20), arrowY - 15, QPen(focusedColor));
                m_scene->addLine(currentX + (focusedDirection > 0 ? 50 : -50), arrowY, currentX + (focusedDirection > 0 ? 20 : -20), arrowY + 15, QPen(focusedColor));
            // }


        }


        currentX += width;
    }
}

void Preprocessor::saveModels(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);

        int sizeRowCount = m_sizeModel->rowCount();
        int sizeColCount = m_sizeModel->columnCount();
        out << sizeRowCount << sizeColCount;
        for (int row = 0; row < sizeRowCount; ++row) {
            for (int col = 0; col < sizeColCount; ++col) {
                QVariant data = m_sizeModel->data(m_sizeModel->index(row, col));
                out << data;
            }
        }

        int nodeRowCount = m_nodeModel->rowCount();
        int nodeColCount = m_nodeModel->columnCount();
        out << nodeRowCount << nodeColCount;
        for (int row = 0; row < nodeRowCount; ++row) {
            for (int col = 0; col < nodeColCount; ++col) {
                QVariant data = m_nodeModel->data(m_nodeModel->index(row, col));
                out << data;
            }
        }

        file.close();
    }
}

void Preprocessor::loadModels(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        m_nodeModel->clearData();
        m_sizeModel->clearData();

        QDataStream in(&file);

        int sizeRowCount, sizeColCount;
        in >> sizeRowCount >> sizeColCount;
        m_sizeModel->setRowCount(sizeRowCount);
        m_sizeModel->setColumnCount(sizeColCount);
        for (int row = 0; row < sizeRowCount; ++row) {
            for (int col = 0; col < sizeColCount; ++col) {
                QVariant data;
                in >> data;
                m_sizeModel->setData(m_sizeModel->index(row, col), data);
            }
        }

        int nodeRowCount, nodeColCount;
        in >> nodeRowCount >> nodeColCount;
        m_nodeModel->setRowCount(nodeRowCount);
        m_nodeModel->setColumnCount(nodeColCount);
        for (int row = 0; row < nodeRowCount; ++row) {
            for (int col = 0; col < nodeColCount; ++col) {
                QVariant data;
                in >> data;
                m_nodeModel->setData(m_nodeModel->index(row, col), data);
            }
        }

        file.close();
    }
}

// по идее можно по другому(не знаю как)
void Preprocessor::clearData()
{
    m_nodeModel->clearData();
    m_sizeModel->clearData();

    m_scene->clear();
    delete m_scene;

    m_scene = new QGraphicsScene(this);
    connect(m_timer, &QTimer::timeout, m_scene, &QGraphicsScene::advance);
    m_view->resetView();
    m_view->setScene(m_scene);
    updateScene();
}

