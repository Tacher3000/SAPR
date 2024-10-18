#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include "scenedrawer.h"
#include "scalablegraphicsview.h"

#include <QGraphicsScene>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class PostProcessor : public QWidget
{
    Q_OBJECT
public:
    PostProcessor(QWidget *parent = nullptr);

    void draw(const SizeTableModel *sizeModel, const NodeModel *nodeModel);
private slots:
    void toProcessor();
signals:
    void clickedToProcessor();
private:
    SceneDrawer *m_sceneDrawer;
    QGraphicsScene* m_scene;
    ScalableGraphicsView* m_view;

    QPushButton *m_toProcessorButton;
};

#endif // POSTPROCESSOR_H
