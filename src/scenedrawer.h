#ifndef SCENEDRAWER_H
#define SCENEDRAWER_H

#include "sizetablemodel.h"
#include "nodemodel.h"
#include "settingsdialog.h"
#include "app.h"
#include "processor.h"

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

#include <algorithm>

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

    void drawKernelStripes(const SizeTableModel *sizeModel);
    void drawNx(const SizeTableModel *sizeModel, const QVector<double> *vectorNx);
    void drawUx(Processor* processor, const SizeTableModel *sizeModel, const QVector<double> *vectorUx, const QVector<double>* vectorNx);
private:
    QGraphicsScene* m_scene;
};

#endif // SCENEDRAWER_H
