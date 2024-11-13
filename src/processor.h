#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "sizetablemodel.h"
#include "nodemodel.h"

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QVector>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTableView>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGraphicsView>


#define DEBUG_MATRIX(matrix) \
do { \
        qDebug() << "Matrix"; \
        for (int i = 0; i < matrix.size(); ++i) { \
            qDebug() << matrix[i]; \
    } \
} while (0)

#define DEBUG_HASH(hash) \
do { \
        qDebug() << "Hash:"; \
        for (auto it = hash.constBegin(); it != hash.constEnd(); ++it) { \
            qDebug() << "Key:" << it.key() << ", Value:" << it.value(); \
    } \
} while (0)


class Processor : public QWidget {
    Q_OBJECT

public:
    explicit Processor(QWidget *parent = nullptr);
    // void extracted(const SizeTableModel *&sizeModel, double &modulusValue,
    //                int &nSizeMatrix, int &k);
    void calculate();

    QVector<double> backSubstitution(const QVector<QVector<double> > &A, const QVector<double> &B);
    void gaussianElimination(QVector<QVector<double> > &A, QVector<double> &B);

    const QVector<double>& getVectorNx() const;
    const QVector<double>& getVectorUx() const;
    const QVector<double>& getVectorSigmax() const;
    void setNodeModel(const NodeModel *nodeModel);
    void setSizeModel(const SizeTableModel *sizeModel);
    const void fillTable();
    const double calculationUxAtPoint(int number, double x);
    const double calculationNxAtPoint(int number, double x);
    const double calculationSigmaxAtPoint(int number, double x);
    QStandardItemModel* getTableModel() const;
    QTableView* getTableView() const;
    const double calculationUxAtGlobalPoint(double globalX);
    const double maxGlobalX();
public slots:
    void toPreprocessor();
    void toPostprocessor();
    void changePointEdit(QString value);
signals:
    void clickedToPreprocessor();
    void clickedToPostprocessor();
private:
    void logMatrix(const QVector<QVector<double>> &matrix, const QString &name);
    void logVector(const QVector<double> &vector, const QString &name);

    const NodeModel *m_nodeModel;
    const SizeTableModel *m_sizeModel;

    QVector<QVector<double>> m_matrixA;
    QVector<double> m_vectorB;
    QVector<double> m_vectorDelta;
    QVector<double> m_vectorNx;
    QVector<double> m_vectorUx;
    QVector<double> m_vectorSigmax;

    QMap<double, double> m_mapNx;

    // QTextEdit *m_textEdit;
    QDoubleSpinBox *m_stepSelector;
    QLineEdit *m_pointEdit;

    QLabel *nxInPoint;
    QLabel *uxInPoint;
    QLabel *sigmaxInPoint;

    QPushButton *m_decreaseFromPoint;
    QPushButton *m_addToPoint;

    QStandardItemModel *m_tableModel;
    QTableView *m_tableView;

    // SceneDrawer *m_sceneDrawer;
    QGraphicsScene *m_scene;
    QGraphicsView *m_view;


    QPushButton *m_toPreprocessorButton;
    QPushButton *m_toPostprocessorButton;
};

#endif // PROCESSOR_H
