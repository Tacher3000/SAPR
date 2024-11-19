#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H


#include "scalablegraphicsview.h"
#include "sizetablemodel.h"
#include "settingsdialog.h"
#include "nodemodel.h"
#include "bouncingtext.h"
#include "scenedrawer.h"
#include "numericdelegate.h"
#include "positivenumericdelegate.h"
#include "positivedoubledelegate.h"
#include "doubledelegate.h"

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
#include <QPushButton>
#include <QPdfWriter>
#include <QPainter>

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
    void saveModels(const QString &filePath);
    void loadModels(const QString &filePath);
    void clearData();
    NodeModel* getNodeModel() const;
    SizeTableModel* getSizeModel() const;
    QTableView* getSizeTableView() const;
    QTableView* getNodeTableView() const;
    QGraphicsScene* getScene() const;
    void exportModelToPdf(const QString &filePath);
private slots:
    void toProcessor();
signals:
     void clickedToProcessor();
private:
    NodeModel *m_nodeModel;
    QTableView *m_nodeTableView;

    SizeTableModel *m_sizeModel;
    QTableView *m_sizeTableView;

    SceneDrawer *m_sceneDrawer;
    QGraphicsScene *m_scene;
    ScalableGraphicsView *m_view;

    QTimer* m_timer;

    QPushButton *m_toProcessorButton;
};

#endif // PREPROCESSOR_H
