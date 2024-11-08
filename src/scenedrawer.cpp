#include "scenedrawer.h"

SceneDrawer::SceneDrawer(QGraphicsScene* scene, QObject* parent)
    : QObject(parent), m_scene(scene) {}

void SceneDrawer::setScene(QGraphicsScene* scene) {
    m_scene = scene;
}


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
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;
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

//некрасиво
void SceneDrawer::drawKernelStripes(const SizeTableModel* sizeModel) {
    qreal currentX = 0;
    int rowCount = sizeModel->rowCount();

    qreal stripeYStart = 0;
    qreal stripeYEnd = stripeYStart + 800;
    QLineF stripeLine(0, stripeYStart, 0, stripeYEnd);
    m_scene->addLine(stripeLine, QPen(Qt::black));

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;

        if (width == 0 || height == 0) {
            continue;
        }

        qreal stripeX = currentX + width;


        QLineF stripeLine(stripeX, stripeYStart, stripeX, stripeYEnd);
        m_scene->addLine(stripeLine, QPen(Qt::black));

        currentX += width;
    }
}

//исправить рисование текста Nx
void SceneDrawer::drawNx(const SizeTableModel* sizeModel, const QVector<double>* vectorNx) {
    if (vectorNx->isEmpty()) {
        return;
    }

    qreal currentX = 0;
    int ratio = 0;
    int rowCount = sizeModel->rowCount();
    double maxEleventNx = *std::max_element(vectorNx->begin(), vectorNx->end(),
                                            [](double a, double b) {
                                                return std::abs(a) < std::abs(b);
                                            }) * RECT_HEIGHT_MULTIPLIER;



    while(std::abs(maxEleventNx) > 100){
        maxEleventNx /= 2;
        ++ratio;
    }
    const QString NxString = "Nx [qL]";
    QGraphicsTextItem *text = m_scene->addText(NxString);
    text->setPos(-60, 235);

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;

        if (width == 0 || height == 0) {
            continue;
        }


        qreal pointX1 = currentX;
        qreal pointX2 = currentX + width;
        double t1 = (*vectorNx)[2 * row] * RECT_HEIGHT_MULTIPLIER;
        double t2 = (*vectorNx)[2 * row + 1] * RECT_HEIGHT_MULTIPLIER;

        for (int i = 0; i <= ratio; ++i) {
            t1 /= 2;
            t2 /= 2;
        }

        qreal pointY1 = 250 - t1;
        qreal pointY2 = 250 - t2;



        QLineF line(pointX1, pointY1, pointX2, pointY2);
        m_scene->addLine(line, QPen(Qt::red));

        QGraphicsTextItem *Nx1n = m_scene->addText(QString::number((*vectorNx)[2 * row]));
        QGraphicsTextItem *Nx2n = m_scene->addText(QString::number((*vectorNx)[2 * row + 1]));

        if((*vectorNx)[2 * row] >= 0){
            Nx1n->setPos(pointX1 , pointY1 - 20);
        }else{
            Nx1n->setPos(pointX1 , pointY1);
        }

        if((*vectorNx)[2 * row + 1] >= 0){
            Nx2n->setPos(pointX2 - 30, pointY2 - 20);
        }else{
            Nx2n->setPos(pointX2 - 30, pointY2);
        }

        qreal hatchStep = 5.0;

        for (qreal x = pointX1; x <= pointX2; x += hatchStep) {
            qreal t = (x - pointX1) / (pointX2 - pointX1);
            qreal yLine = pointY1 * (1 - t) + pointY2 * t;
            qreal yOX = 250;

            QLineF hatchLine(x, yOX, x, yLine);
            m_scene->addLine(hatchLine, QPen(Qt::red));
        }

        QLineF lineOX(pointX1, 250, pointX2, 250);
        m_scene->addLine(lineOX, QPen(Qt::black));

        currentX += width;

    }
}

void SceneDrawer::drawUx(Processor* processor, const SizeTableModel* sizeModel, const QVector<double>* vectorUx, const QVector<double>* vectorNx) {
    if (vectorNx->isEmpty()) {
        return;
    }

    qreal currentX = 0;
    int ratio = 0;
    double maxEleventUx = 0;
    int rowCount = sizeModel->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;

        if (width == 0 || height == 0) {
            continue;
        }
        double step = 0.005;
        for (int i = 0; i * step < width / RECT_WIDTH_MULTIPLIER; ++i) {
            double uxValue = processor->calculationUxAtPoint(row, step * i);
            if(std::abs(maxEleventUx) < std::abs(uxValue)){
                maxEleventUx = uxValue;
            }
        }
    }

    maxEleventUx *= RECT_HEIGHT_MULTIPLIER;

    while(std::abs(maxEleventUx) > 100){
        maxEleventUx /= 2;
        ++ratio;
    }
    const QString NxString = "Ux [qL^2/EA]";
    QGraphicsTextItem *text = m_scene->addText(NxString);
    text->setPos(-80, 435);

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;

        if (width == 0 || height == 0) {
            continue;
        }

        qreal pointX1 = currentX;
        qreal pointX2 = currentX + width;
        double t1Ux = (*vectorUx)[2 * row] * RECT_HEIGHT_MULTIPLIER;
        double t2Ux = (*vectorUx)[2 * row + 1] * RECT_HEIGHT_MULTIPLIER;
        for (int i = 0; i <= ratio; ++i) {
            t1Ux /= 2;
            t2Ux /= 2;
        }

        qreal pointY1 = 450 - t1Ux;
        qreal pointY2 = 450 - t2Ux;

        if((*vectorNx)[2 * row] == (*vectorNx)[2 * row + 1]){

            QLineF line(pointX1, pointY1, pointX2, pointY2);
            m_scene->addLine(line, QPen(Qt::red));

            qreal hatchStep = 5.0;

            for (qreal x = pointX1; x <= pointX2; x += hatchStep) {
                qreal t = (x - pointX1) / (pointX2 - pointX1);
                qreal yLine = pointY1 * (1 - t) + pointY2 * t;
                qreal yOX = 450;

                QLineF hatchLine(x, yOX, x, yLine);
                m_scene->addLine(hatchLine, QPen(Qt::red));
            }
        }else{
            double hatchStep = 5.0;
            double step = 0.001;
            qreal yOX = 450;
            for (int i = 0; i * step < width / RECT_WIDTH_MULTIPLIER; ++i) {
                double uxValue = processor->calculationUxAtPoint(row, step * i) * RECT_HEIGHT_MULTIPLIER;
                for (int i = 0; i <= ratio; ++i) {
                    uxValue /= 2;
                }
                m_scene->addEllipse(currentX + i * step * RECT_WIDTH_MULTIPLIER, 450 - uxValue, 0.5, 0.3, QPen(Qt::red));

                // qDebug() << fmod(step * i * RECT_WIDTH_MULTIPLIER, hatchStep);
                if(std::abs(fmod(step * i * RECT_WIDTH_MULTIPLIER, hatchStep)) < 0.001){
                    QLineF hatchLine(currentX + i * step * RECT_WIDTH_MULTIPLIER, yOX, currentX + i * step * RECT_WIDTH_MULTIPLIER, 450 - uxValue);
                    m_scene->addLine(hatchLine, QPen(Qt::red));
                }
            }
        }

        QGraphicsTextItem *Ux1n = m_scene->addText(QString::number((*vectorUx)[2 * row]));
        QGraphicsTextItem *Ux2n = m_scene->addText(QString::number((*vectorUx)[2 * row + 1]));

        if((*vectorUx)[2 * row] >= 0){
            Ux1n->setPos(pointX1 , pointY1 - 20);
        }else{
            Ux1n->setPos(pointX1 , pointY1);
        }

        if((*vectorUx)[2 * row + 1] >= 0){
            Ux2n->setPos(pointX2 - 30, pointY2 - 20);
        }else{
            Ux2n->setPos(pointX2 - 30, pointY2);
        }

        QLineF lineOX(pointX1, 450, pointX2, 450);
        m_scene->addLine(lineOX, QPen(Qt::black));

        currentX += width;
    }
}

void SceneDrawer::drawSigmax(const SizeTableModel *sizeModel, const QVector<double> *vectorSigmax)
{
    if (vectorSigmax->isEmpty()) {
        return;
    }

    qreal currentX = 0;
    int ratio = 0;
    int rowCount = sizeModel->rowCount();
    double maxElementSigmax = *std::max_element(vectorSigmax->begin(), vectorSigmax->end(),
                                            [](double a, double b) {
                                                return std::abs(a) < std::abs(b);
                                            }) * RECT_HEIGHT_MULTIPLIER;



    while(std::abs(maxElementSigmax) > 100){
        maxElementSigmax /= 2;
        ++ratio;
    }
    const QString NxString = "σx [qL/A]";
    QGraphicsTextItem *text = m_scene->addText(NxString);
    text->setPos(-60, 635);

    for (int row = 0; row < rowCount; ++row) {
        int width = sizeModel->data(sizeModel->index(row, 0)).toInt() * RECT_WIDTH_MULTIPLIER;
        int height = sizeModel->data(sizeModel->index(row, 1)).toInt() * RECT_HEIGHT_MULTIPLIER;

        if (width == 0 || height == 0) {
            continue;
        }


        qreal pointX1 = currentX;
        qreal pointX2 = currentX + width;
        double t1 = (*vectorSigmax)[2 * row] * RECT_HEIGHT_MULTIPLIER;
        double t2 = (*vectorSigmax)[2 * row + 1] * RECT_HEIGHT_MULTIPLIER;

        for (int i = 0; i <= ratio; ++i) {
            t1 /= 2;
            t2 /= 2;
        }

        qreal pointY1 = 650 - t1;
        qreal pointY2 = 650 - t2;



        QLineF line(pointX1, pointY1, pointX2, pointY2);
        m_scene->addLine(line, QPen(Qt::red));

        QGraphicsTextItem *Sigmax1n = m_scene->addText(QString::number((*vectorSigmax)[2 * row]));
        QGraphicsTextItem *Sigmax2n = m_scene->addText(QString::number((*vectorSigmax)[2 * row + 1]));

        if((*vectorSigmax)[2 * row] >= 0){
            Sigmax1n->setPos(pointX1 , pointY1 - 20);
        }else{
            Sigmax1n->setPos(pointX1 , pointY1);
        }

        if((*vectorSigmax)[2 * row + 1] >= 0){
            Sigmax2n->setPos(pointX2 - 30, pointY2 - 20);
        }else{
            Sigmax2n->setPos(pointX2 - 30, pointY2);
        }

        qreal hatchStep = 5.0;

        for (qreal x = pointX1; x <= pointX2; x += hatchStep) {
            qreal t = (x - pointX1) / (pointX2 - pointX1);
            qreal yLine = pointY1 * (1 - t) + pointY2 * t;
            qreal yOX = 650;

            QLineF hatchLine(x, yOX, x, yLine);
            m_scene->addLine(hatchLine, QPen(Qt::red));
        }

        QLineF lineOX(pointX1, 650, pointX2, 650);
        m_scene->addLine(lineOX, QPen(Qt::black));

        currentX += width;

    }
}
