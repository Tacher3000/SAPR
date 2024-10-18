#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "sizetablemodel.h"
#include "nodemodel.h"

#include <QWidget>
#include <QTextEdit>
#include <QVector>
#include <QDebug>
#include <QVBoxLayout>


#define DEBUG_MATRIX(matrix) \
do { \
        qDebug() << "Matrix"; \
        for (int i = 0; i < matrix.size(); ++i) { \
            qDebug() << matrix[i]; \
    } \
} while (0)

class Processor : public QWidget {
    Q_OBJECT

public:
    explicit Processor(QWidget *parent = nullptr);
    void calculate(const SizeTableModel *sizeModel, const NodeModel *nodeModel);

    QVector<double> backSubstitution(const QVector<QVector<double> > &A, const QVector<double> &B);
    void gaussianElimination(QVector<QVector<double> > &A, QVector<double> &B);
private:
    void logMatrix(const QVector<QVector<double>> &matrix, const QString &name);
    void logVector(const QVector<double> &vector, const QString &name);

    QVector<QVector<double>> m_matrixA;
    QVector<double> m_vectorB;
    QVector<double> m_vectorDelta;
    QVector<double> m_vectorNx;
    QTextEdit *m_textEdit;
};

#endif // PROCESSOR_H
