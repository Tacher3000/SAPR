// #include "SceneManager.h"

// SceneManager::SceneManager(QGraphicsView* view, SizeTableModel* sizeModel, NodeModel* nodeModel, QObject* parent)
//     : QObject(parent), m_view(view), m_sizeModel(sizeModel), m_nodeModel(nodeModel)
// {
//     m_scene = new QGraphicsScene(this);
//     m_view->setScene(m_scene);
// }

// SceneManager::~SceneManager()
// {
//     delete m_scene;
// }

// void SceneManager::clearScene()
// {
//     m_scene->clear();
// }

// qreal SceneManager::getMaxHeight() const
// {
//     return RECT_HEIGHT_MULTIPLIER * m_sizeModel->getMaxSection();
// }

// void SceneManager::updateScene()
// {
//     clearScene();

//     App* app = App::theApp();
//     QSettings* settings = app->settings();

//     qreal maxHeight = getMaxHeight();

//     std::vector<DrawFunction> drawFunctions = {
//                                                {"checkBoxFocusedLoad", std::bind(&SceneManager::drawFocusedLoad, this)},
//                                                {"checkBoxKernelN", std::bind(&SceneManager::drawKernelN, this, maxHeight)},
//                                                {"checkBoxDistributedLoad", std::bind(&SceneManager::drawDistributedLoad, this)},
//                                                {"checkBoxKernel", std::bind(&SceneManager::drawKernel, this)},
//                                                {"checkBoxSupport", std::bind(&SceneManager::drawSupport, this)},
//                                                {"checkBoxNodeN", std::bind(&SceneManager::drawNode, this, maxHeight)},
//                                                };

//     for (const auto& drawFunction : drawFunctions) {
//         if (settings->value(drawFunction.setting, false).toBool()) {
//             drawFunction.function();
//         }
//     }
// }

// void SceneManager::drawKernel()
// {
//     App* app = App::theApp();
//     QSettings* settings = app->settings();
//     qreal currentX = 0;
//     qreal maxHeight = 50 * m_sizeModel->getMaxSection();
//     int rowCount = m_sizeModel->rowCount();

//     for (int row = 0; row < rowCount; ++row) {
//         int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
//         int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;

//         if(width == 0 || height == 0){
//             continue;
//         }

//         QString kernelString = settings->value("kernelColor", QColor(Qt::black).name()).toString();
//         QColor kernelColor(kernelString);

//         QRectF rect(currentX, -height / 2, width, height);
//         m_scene->addRect(rect, QPen(kernelColor));

//         currentX += width;
//     }
// }

// // можно сделать красивее
// void SceneManager::drawNode(qreal maxHeight)
// {
//     App* app = App::theApp();
//     QSettings* settings = app->settings();

//     QString nodeNString = settings->value("nodeColorN", QColor(Qt::black).name()).toString();
//     QColor nodeColorN(nodeNString);

//     qreal currentX = 0;
//     for (int row = 0; row < m_sizeModel->rowCount(); ++row) {
//         int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
//         int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
//         if(width == 0 || height == 0){
//             continue;
//         }

//         qreal numberX = currentX;
//         qreal numberY = maxHeight / 2 + 10;

//         QRectF square(numberX - 10, numberY, 20, 20);
//         m_scene->addRect(square, QPen(Qt::black), QBrush(nodeColorN));

//         QString number = QString::number(row + 1);
//         QGraphicsTextItem* textItem = m_scene->addText(number);
//         QRectF textRect = textItem->boundingRect();
//         textItem->setPos(numberX - textRect.width() / 2, numberY - textRect.height() / 2 + 10);

//         currentX += width;

//         {
//             qreal numberX = currentX;

//             QRectF square(numberX - 10, numberY, 20, 20);
//             m_scene->addRect(square, QPen(Qt::black), QBrush(nodeColorN));

//             QString number = QString::number(row + 2);
//             QGraphicsTextItem* textItem = m_scene->addText(number);
//             QRectF textRect = textItem->boundingRect();
//             textItem->setPos(numberX - textRect.width() / 2, numberY - textRect.height() / 2 + 10);
//         }
//     }
// }

// void SceneManager::drawKernelN(qreal maxHeight)
// {
//     App* app = App::theApp();
//     QSettings* settings = app->settings();

//     QString kernelNString = settings->value("kernelColorN", QColor(Qt::black).name()).toString();
//     QColor kernelColorN(kernelNString);

//     qreal currentX = 0;
//     for (int row = 0; row < m_sizeModel->rowCount() - 1; ++row) {
//         int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
//         int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
//         if(width == 0 || height == 0){
//             continue;
//         }

//         qreal centerX = currentX + width / 2;
//         qreal centerY = -maxHeight / 2 - 30;

//         QRectF circle(centerX - 15, centerY - 15, 30, 30);
//         m_scene->addEllipse(circle, QPen(Qt::black), QBrush(kernelColorN));

//         QString number = QString::number(row + 1);
//         QGraphicsTextItem *text = m_scene->addText(number);
//         QRectF textRect = text->boundingRect();
//         text->setPos(centerX - textRect.width() / 2, centerY - textRect.height() / 2);

//         currentX += width;
//     }
// }

// void SceneManager::drawDistributedLoad()
// {
//     App* app = App::theApp();
//     QSettings* settings = app->settings();

//     qreal currentX = 0;
//     int rowCount = m_sizeModel->rowCount();

//     for (int row = 0; row < rowCount; ++row) {
//         int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
//         int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
//         int direction = m_sizeModel->data(m_sizeModel->index(row, 2)).toInt();

//         if(width == 0 || height == 0){
//             continue;
//         }

//         if (direction != 0) {
//             QString distributedLoadString = settings->value("distributedLoadColor", QColor(Qt::black).name()).toString();
//             QColor distributedLoadColor(distributedLoadString);

//             int arrowCount = width / 10;
//             qreal arrowSpacing = width / arrowCount;
//             qreal arrowY = 0;

//             QLineF line(currentX, arrowY, currentX + width, arrowY);
//             m_scene->addLine(line, QPen(QColor(distributedLoadColor)));

//             for (int i = 0; i < arrowCount; ++i) {
//                 qreal arrowX = currentX + i * arrowSpacing;


//                 if (direction > 0) {
//                     QLineF arrowHead1(arrowX + arrowSpacing, arrowY, arrowX + arrowSpacing - 5, arrowY - 5);
//                     QLineF arrowHead2(arrowX + arrowSpacing, arrowY, arrowX + arrowSpacing - 5, arrowY + 5);
//                     m_scene->addLine(arrowHead1, QPen(QColor(distributedLoadColor)));
//                     m_scene->addLine(arrowHead2, QPen(QColor(distributedLoadColor)));
//                 } else {
//                     QLineF arrowHead1(arrowX, arrowY, arrowX + 5, arrowY - 5);
//                     QLineF arrowHead2(arrowX, arrowY, arrowX + 5, arrowY + 5);
//                     m_scene->addLine(arrowHead1, QPen(QColor(distributedLoadColor)));
//                     m_scene->addLine(arrowHead2, QPen(QColor(distributedLoadColor)));
//                 }
//             }
//         }

//         currentX += width;
//     }
// }

// //можно сделать красивее код(говнокод)
// void SceneManager::drawSupport()
// {
//     App* app = App::theApp();
//     QSettings* settings = app->settings();

//     qreal currentX = 0;
//     int rowCount = m_nodeModel->rowCount();
//     int lastHeight = 0;

//     for (int row = 0; row < rowCount; ++row) {
//         int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
//         int height = m_sizeModel->data(m_sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
//         QString support = m_nodeModel->data(m_nodeModel->index(row, 1)).toString();

//         if(height != 0){
//             lastHeight = height;
//         }

//         QString supportString = settings->value("supportColor", QColor(Qt::black).name()).toString();
//         QColor supportColor(supportString);

//         // Левая вертикальная опора
//         if(row == 0 && !support.isEmpty()){
//             qreal leftX = currentX;
//             qreal topY = -height / 2 - 10;
//             qreal bottomY = height / 2 + 10;

//             m_scene->addLine(QLineF(leftX, topY, leftX, bottomY), QPen(supportColor));

//             int arrowCount = (height + 20) / 10;
//             qreal arrowSpacing = (height + 20) / arrowCount;

//             for (int i = 0; i <= arrowCount; ++i) {
//                 qreal arrowY = topY + i * arrowSpacing;

//                 QLineF arrowHead1(0, arrowY,  -5, arrowY + 5);
//                 m_scene->addLine(arrowHead1, QPen(supportColor));
//             }
//         }
//         // Правая вертикальная опора
//         if((row == (rowCount - 1)) && !support.isEmpty()){
//             qreal rightX = currentX + width;
//             qreal topY = -lastHeight / 2 - 10;
//             qreal bottomY = lastHeight / 2 + 10;
//             m_scene->addLine(QLineF(rightX, topY, rightX, bottomY), QPen(supportColor));

//             int arrowCount = (lastHeight + 20) / 10;
//             qreal arrowSpacing = (lastHeight + 20) / arrowCount;

//             for (int i = 0; i <= arrowCount; ++i) {
//                 qreal arrowY = topY + i * arrowSpacing;

//                 QLineF arrowHead1(rightX, arrowY,  rightX + 5, arrowY - 5);
//                 m_scene->addLine(arrowHead1, QPen(supportColor));
//             }
//         }

//         currentX += width;
//     }
// }

// void SceneManager::drawFocusedLoad()
// {
//     App* app = App::theApp();
//     QSettings* settings = app->settings();

//     QString FocusedColorString = settings->value("focusedLoadColor", QColor(Qt::black).name()).toString();
//     QColor focusedColor(FocusedColorString);

//     qreal currentX = 0;

//     int rowCount = m_sizeModel->rowCount();

//     qreal arrowY = 0;
//     qreal arrowX = 0;

//     for (int row = 0; row < rowCount; ++row) {
//         int width = m_sizeModel->data(m_sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
//         int focusedDirection = m_nodeModel->data(m_nodeModel->index(row, 0)).toInt();
//         int direction = m_sizeModel->data(m_sizeModel->index(row, 2)).toInt();

//         if (focusedDirection != 0) {
//             // if(direction != 0){
//             //     m_scene->addLine(currentX, arrowY, currentX + (focusedDirection > 0 ? 50 : -50), arrowY, QPen(focusedColor));

//             //     m_scene->addLine(currentX + (focusedDirection > 0 ? 50 : -50), arrowY, currentX + (focusedDirection > 0 ? 20 : -20), arrowY - 15, QPen(focusedColor));
//             //     m_scene->addLine(currentX + (focusedDirection > 0 ? 50 : -50), arrowY, currentX + (focusedDirection > 0 ? 20 : -20), arrowY + 15, QPen(focusedColor));
//             // }else{
//             m_scene->addLine(currentX, arrowY, currentX + (focusedDirection > 0 ? 50 : -50), arrowY, QPen(focusedColor));

//             m_scene->addLine(currentX + (focusedDirection > 0 ? 50 : -50), arrowY, currentX + (focusedDirection > 0 ? 20 : -20), arrowY - 15, QPen(focusedColor));
//             m_scene->addLine(currentX + (focusedDirection > 0 ? 50 : -50), arrowY, currentX + (focusedDirection > 0 ? 20 : -20), arrowY + 15, QPen(focusedColor));
//             // }


//         }


//         currentX += width;
//     }
// }
