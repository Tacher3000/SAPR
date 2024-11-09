#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include "scenedrawer.h"
#include "scalablegraphicsview.h"
#include "processor.h"

#include <QGraphicsScene>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPrinter>
#include <QPainter>
#include <QPdfWriter>
#include <QPdfDocument>
#include <QFile>
#include <QBuffer>
#include <QTemporaryFile>
#include <QPdfDocumentRenderOptions>


class PostProcessor : public QWidget
{
    Q_OBJECT
public:
    PostProcessor(QWidget *parent = nullptr);

    void draw(Processor *processor, const SizeTableModel *sizeModel,
              const NodeModel *nodeModel);
    void exportSceneToPdf(const QString &filePath);
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
