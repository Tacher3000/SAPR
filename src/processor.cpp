#include "processor.h"

Processor::Processor(QWidget *parent) : QWidget(parent) {
    m_toPreprocessorButton = new QPushButton("Назад", this);
    m_toPostprocessorButton = new QPushButton("Вперед", this);
    m_toPreprocessorButton->setMinimumHeight(40);
    m_toPostprocessorButton->setMinimumHeight(40);

    connect(m_toPreprocessorButton, &QPushButton::clicked, this, &Processor::toPreprocessor);
    connect(m_toPostprocessorButton, &QPushButton::clicked, this, &Processor::toPostprocessor);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_toPreprocessorButton);
    buttonLayout->addWidget(m_toPostprocessorButton);

    m_textEdit = new QTextEdit(this);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_textEdit);
    layout->addLayout(buttonLayout);

    setLayout(layout);
}


void Processor::logMatrix(const QVector<QVector<double>> &matrix, const QString &name) {
    QString output = name + ":\n";
    for (const auto &row : matrix) {
        for (double value : row) {
            output += QString::number(value) + "\t";
        }
        output += "\n";
    }
    m_textEdit->append(output);
}

void Processor::logVector(const QVector<double> &vector, const QString &name) {
    QString output = name + ":\n";
    for (double value : vector) {
        output += QString::number(value) + "\t";
    }
    output += "\n";
    m_textEdit->append(output);
}

const QVector<double>& Processor::getVectorNx() const
{
    return m_vectorNx;
}

const QVector<double> &Processor::getVectorUx() const
{
    return m_vectorUx;
}

void Processor::calculate(const SizeTableModel *sizeModel, const NodeModel *nodeModel) {
    m_textEdit->clear();
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
            double width = sizeModel->data(sizeModel->index(i, 0)).toDouble();
            double loadDirection = sizeModel->data(sizeModel->index(i, 2)).toDouble();
            double focusedDirection = nodeModel->data(nodeModel->index(i, 0)).toDouble();
            m_vectorB[i] = focusedDirection + loadDirection * width / 2;
        }
        if (i == nSizeMatrix - 1) {
            double width = sizeModel->data(sizeModel->index(i - 1, 0)).toDouble();
            double loadDirection = sizeModel->data(sizeModel->index(i - 1, 2)).toDouble();
            double focusedDirection = nodeModel->data(nodeModel->index(i, 0)).toDouble();
            m_vectorB[i] = focusedDirection + loadDirection * width / 2;
        } else {
            double width1 = sizeModel->data(sizeModel->index(i - 1, 0)).toDouble();
            double width2 = sizeModel->data(sizeModel->index(i, 0)).toDouble();
            double loadDirection = sizeModel->data(sizeModel->index(i - 1, 2)).toDouble();
            double loadDirection2 = sizeModel->data(sizeModel->index(i, 2)).toDouble();
            double focusedDirection = nodeModel->data(nodeModel->index(i, 0)).toDouble();
            m_vectorB[i] = focusedDirection + loadDirection * width1 / 2 + loadDirection2 * width2 / 2;
        }
    }

    logVector(m_vectorB, "Vector B");

    QVector<QVector<double>> A = m_matrixA;
    QVector<double> B = m_vectorB;

    gaussianElimination(A, B);

    m_vectorDelta = backSubstitution(A, B);

    logVector(m_vectorDelta, "Vector Delta");

    m_vectorNx.resize((nSizeMatrix - 1) * 2);

    int k = 0;

    for (int i = 0; i < (nSizeMatrix - 1) * 2; ++i) {
        double width = sizeModel->data(sizeModel->index(i / 2, 0)).toDouble();
        double height = sizeModel->data(sizeModel->index(i / 2, 1)).toDouble();
        double loadDirection = sizeModel->data(sizeModel->index(i / 2, 2)).toDouble();

        if (i % 2 == 0) {
            m_vectorNx[i] = modulusValue * height / width * (m_vectorDelta[k + 1] - m_vectorDelta[k]) +
                            loadDirection * width * (1 - 2 * 0 / width) / 2;
        } else {
            m_vectorNx[i] = modulusValue * height / width * (m_vectorDelta[k + 1] - m_vectorDelta[k]) +
                            loadDirection * width * (1 - 2 * width / width) / 2;
            ++k;
        }
    }

    logVector(m_vectorNx, "Vector Nx");

    m_vectorUx.resize((nSizeMatrix - 1) * 2);

    k = 0;

    for (int i = 0; i < (nSizeMatrix - 1) * 2; ++i) {
        double width = sizeModel->data(sizeModel->index(i / 2, 0)).toDouble();
        double height = sizeModel->data(sizeModel->index(i / 2, 1)).toDouble();
        double loadDirection = sizeModel->data(sizeModel->index(i / 2, 2)).toDouble();

        if (i % 2 == 0) {
            m_vectorUx[i] = m_vectorDelta[k] + 0 + 0;
        } else {
            m_vectorUx[i] = m_vectorDelta[k] + (width / width) * (m_vectorDelta[k + 1] - m_vectorDelta[k]) +
                            (loadDirection * width * width) / (2 * modulusValue * height) * (1 - width / width);
            ++k;
        }
    }
    logVector(m_vectorUx, "Vector Ux");
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

void Processor::toPreprocessor() {
    emit clickedToPreprocessor();
}

void Processor::toPostprocessor() {
    emit clickedToPostprocessor();
}
