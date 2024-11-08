#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "scalablegraphicsview.h"
#include "sizetablemodel.h"
#include "settingsdialog.h"

#include "preprocessor.h"
#include "processor.h"
#include "postprocessor.h"

#include <QMainWindow>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QTableView>
#include <QSplitter>
#include <QMenuBar>
#include <QMessageBox>
#include <QStackedWidget>
#include <QCloseEvent>
#include <QFileDialog>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void openSettings();
    void showAboutDialog();

    void loadFile();
    void saveFile();
    void saveAsFile();

    void switchToProcessor();
    void switchToPreprocessor();
    void switchToPostprocessor();

private:
    QStackedWidget *m_stackWidget;

    Preprocessor *m_preprocessor;
    Processor *m_processor;
    PostProcessor *m_postProcessor;

    QString m_currentFilePath;
};
#endif // MAINWINDOW_H
