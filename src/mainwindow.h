#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scalablegraphicsview.h"
#include "sizetablemodel.h"
#include "settingsdialog.h"

#include "preprocessor.h"

#include <QMainWindow>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QTableView>
#include <QSplitter>
#include <QMenuBar>
#include <QMessageBox>
#include <QStackedWidget>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void openSettings();
    void showAboutDialog();

private:
    Preprocessor *m_preprocessor;
};
#endif // MAINWINDOW_H
