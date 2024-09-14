#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scalablegraphicsview.h"
#include "sizetablemodel.h"
#include "settingsdialog.h"

#include <QMainWindow>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QTableView>
#include <QSplitter>
#include <QMenuBar>
#include <QMessageBox>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateScene();
    void openSettings();
    void showAboutDialog();
    void drawNode(qreal maxHeight);
    void drawKernel(qreal maxHeight);
private:
    SizeTableModel *model;
    QTableView *tableView;

    QGraphicsScene *m_scene;
    ScalableGraphicsView *m_view;
};
#endif // MAINWINDOW_H
