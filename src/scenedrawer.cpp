#include "scenedrawer.h"

SceneDrawer::SceneDrawer(QGraphicsScene* scene, QObject* parent)
    : QObject(parent), m_scene(scene) {}

void SceneDrawer::clearScene() {
    m_scene->clear();
}

void SceneDrawer::setSceneSize(qreal width, qreal height) {
    m_scene->setSceneRect(0, 0, width, height);
}

void SceneDrawer::drawKernel(const SizeTableModel* sizeModel) {
    App* app = App::theApp();
    QSettings* settings = app->settings();
    qreal currentX = 0;
    int rowCount = sizeModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;

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

void SceneDrawer::drawKernelWidget(const SizeTableModel* sizeModel) {
    qreal currentX = 0;
    int rowCount = sizeModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;

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

//исправить нодМОдел
// можно сделать красивее
void SceneDrawer::drawNode(const SizeTableModel* sizeModel, qreal maxHeight) {
    App* app = App::theApp();
    QSettings* settings = app->settings();

    QString nodeNString = settings->value("nodeColorN", QColor(Qt::black).name()).toString();
    QColor nodeColorN(nodeNString);

    qreal currentX = 0;
    for (int row = 0; row < sizeModel->rowCount(); ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
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

void SceneDrawer::drawKernelN(const SizeTableModel* sizeModel, qreal maxHeight) {
    App* app = App::theApp();
    QSettings* settings = app->settings();

    QString kernelNString = settings->value("kernelColorN", QColor(Qt::black).name()).toString();
    QColor kernelColorN(kernelNString);

    qreal currentX = 0;
    for (int row = 0; row < sizeModel->rowCount() - 1; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
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

void SceneDrawer::drawDistributedLoad(const SizeTableModel* sizeModel) {
    App* app = App::theApp();
    QSettings* settings = app->settings();

    qreal currentX = 0;
    int rowCount = sizeModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
        int direction = sizeModel->data(sizeModel->index(row, 2)).toInt();

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
void SceneDrawer::drawDistributedLoadWidget(const SizeTableModel* sizeModel) {
    qreal currentX = 0;
    int rowCount = sizeModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
        int direction = sizeModel->data(sizeModel->index(row, 2)).toInt();

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
void SceneDrawer::drawSupport(const SizeTableModel* sizeModel, const NodeModel* nodeModel) {
    App* app = App::theApp();
    QSettings* settings = app->settings();

    qreal currentX = 0;
    int rowCount = nodeModel->rowCount();
    int lastHeight = 0;

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
        QString support = nodeModel->data(nodeModel->index(row, 1)).toString();

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

void SceneDrawer::drawFocusedLoad(const SizeTableModel* sizeModel, const NodeModel* nodeModel) {
    App* app = App::theApp();
    QSettings* settings = app->settings();

    QString FocusedColorString = settings->value("focusedLoadColor", QColor(Qt::black).name()).toString();
    QColor focusedColor(FocusedColorString);

    qreal currentX = 0;

    int rowCount = sizeModel->rowCount();

    qreal arrowY = 0;
    qreal arrowX = 0;

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int focusedDirection = nodeModel->data(nodeModel->index(row, 0)).toInt();
        int direction = sizeModel->data(sizeModel->index(row, 2)).toInt();

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
