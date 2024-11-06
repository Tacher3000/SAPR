#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "sizetablemodel.h"
#include "nodemodel.h"

#include <QWidget>
#include <QTextEdit>
#include <QVector>
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>


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
    void extracted(const SizeTableModel *&sizeModel, double &modulusValue,
                   int &nSizeMatrix, int &k);
    void calculate(const SizeTableModel *sizeModel, const NodeModel *nodeModel);

    QVector<double> backSubstitution(const QVector<QVector<double> > &A, const QVector<double> &B);
    void gaussianElimination(QVector<QVector<double> > &A, QVector<double> &B);

    const QVector<double>& getVectorNx() const;
    const QVector<double>& getVectorUx() const;
public slots:
    void toPreprocessor();
    void toPostprocessor();
signals:
    void clickedToPreprocessor();
    void clickedToPostprocessor();
private:
    void logMatrix(const QVector<QVector<double>> &matrix, const QString &name);
    void logVector(const QVector<double> &vector, const QString &name);

    void fillHashNx(const SizeTableModel *sizeModel, const NodeModel *nodeModel);

    QVector<QVector<double>> m_matrixA;
    QVector<double> m_vectorB;
    QVector<double> m_vectorDelta;
    QVector<double> m_vectorNx;
    QVector<double> m_vectorUx;

    QHash<double, double> m_hashNx;

    // QTextEdit *m_textEdit;

    QPushButton *m_toPreprocessorButton;
    QPushButton *m_toPostprocessorButton;
};

#endif // PROCESSOR_H
