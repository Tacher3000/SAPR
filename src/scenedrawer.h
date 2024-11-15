#ifndef SCENEDRAWER_H
#define SCENEDRAWER_H

#include "sizetablemodel.h"
#include "nodemodel.h"
#include "settingsdialog.h"
#include "app.h"
// #include "processor.h"

#include <QGraphicsScene>
#include <QSettings>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QProgressBar>
#include <QTextEdit>
#include <QPen>
#include <QBrush>
#include <QTimer>
#include <QPropertyAnimation>
#include <QRandomGenerator>
#include <QSet>

#include <algorithm>

class Processor;

class RotatingPixmapItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    RotatingPixmapItem(const QPixmap& pixmap)
        : QGraphicsPixmapItem(pixmap), m_rotation(0) {}

    qreal rotation() const { return m_rotation; }
    void setRotation(qreal angle) {
        if (m_rotation != angle) {
            m_rotation = angle;
            QGraphicsPixmapItem::setRotation(angle);
            emit rotationChanged();
        }
    }

signals:
    void rotationChanged();

private:
    qreal m_rotation;
};

const int RECT_WIDTH_MULTIPLIER = 100;
const int RECT_HEIGHT_MULTIPLIER = 50;

class SceneDrawer : public QObject {
    Q_OBJECT

public:
    SceneDrawer(QGraphicsScene* scene, QObject* parent = nullptr);

    void setScene(QGraphicsScene* scene);

    void drawKernel(const SizeTableModel* sizeModel);
    void drawKernelWidget(const SizeTableModel* sizeModel);
    void drawNode(const SizeTableModel* sizeModel, qreal maxHeight);
    void drawKernelN(const SizeTableModel* sizeModel, qreal maxHeight);
    void drawDistributedLoad(const SizeTableModel* sizeModel);
    void drawDistributedLoadWidget(const SizeTableModel* sizeModel);
    void drawSupport(const SizeTableModel* sizeModel, const NodeModel* nodeModel);
    void drawFocusedLoad(const SizeTableModel* sizeModel, const NodeModel* nodeModel);
    void clearScene();
    void setSceneSize(qreal width, qreal height);

    void drawLengthKernel(const SizeTableModel* sizeModel);
    void drawSignatureSectionAndModulusValue(const SizeTableModel* sizeModel);
    void drawSignatureDistributedLoad(const SizeTableModel* sizeModel);
    void drawSignatureFocusedlLoad(const SizeTableModel* sizeModel, const NodeModel* nodeModel);

    void drawPricol(const SizeTableModel* sizeModel);


    void drawKernelStripes(const SizeTableModel *sizeModel);
    void drawNx(const SizeTableModel *sizeModel, const QVector<double> *vectorNx);
    void drawUx(Processor* processor, const SizeTableModel *sizeModel, const QVector<double> *vectorUx, const QVector<double>* vectorNx);
    void drawSigmax(const SizeTableModel *sizeModel, const QVector<double> *vectorSigmax);
private:
    QGraphicsScene* m_scene;
};

#endif // SCENEDRAWER_H
