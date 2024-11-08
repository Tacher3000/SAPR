#include "postprocessor.h"

PostProcessor::PostProcessor(QWidget *parent) : QWidget(parent)
{
    m_scene = new QGraphicsScene(this);
    m_sceneDrawer = new SceneDrawer(m_scene, this);

    m_view = new ScalableGraphicsView(this);
    m_view->setRenderHint(QPainter::Antialiasing);
    m_view->setFrameStyle(0);
    m_view->setScene(m_scene);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_view);

    m_toProcessorButton = new QPushButton("Назад", this);
    m_toProcessorButton->setMinimumHeight(40);
    connect(m_toProcessorButton, &QPushButton::clicked, this, &PostProcessor::toProcessor);
    layout->addWidget(m_toProcessorButton);


    setLayout(layout);
}



void PostProcessor::toProcessor()
{
    emit clickedToProcessor();
}

void PostProcessor::draw(Processor *processor, const SizeTableModel *sizeModel,
                         const NodeModel *nodeModel) {
    m_sceneDrawer->clearScene();
    m_sceneDrawer->setSceneSize(0, 0);

    App *app = App::theApp();
    QSettings *settings = app->settings();
    qreal maxHeight = RECT_HEIGHT_MULTIPLIER * sizeModel->getMaxSection();

    m_sceneDrawer->drawNx(sizeModel, &processor->getVectorNx());
    m_sceneDrawer->drawUx(processor, sizeModel, &processor->getVectorUx(), &processor->getVectorNx());
    m_sceneDrawer->drawSigmax(sizeModel, &processor->getVectorSigmax());
    m_sceneDrawer->drawKernelStripes(sizeModel);

    if (settings->value("checkBoxWidget", false).toBool()) {
        if (settings->value("checkBoxFocusedLoad", false).toBool())
            m_sceneDrawer->drawFocusedLoad(sizeModel, nodeModel);
        if (settings->value("checkBoxKernelN", false).toBool())
            m_sceneDrawer->drawKernelN(sizeModel, maxHeight);
        if (settings->value("checkBoxKernel", false).toBool())
            m_sceneDrawer->drawKernelWidget(sizeModel);
        if (settings->value("checkBoxDistributedLoad", false).toBool())
            m_sceneDrawer->drawDistributedLoadWidget(sizeModel);
        if (settings->value("checkBoxSupport", false).toBool())
            m_sceneDrawer->drawSupport(sizeModel, nodeModel);
        if (settings->value("checkBoxNodeN", false).toBool())
            m_sceneDrawer->drawNode(sizeModel, maxHeight);
    } else {
        if (settings->value("checkBoxFocusedLoad", false).toBool())
            m_sceneDrawer->drawFocusedLoad(sizeModel, nodeModel);
        if (settings->value("checkBoxKernelN", false).toBool())
            m_sceneDrawer->drawKernelN(sizeModel, maxHeight);
        if (settings->value("checkBoxDistributedLoad", false).toBool())
            m_sceneDrawer->drawDistributedLoad(sizeModel);
        if (settings->value("checkBoxKernel", false).toBool())
            m_sceneDrawer->drawKernel(sizeModel);
        if (settings->value("checkBoxSupport", false).toBool())
            m_sceneDrawer->drawSupport(sizeModel, nodeModel);
        if (settings->value("checkBoxNodeN", false).toBool())
            m_sceneDrawer->drawNode(sizeModel, maxHeight);
    }
}
