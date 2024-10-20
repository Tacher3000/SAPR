#include "preprocessor.h"

Preprocessor::Preprocessor(QWidget* parent) : QWidget(parent) {
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    QHBoxLayout* mainLayput = new QHBoxLayout(this);
    QVBoxLayout* tablesLayput = new QVBoxLayout(this);
    QWidget* tablesWidget = new QWidget(this);

    NumericDelegate *delegate = new NumericDelegate(this);

    m_sizeModel = new SizeTableModel;
    connect(m_sizeModel, &SizeTableModel::dataChanged, this, &Preprocessor::updateNodeModel);
    m_sizeTableView = new QTableView(this);
    m_sizeTableView->setModel(m_sizeModel);
    m_sizeTableView->resizeRowsToContents();
    m_sizeTableView->resizeColumnsToContents();
    m_sizeTableView->setItemDelegate(delegate);
    tablesLayput->addWidget(m_sizeTableView);

    m_nodeModel = new NodeModel;
    connect(m_nodeModel, &NodeModel::dataChanged, this, &Preprocessor::updateScene);
    m_nodeTableView = new QTableView(this);
    m_nodeTableView->setModel(m_nodeModel);
    m_nodeTableView->resizeRowsToContents();
    m_nodeTableView->resizeColumnsToContents();
    m_nodeTableView->setItemDelegate(delegate);
    tablesLayput->addWidget(m_nodeTableView);

    m_toProcessorButton = new QPushButton("Расчитать", this);
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
    sizes << 700 << 1400;
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

    if (settings->value("checkBoxWidget", false).toBool()) {
        if (settings->value("checkBoxFocusedLoad", false).toBool()) m_sceneDrawer->drawFocusedLoad(m_sizeModel, m_nodeModel);
        if (settings->value("checkBoxKernelN", false).toBool()) m_sceneDrawer->drawKernelN(m_sizeModel, maxHeight);
        if (settings->value("checkBoxKernel", false).toBool()) m_sceneDrawer->drawKernelWidget(m_sizeModel);
        if (settings->value("checkBoxDistributedLoad", false).toBool()) m_sceneDrawer->drawDistributedLoadWidget(m_sizeModel);
        if (settings->value("checkBoxSupport", false).toBool()) m_sceneDrawer->drawSupport(m_sizeModel, m_nodeModel);
        if (settings->value("checkBoxNodeN", false).toBool()) m_sceneDrawer->drawNode(m_sizeModel, maxHeight);
    } else {
        if (settings->value("checkBoxFocusedLoad", false).toBool()) m_sceneDrawer->drawFocusedLoad(m_sizeModel, m_nodeModel);
        if (settings->value("checkBoxKernelN", false).toBool()) m_sceneDrawer->drawKernelN(m_sizeModel, maxHeight);
        if (settings->value("checkBoxDistributedLoad", false).toBool()) m_sceneDrawer->drawDistributedLoad(m_sizeModel);
        if (settings->value("checkBoxKernel", false).toBool()) m_sceneDrawer->drawKernel(m_sizeModel);
        if (settings->value("checkBoxSupport", false).toBool()) m_sceneDrawer->drawSupport(m_sizeModel, m_nodeModel);
        if (settings->value("checkBoxNodeN", false).toBool()) m_sceneDrawer->drawNode(m_sizeModel, maxHeight);
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
    m_view->resetView();
    m_view->setScene(m_scene);
    updateScene();
}

void Preprocessor::toProcessor()
{
    emit clickedToProcessor();
}

NodeModel* Preprocessor::getNodeModel() const {
    return m_nodeModel;
}

SizeTableModel* Preprocessor::getSizeModel() const {
    return m_sizeModel;
}
