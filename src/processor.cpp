#include "processor.h"

#include <QVBoxLayout>

Processor::Processor(QWidget *parent) : QWidget(parent) {
    m_textEdit = new QTextEdit(this);  // Инициализируем QTextEdit

    // Создаем лэйаут
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Добавляем m_textEdit в лэйаут
    layout->addWidget(m_textEdit);

    // Устанавливаем лэйаут для виджета Processor
    setLayout(layout);
}


void Processor::logMatrix(const QVector<QVector<double>> &matrix, const QString &name) {
    QString output = name + ":\n";  // Добавляем название матрицы перед её содержимым
    for (const auto &row : matrix) {
        for (double value : row) {
            output += QString::number(value) + "\t";  // Используем табуляцию
        }
        output += "\n";
    }
    m_textEdit->append(output);
}

void Processor::logVector(const QVector<double> &vector, const QString &name) {
    QString output = name + ":\n";  // Добавляем название вектора перед его содержимым
    for (double value : vector) {
        output += QString::number(value) + "\t";  // Используем табуляцию
    }
    output += "\n";
    m_textEdit->append(output);
}

void Processor::calculate(const SizeTableModel *sizeModel, const NodeModel *nodeModel) {
    double modulusValue = sizeModel->getModulusValue().toDouble();

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
        if (i == 0 && supportLeft.isEmpty()) {
            m_matrixA[i][i] = height / width;
            m_matrixA[i + 1][i] = -(height / width);
            m_matrixA[i][i + 1] = -(height / width);
            continue;
        } else if (i == 0) {
            m_matrixA[i][i] = 1;
            continue;
        }
        if (i == nSizeMatrix - 2 && supportRight.isEmpty()) {
            double width2 = sizeModel->data(sizeModel->index(i - 1, 0)).toDouble();
            double height2 = sizeModel->data(sizeModel->index(i - 1, 1)).toDouble();

            m_matrixA[i][i] = (height2 / width2) + (height / width);

            m_matrixA[i + 1][i + 1] = height / width;
            m_matrixA[i][i + 1] = -(height / width);
            m_matrixA[i + 1][i] = -(height / width);
            continue;
        } else if (i == nSizeMatrix - 2) {
            double width2 = sizeModel->data(sizeModel->index(i - 1, 0)).toDouble();
            double height2 = sizeModel->data(sizeModel->index(i - 1, 1)).toDouble();

            m_matrixA[i][i] = (height2 / width2) + (height / width);
            m_matrixA[i + 1][i + 1] = 1;
            continue;
        }
        double width2 = sizeModel->data(sizeModel->index(i - 1, 0)).toDouble();
        double height2 = sizeModel->data(sizeModel->index(i - 1, 1)).toDouble();

        m_matrixA[i][i] = (height / width) + (height2 / width2);
        m_matrixA[i + 1][i] = -(height / width);
        m_matrixA[i][i + 1] = -(height / width);
    }

    // Логирование матрицы A
    logMatrix(m_matrixA, "Matrix A");

    m_vectorB.resize(nSizeMatrix);
    for (int i = 0; i < nSizeMatrix; ++i) {
        m_vectorB[i] = 0.0;
    }

    for (int i = 0; i < nSizeMatrix; ++i) {
        if (i == 0 && !supportLeft.isEmpty()) {
            continue;
        }
        if (i == nSizeMatrix - 1 && !supportRight.isEmpty()) {
            continue;
        }
        if (i == 0) {
            double loadDirection = sizeModel->data(sizeModel->index(i, 2)).toInt();
            double focusedDirection = nodeModel->data(nodeModel->index(i, 0)).toInt();
            m_vectorB[i] = focusedDirection + loadDirection / 2;
        }
        if (i == nSizeMatrix - 1) {
            double loadDirection = sizeModel->data(sizeModel->index(i - 1, 2)).toInt();
            double focusedDirection = nodeModel->data(nodeModel->index(i, 0)).toInt();
            m_vectorB[i] = focusedDirection + loadDirection / 2;
        } else {
            double loadDirection = sizeModel->data(sizeModel->index(i - 1, 2)).toInt();
            double loadDirection2 = sizeModel->data(sizeModel->index(i, 2)).toInt();
            double focusedDirection = nodeModel->data(nodeModel->index(i, 0)).toInt();
            m_vectorB[i] = focusedDirection + loadDirection / 2 + loadDirection2 / 2;
        }
    }

    // Логирование вектора B
    logVector(m_vectorB, "Vector B");

    QVector<QVector<double>> A = m_matrixA;
    QVector<double> B = m_vectorB;

    gaussianElimination(A, B);

    m_vectorDelta = backSubstitution(A, B);

    // Логирование вектора Delta
    logVector(m_vectorDelta, "Vector Delta");

    m_vectorNx.resize((nSizeMatrix - 1) * 2);

    int k = 0;

    for (int i = 0; i < (nSizeMatrix - 1) * 2; ++i) {
        double width = sizeModel->data(sizeModel->index(i / 2, 0)).toDouble();
        double height = sizeModel->data(sizeModel->index(i / 2, 1)).toDouble();
        double loadDirection = sizeModel->data(sizeModel->index(i / 2, 2)).toInt();

        if (i % 2 == 0) {
            m_vectorNx[i] = modulusValue * height / width * (m_vectorDelta[k + 1] - m_vectorDelta[k]) +
                            loadDirection * width * (1 - 2 * 0 / width) / 2;
        } else {
            m_vectorNx[i] = modulusValue * height / width * (m_vectorDelta[k + 1] - m_vectorDelta[k]) +
                            loadDirection * width * (1 - 2 * width / width) / 2;
            ++k;
        }
    }

    // Логирование вектора Nx
    logVector(m_vectorNx, "Vector Nx");
}



void Processor::gaussianElimination(QVector<QVector<double>> &A, QVector<double> &B) {
    int n = A.size();

    for (int i = 0; i < n; ++i) {
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (fabs(A[k][i]) > fabs(A[maxRow][i])) {
                maxRow = k;
            }
        }

        std::swap(A[i], A[maxRow]);
        std::swap(B[i], B[maxRow]);

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
