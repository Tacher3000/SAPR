// #ifndef SCENEMANAGER_H
// #define SCENEMANAGER_H

// #include <QGraphicsScene>
// #include <QGraphicsView>
// #include <QSettings>
// #include <QColor>
// #include <QPen>
// #include <QBrush>
// #include <QGraphicsTextItem>
// #include <functional>

// #include "sizetablemodel.h"
// #include "nodemodel.h"
// #include "app.h"

// const int RECT_WIDTH_MULTIPLIER = 100;
// const int RECT_HEIGHT_MULTIPLIER = 50;

// class SceneManager : public QObject
// {
//     Q_OBJECT

// public:
//     SceneManager(QGraphicsView* view, SizeTableModel* sizeModel, NodeModel* nodeModel, QObject* parent = nullptr);
//     ~SceneManager();

//     void updateScene();
//     void clearScene();

// private:
//     QGraphicsScene* m_scene;
//     QGraphicsView* m_view;
//     SizeTableModel* m_sizeModel;
//     NodeModel* m_nodeModel;

//     struct DrawFunction {
//         QString setting;
//         std::function<void()> function;
//     };

//     void drawKernel();
//     void drawNode(qreal maxHeight);
//     void drawKernelN(qreal maxHeight);
//     void drawDistributedLoad();
//     void drawSupport();
//     void drawFocusedLoad();

//     qreal getMaxHeight() const;
//     void addDrawFunction(const QString& setting, std::function<void()> function);
// };

// #endif // SCENEMANAGER_H
