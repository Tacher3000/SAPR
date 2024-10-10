#include "processor.h"

Processor::Processor(QWidget *parent) : QWidget(parent) {}

void Processor::calculate(const SizeTableModel *sizeModel, const NodeModel *nodeModel)
{
    int nSizeMatrix = nodeModel->rowCount();
    m_matrixA.resize(nSizeMatrix);

    for (int i = 0; i < nSizeMatrix; ++i) {
        m_matrixA[i].resize(nSizeMatrix);
        for (int j = 0; j < nSizeMatrix; ++j) {
            m_matrixA[i][j] = 0.0;
        }
    }

    QString supportLeft = nodeModel->data(nodeModel->index(0, 1)).toString();
    QString supportRight = nodeModel->data(nodeModel->index(nSizeMatrix - 1, 1)).toString();

    for (int i = 0; i < nSizeMatrix - 1; ++i) {
        double width = sizeModel->data(sizeModel->index(i, 0)).toDouble();
        double height = sizeModel->data(sizeModel->index(i, 1)).toDouble();
        if(i == 0 && supportLeft.isEmpty()){
            m_matrixA[i][i] = width / height;
            m_matrixA[i + 1][i] = -(width / height);
            m_matrixA[i][i + 1] = -(width / height);
            continue;
        }else if(i == 0){
            m_matrixA[i][i] = 1;
            continue;
        }
        if(i == nSizeMatrix - 2 && supportRight.isEmpty()){
            double width2 = sizeModel->data(sizeModel->index(i - 1, 0)).toDouble();
            double height2 = sizeModel->data(sizeModel->index(i - 1, 1)).toDouble();

            m_matrixA[i][i] = (width2 / height2) + (width / height);

            m_matrixA[i + 1][i + 1] = width / height;
            m_matrixA[i][i + 1] = -(width / height);
            m_matrixA[i + 1][i] = -(width / height);
            continue;
        }else if(i == nSizeMatrix - 2){
            double width2 = sizeModel->data(sizeModel->index(i - 1, 0)).toDouble();
            double height2 = sizeModel->data(sizeModel->index(i - 1, 1)).toDouble();

            m_matrixA[i][i] = (width2 / height2) + (width / height);
            m_matrixA[i + 1][i + 1] = 1;
            continue;
        }
        double width2 = sizeModel->data(sizeModel->index(i + 1, 0)).toDouble();
        double height2 = sizeModel->data(sizeModel->index(i + 1, 1)).toDouble();

        m_matrixA[i][i] = (width / height) + (width2 / height2);
        m_matrixA[i + 1][i] = -(width / height);
        m_matrixA[i][i + 1] = -(width / height);

    }

    DEBUG_MATRIX(m_matrixA);

    m_vectorB.resize(nSizeMatrix);

    for (int i = 0; i < nSizeMatrix; ++i) {
        m_vectorB[i] = 0.0;
    }

    for (int i = 0; i < nSizeMatrix; ++i) {
        if(i == 0 && !supportLeft.isEmpty()){
            continue;
        }
        if(i == nSizeMatrix - 1 && !supportRight.isEmpty()){
            continue;
        }
        if(i == 0){
            double loadDirection = sizeModel->data(sizeModel->index(i, 2)).toInt();
            double focusedDirection = nodeModel->data(nodeModel->index(i, 0)).toInt();
            m_vectorB[i] = focusedDirection + loadDirection/2;
        }
        if(i == nSizeMatrix - 1){
            double loadDirection = sizeModel->data(sizeModel->index(i - 1, 2)).toInt();
            double focusedDirection = nodeModel->data(nodeModel->index(i, 0)).toInt();
            m_vectorB[i] = focusedDirection + loadDirection/2;
        }else{
            double loadDirection = sizeModel->data(sizeModel->index(i - 1, 2)).toInt();
            double loadDirection2 = sizeModel->data(sizeModel->index(i, 2)).toInt();
            double focusedDirection = nodeModel->data(nodeModel->index(i, 0)).toInt();
            m_vectorB[i] = focusedDirection + loadDirection/2 + loadDirection2/2;

        }

    }
    DEBUG_MATRIX(m_vectorB);

    QVector<QVector<double>> A = m_matrixA;
    QVector<double> B = m_vectorB;

    gaussianElimination(A, B);

    m_vectorDelta = backSubstitution(A, B);

    DEBUG_MATRIX(m_vectorDelta);

}

void Processor::gaussianElimination(QVector<QVector<double>> &A, QVector<double> &B) {
    int n = A.size();

    for (int i = 0; i < n; ++i) {
        // Находим максимальный элемент в текущем столбце для улучшения численной стабильности
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (fabs(A[k][i]) > fabs(A[maxRow][i])) {
                maxRow = k;
            }
        }

        // Меняем строки местами
        std::swap(A[i], A[maxRow]);
        std::swap(B[i], B[maxRow]);

        // Приводим к единичному элементу на диагонали и обрабатываем остальные строки
        for (int k = i + 1; k < n; ++k) {
            double coeff = A[k][i] / A[i][i];
            for (int j = i; j < n; ++j) {
                A[k][j] -= coeff * A[i][j];
            }
            B[k] -= coeff * B[i];
        }
    }
}

QVector<double> Processor::backSubstitution(const QVector<QVector<double>> &A, const QVector<double> &B) {
    int n = A.size();
    QVector<double> x(n);

    for (int i = n - 1; i >= 0; --i) {
        x[i] = B[i];
        for (int j = i + 1; j < n; ++j) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }

    return x;
}
