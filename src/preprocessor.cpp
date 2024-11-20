#include "preprocessor.h"

Preprocessor::Preprocessor(QWidget* parent) : QWidget(parent) {
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    QHBoxLayout* mainLayput = new QHBoxLayout(this);
    QVBoxLayout* tablesLayput = new QVBoxLayout(this);
    QWidget* tablesWidget = new QWidget(this);

    DoubleDelegate *delegateDouble = new DoubleDelegate(this);
    PositiveDoubleDelegate *delegatePositiveDouble = new PositiveDoubleDelegate(this);

    m_sizeModel = new SizeTableModel;
    connect(m_sizeModel, &SizeTableModel::dataChanged, this, &Preprocessor::updateNodeModel);
    m_sizeTableView = new QTableView(this);
    m_sizeTableView->setModel(m_sizeModel);
    m_sizeTableView->resizeRowsToContents();
    m_sizeTableView->resizeColumnsToContents();
    m_sizeTableView->setItemDelegateForColumn(0, delegatePositiveDouble);
    m_sizeTableView->setItemDelegateForColumn(1, delegatePositiveDouble);
    m_sizeTableView->setItemDelegateForColumn(2, delegateDouble);
    m_sizeTableView->setItemDelegateForColumn(3, delegatePositiveDouble);
    m_sizeTableView->setItemDelegateForColumn(4, delegatePositiveDouble);
    tablesLayput->addWidget(m_sizeTableView);

    m_nodeModel = new NodeModel;
    connect(m_nodeModel, &NodeModel::dataChanged, this, &Preprocessor::updateScene);
    m_nodeTableView = new QTableView(this);
    m_nodeTableView->setModel(m_nodeModel);
    m_nodeTableView->resizeRowsToContents();
    m_nodeTableView->resizeColumnsToContents();
    m_nodeTableView->setItemDelegateForColumn(0, delegateDouble);
    // m_nodeTableView->setItemDelegateForColumn(1, delegateNumeric);

    tablesLayput->addWidget(m_nodeTableView);

    m_toProcessorButton = new QPushButton("Рассчитать", this);
    m_toProcessorButton->setMinimumHeight(40);
    connect(m_toProcessorButton, &QPushButton::clicked, this, &Preprocessor::toProcessor);
    tablesLayput->addWidget(m_toProcessorButton);

    tablesWidget->setLayout(tablesLayput);
    splitter->addWidget(tablesWidget);

    m_scene = new QGraphicsScene(this);
    m_sceneDrawer = new SceneDrawer(m_scene, this);
    m_scene->setSceneRect(0, 0, 1000, 700);

    m_view = new ScalableGraphicsView(this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setFrameStyle(0);
    m_view->setScene(m_scene);
    splitter->addWidget(m_view);

    tablesWidget->setMinimumWidth(100);
    m_view->setMinimumWidth(400);

    QList<int> sizes;
    sizes << 850 << 1250;
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
    m_sceneDrawer->clearScene();
    m_sceneDrawer->setSceneSize(0, 0);


    if (m_nodeModel->isEmpty() && m_sizeModel->isEmpty()) {
        m_sceneDrawer->setSceneSize(1000, 700);
        flyText();
        return;
    }

    App* app = App::theApp();
    QSettings* settings = app->settings();
    qreal maxHeight = RECT_HEIGHT_MULTIPLIER * m_sizeModel->getMaxSection();

    // придумать
    // for(int row = 0; row < m_sizeModel->rowCount() - 1; ++row){
    //     int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt();
    //     int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt();
    //     if(width == 0 || height == 0) {
    //         return;
    //     }
    // }

    if (settings->value("checkBoxWidget", false).toBool()) {
        m_sceneDrawer->drawSignatureSectionAndModulusValue(m_sizeModel);
        if (settings->value("checkBoxSignatureDistributedLoad", false).toBool()) m_sceneDrawer->drawSignatureDistributedLoad(m_sizeModel);
        if (settings->value("checkBoxSignatureFocusedlLoad", false).toBool()) m_sceneDrawer->drawSignatureFocusedlLoad(m_sizeModel, m_nodeModel);
        if (settings->value("checkBoxLengthKernelt", false).toBool()) m_sceneDrawer->drawLengthKernel(m_sizeModel);
        if (settings->value("checkBoxKernelN", false).toBool()) m_sceneDrawer->drawKernelN(m_sizeModel, maxHeight);
        if (settings->value("checkBoxKernel", false).toBool()) m_sceneDrawer->drawKernelWidget(m_sizeModel);
        if (settings->value("checkBoxDistributedLoad", false).toBool()) m_sceneDrawer->drawDistributedLoadWidget(m_sizeModel);
        if (settings->value("checkBoxFocusedLoad", false).toBool()) m_sceneDrawer->drawFocusedLoad(m_sizeModel, m_nodeModel);
        if (settings->value("checkBoxSupport", false).toBool()) m_sceneDrawer->drawSupport(m_sizeModel, m_nodeModel);
        if (settings->value("checkBoxNodeN", false).toBool()) m_sceneDrawer->drawNode(m_sizeModel, maxHeight);
    } else {
        m_sceneDrawer->drawSignatureSectionAndModulusValue(m_sizeModel);
        if (settings->value("checkBoxSignatureDistributedLoad", false).toBool()) m_sceneDrawer->drawSignatureDistributedLoad(m_sizeModel);
        if (settings->value("checkBoxSignatureFocusedlLoad", false).toBool()) m_sceneDrawer->drawSignatureFocusedlLoad(m_sizeModel, m_nodeModel);
        if (settings->value("checkBoxLengthKernelt", false).toBool()) m_sceneDrawer->drawLengthKernel(m_sizeModel);
        if (settings->value("checkBoxKernelN", false).toBool()) m_sceneDrawer->drawKernelN(m_sizeModel, maxHeight);
        if (settings->value("checkBoxDistributedLoad", false).toBool()) m_sceneDrawer->drawDistributedLoad(m_sizeModel);
        if (settings->value("checkBoxFocusedLoad", false).toBool()) m_sceneDrawer->drawFocusedLoad(m_sizeModel, m_nodeModel);
        if (settings->value("checkBoxKernel", false).toBool()) m_sceneDrawer->drawKernel(m_sizeModel);
        if (settings->value("checkBoxSupport", false).toBool()) m_sceneDrawer->drawSupport(m_sizeModel, m_nodeModel);
        if (settings->value("checkBoxNodeN", false).toBool()) m_sceneDrawer->drawNode(m_sizeModel, maxHeight);

    }
    m_sceneDrawer->drawPricol(m_sizeModel);

    m_sizeTableView->resizeColumnsToContents();
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

        QVariant modulusValue = m_sizeModel->getModulusValue();
        out << modulusValue;

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

        QVariant modulusValue;
        in >> modulusValue;
        m_sizeModel->setModulusValue(modulusValue);

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

    m_sceneDrawer->clearScene();
    delete m_scene;

    m_scene = new QGraphicsScene(this);
    connect(m_timer, &QTimer::timeout, m_scene, &QGraphicsScene::advance);
    m_sceneDrawer->setScene(m_scene);
    m_view->resetView();
    m_view->setScene(m_scene);
    updateScene();
}

void Preprocessor::toProcessor()
{
    if(m_nodeModel->data(m_nodeModel->index(0, 1)).toBool() || m_nodeModel->data(m_nodeModel->index(m_nodeModel->rowCount() - 1, 1)).toBool()){
        if(m_sizeModel->getModulusValue().toBool()){
            for(int row = 0; row < m_sizeModel->rowCount() - 1; ++row){
                double width = m_sizeModel->data(m_sizeModel->index(row, 0)).toString().replace(',', '.').toDouble();
                double height = m_sizeModel->data(m_sizeModel->index(row, 1)).toString().replace(',', '.').toDouble();
                if(width == 0 || height == 0) {
                    return;
                }
            }
            emit clickedToProcessor();
        }
    }
}

NodeModel* Preprocessor::getNodeModel() const {
    return m_nodeModel;
}

SizeTableModel* Preprocessor::getSizeModel() const {
    return m_sizeModel;
}

QTableView *Preprocessor::getSizeTableView() const
{
    return m_sizeTableView;
}

QTableView *Preprocessor::getNodeTableView() const
{
    return m_nodeTableView;
}

QGraphicsScene *Preprocessor::getScene() const
{
    return m_scene;
}


void Preprocessor::exportModelToPdf(const QString &filePath) {
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
            QRect rect(x, y + row * cellHeight, cellWidth, cellHeight);
            painter.drawRect(rect);
            painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, QString::number(row + 1));

            for (int col = 0; col < columnCount; ++col) {
                QRect rect(x + (col + 1) * cellWidth, y + row * cellHeight, cellWidth, cellHeight);
                painter.drawRect(rect);
                painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, model->data(model->index(row, col)).toString());
            }
        }

        y += rowCount * cellHeight + 50;
    };

    int y = 100;

    printModel(m_sizeModel, "Таблица стержней", y, true);

    printModel(m_nodeModel, "Таблица узлов", y);

    painter.end();
}
