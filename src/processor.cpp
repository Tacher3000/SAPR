#include "processor.h"

Processor::Processor(QWidget *parent) : QWidget(parent) {
    m_stepSelector = new QDoubleSpinBox(this);
    m_stepSelector->setMinimum(0.01);
    m_stepSelector->setMaximum(1.0);
    m_stepSelector->setSingleStep(0.01);
    m_stepSelector->setValue(0.25);
    m_stepSelector->setMinimumHeight(40);
    m_stepSelector->setMaximumWidth(120);
    // QLabel *stepLabel = new QLabel("Шаг:", this);

    connect(m_stepSelector, &QDoubleSpinBox::valueChanged, this, &Processor::stepChanged);


    m_toPreprocessorButton = new QPushButton("Назад", this);
    m_toPostprocessorButton = new QPushButton("Вперед", this);
    m_toPreprocessorButton->setMinimumHeight(40);
    m_toPostprocessorButton->setMinimumHeight(40);

    connect(m_toPreprocessorButton, &QPushButton::clicked, this, &Processor::toPreprocessor);
    connect(m_toPostprocessorButton, &QPushButton::clicked, this, &Processor::toPostprocessor);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_toPreprocessorButton);
    buttonLayout->addWidget(m_toPostprocessorButton);

    m_tableModel = new QStandardItemModel(this);
    m_tableModel->setColumnCount(6);
    m_tableModel->setHeaderData(0, Qt::Horizontal, "Номер стержня");
    m_tableModel->setHeaderData(1, Qt::Horizontal, "Локальная координата");
    m_tableModel->setHeaderData(2, Qt::Horizontal, "Глобальная координата");
    m_tableModel->setHeaderData(3, Qt::Horizontal, "Nx");
    m_tableModel->setHeaderData(4, Qt::Horizontal, "Ux");
    m_tableModel->setHeaderData(5, Qt::Horizontal, "Предельное значение");

    m_tableView = new QTableView(this);
    m_tableView->setModel(m_tableModel);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_tableView->resizeColumnsToContents();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_stepSelector);
    layout->addWidget(m_tableView);
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
    // m_textEdit->append(output);
}

void Processor::logVector(const QVector<double> &vector, const QString &name) {
    QString output = name + ":\n";
    for (double value : vector) {
        output += QString::number(value) + "\t";
    }
    output += "\n";
    // m_textEdit->append(output);
}

void Processor::fillTable(const SizeTableModel *sizeModel, const NodeModel *nodeModel)
{
    m_tableModel->removeRows(0, m_tableModel->rowCount());
    double step = m_stepSelector->value();
    double currentStep = 0;
    int deltaIndex = 0;
    int rowIndex = 0;
    double nxValue;
    double uxValue;
    double cumulativeWidth = 0;
    double globalCoordinate = 0;
    double globalCoordinateRepeated = 0;
    double modulusValue = sizeModel->getModulusValue().toDouble();

    while (true) {
        double width = sizeModel->data(sizeModel->index(deltaIndex, 0)).toDouble();
        double height = sizeModel->data(sizeModel->index(deltaIndex, 1)).toDouble();
        double loadDirection = sizeModel->data(sizeModel->index(deltaIndex, 2)).toDouble();



        if(width == 0 || height == 0){
            break;
        }

        currentStep = 0;
        while(step * currentStep  <= width){
            double localCoordinate = step * currentStep;

            nxValue = modulusValue * height / width * (m_vectorDelta[deltaIndex + 1] - m_vectorDelta[deltaIndex]) +
                          loadDirection * width * (1.0 - 2.0 * step * currentStep / width) / 2.0;

            uxValue = m_vectorDelta[deltaIndex] + (step * currentStep / width) * (m_vectorDelta[deltaIndex + 1] - m_vectorDelta[deltaIndex]) +
                            (loadDirection * width * step * currentStep) / (2 * modulusValue * height) * (1 - step * currentStep / width);

            QString nxValueStr = QString::number(nxValue, 'f', 4).remove(QRegularExpression("\\.?0+$"));
            QString uxValueStr = QString::number(uxValue, 'f', 4).remove(QRegularExpression("\\.?0+$"));
            if (nxValueStr == "-0") nxValueStr = "0";
            if (uxValueStr == "-0") uxValueStr = "0";

            m_tableModel->setItem(rowIndex, 0, new QStandardItem(QString::number(deltaIndex + 1)));
            m_tableModel->setItem(rowIndex, 1, new QStandardItem(QString::number(localCoordinate)));
            m_tableModel->setItem(rowIndex, 2, new QStandardItem(QString::number(globalCoordinateRepeated)));
            m_tableModel->setItem(rowIndex, 3, new QStandardItem(nxValueStr));
            m_tableModel->setItem(rowIndex, 4, new QStandardItem(uxValueStr));


            globalCoordinate += step;
            globalCoordinateRepeated += step;
            ++currentStep;
            ++rowIndex;
        }
        globalCoordinateRepeated -=step;
        cumulativeWidth  += width;
        ++deltaIndex;
    }
    m_tableView->resizeColumnsToContents();
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
    // m_textEdit->clear();
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
            m_matrixA[i][i] = height / width * modulusValue;
            m_matrixA[i + 1][i] = -(height / width * modulusValue);
            m_matrixA[i][i + 1] = -(height / width * modulusValue);
            continue;
        } else if (i == 0) {
            m_matrixA[i][i] = 1;
            continue;
        }
        if (i == nSizeMatrix - 2 && supportRight.isEmpty()) {
            double width2 = sizeModel->data(sizeModel->index(i - 1, 0)).toDouble();
            double height2 = sizeModel->data(sizeModel->index(i - 1, 1)).toDouble();

            m_matrixA[i][i] = (height2 / width2 * modulusValue) + (height / width * modulusValue);

            m_matrixA[i + 1][i + 1] = height / width * modulusValue;
            m_matrixA[i][i + 1] = -(height / width * modulusValue);
            m_matrixA[i + 1][i] = -(height / width * modulusValue);
            continue;
        } else if (i == nSizeMatrix - 2) {
            double width2 = sizeModel->data(sizeModel->index(i - 1, 0)).toDouble();
            double height2 = sizeModel->data(sizeModel->index(i - 1, 1)).toDouble();

            m_matrixA[i][i] = (height2 / width2 * modulusValue) + (height / width * modulusValue);
            m_matrixA[i + 1][i + 1] = 1;
            continue;
        }
        double width2 = sizeModel->data(sizeModel->index(i - 1, 0)).toDouble();
        double height2 = sizeModel->data(sizeModel->index(i - 1, 1)).toDouble();

        m_matrixA[i][i] = (height / width * modulusValue) + (height2 / width2 * modulusValue);
        m_matrixA[i + 1][i] = -(height / width * modulusValue);
        m_matrixA[i][i + 1] = -(height / width * modulusValue);
    }

    // logMatrix(m_matrixA, "Matrix A");

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

    // logVector(m_vectorB, "Vector B");

    QVector<QVector<double>> A = m_matrixA;
    QVector<double> B = m_vectorB;

    gaussianElimination(A, B);

    m_vectorDelta = backSubstitution(A, B);

    // logVector(m_vectorDelta, "Vector Delta");



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

    fillTable(sizeModel, nodeModel);
    // logVector(m_vectorNx, "Vector Nx");

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
    // DEBUG_MATRIX(m_vectorUx);
    // logVector(m_vectorUx, "Vector Ux");
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
