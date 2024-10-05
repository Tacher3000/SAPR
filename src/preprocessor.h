#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H


#include "scalablegraphicsview.h"
#include "sizetablemodel.h"
#include "settingsdialog.h"
#include "nodemodel.h"
#include "bouncingtext.h"


#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QTableView>
#include <QSplitter>
#include <QMenuBar>
#include <QMessageBox>
#include <QFile>
#include <QDataStream>
#include <QTextEdit>
#include <QGraphicsProxyWidget>
#include <QProgressBar>
#include <QPropertyAnimation>

const int RECT_WIDTH_MULTIPLIER = 100;
const int RECT_HEIGHT_MULTIPLIER = 50;


class Preprocessor : public QWidget
{
    Q_OBJECT
public:
    Preprocessor(QWidget *parent = nullptr);
    ~Preprocessor();
    void flyText();
    void updateNodeModel();
    void updateScene();
    void openSettings();
    void showAboutDialog();
    void drawKernel();
    void drawNode(qreal maxHeight);
    void drawKernelN(qreal maxHeight);
    void drawDistributedLoad();
    void drawSupport();
    void drawFocusedLoad();
    void saveModels(const QString &filePath);
    void loadModels(const QString &filePath);
    void clearData();
    void drawKernelWidget();
    void drawDistributedLoadWidget();
private:
    NodeModel *m_nodeModel;
    QTableView *m_nodeTableView;

    SizeTableModel *m_sizeModel;
    QTableView *m_sizeTableView;

    QGraphicsScene *m_scene;
    ScalableGraphicsView *m_view;

    QTimer* m_timer;
};

#endif // PREPROCESSOR_H
