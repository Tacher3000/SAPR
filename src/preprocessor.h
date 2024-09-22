#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H


#include "scalablegraphicsview.h"
#include "sizetablemodel.h"
#include "settingsdialog.h"
#include "nodemodel.h"

#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QTableView>
#include <QSplitter>
#include <QMenuBar>
#include <QMessageBox>

class Preprocessor : public QWidget
{
    Q_OBJECT
public:
    Preprocessor(QWidget *parent = nullptr);
    ~Preprocessor();
    void updateScene();
    void openSettings();
    void showAboutDialog();
    void drawKernel();
    void drawNode(qreal maxHeight);
    void drawKernelN(qreal maxHeight);
    void drawDistributedLoad();
    void drawSupport();

private:
    NodeModel *m_nodeModel;
    QTableView *m_nodeTableView;

    SizeTableModel *m_sizeModel;
    QTableView *m_sizeTableView;

    QGraphicsScene *m_scene;
    ScalableGraphicsView *m_view;
};

#endif // PREPROCESSOR_H
