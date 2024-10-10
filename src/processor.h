#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QWidget>
#include <QVector>
#include "sizetablemodel.h"
#include "nodemodel.h"
#include <QDebug>

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

private:
    QVector<QVector<double>> m_matrixA;
    QVector<double> m_vectorB;
};

#endif // PROCESSOR_H
