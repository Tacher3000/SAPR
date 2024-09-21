#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H


#include "scalablegraphicsview.h"
#include "sizetablemodel.h"
#include "settingsdialog.h"

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
    SizeTableModel *model;
    QTableView *tableView;

    QGraphicsScene *m_scene;
    ScalableGraphicsView *m_view;
};

#endif // PREPROCESSOR_H
