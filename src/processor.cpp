#include "processor.h"

Processor::Processor(QWidget *parent) : QWidget(parent) {
    QLabel *stepLabel = new QLabel("Шаг:", this);

    m_stepSelector = new QDoubleSpinBox(this);
    m_stepSelector->setMinimum(0.01);
    m_stepSelector->setMaximum(1.0);
    m_stepSelector->setSingleStep(0.01);
    m_stepSelector->setValue(0.25);
    m_stepSelector->setMinimumHeight(40);
    m_stepSelector->setMaximumWidth(120);
    connect(m_stepSelector, &QDoubleSpinBox::valueChanged, this, &Processor::fillTable);

    QLabel *pointLabel = new QLabel("В точке:", this);

    m_pointEdit = new QLineEdit(this);
    m_pointEdit->setMinimumHeight(40);
    m_pointEdit->setMaximumWidth(120);

    connect(m_pointEdit, &QLineEdit::textEdited, this, &Processor::changePointEdit);

    nxInPoint = new QLabel("Nx:", this);
    uxInPoint = new QLabel("Ux:", this);
    sigmaxInPoint = new QLabel("σx:", this);

    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(stepLabel);
    topLayout->addWidget(m_stepSelector);
    topLayout->addWidget(line);
    topLayout->addWidget(pointLabel);
    topLayout->addWidget(m_pointEdit);
    topLayout->addWidget(nxInPoint);
    topLayout->addWidget(uxInPoint);
    topLayout->addWidget(sigmaxInPoint);
    topLayout->addStretch();

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
    m_tableModel->setColumnCount(7);
    m_tableModel->setHeaderData(0, Qt::Horizontal, "Номер стержня");
    m_tableModel->setHeaderData(1, Qt::Horizontal, "Локальная координата");
    m_tableModel->setHeaderData(2, Qt::Horizontal, "Глобальная координата");
    m_tableModel->setHeaderData(3, Qt::Horizontal, "Nx");
    m_tableModel->setHeaderData(4, Qt::Horizontal, "Ux");
    m_tableModel->setHeaderData(5, Qt::Horizontal, "σx");
    m_tableModel->setHeaderData(6, Qt::Horizontal, "Предельное значение");

    m_tableView = new QTableView(this);
    m_tableView->setModel(m_tableModel);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_tableView->resizeColumnsToContents();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(topLayout);
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

const void Processor::fillTable()
{
    m_tableModel->removeRows(0, m_tableModel->rowCount());
    double step = m_stepSelector->value();
    double currentStep = 0;
    int deltaIndex = 0;
    int rowIndex = 0;
    double nxValue;
    double uxValue;
    double sigmaValue;
    double cumulativeWidth = 0;
    double globalCoordinate = 0;
    double globalCoordinateRepeated = 0;
    double modulusValue = m_sizeModel->getModulusValue().toDouble();

    while (true) {
        double width = m_sizeModel->data(m_sizeModel->index(deltaIndex, 0)).toDouble();
        double height = m_sizeModel->data(m_sizeModel->index(deltaIndex, 1)).toDouble();
        double loadDirection = m_sizeModel->data(m_sizeModel->index(deltaIndex, 2)).toDouble();
        QString limitValueStr  = m_sizeModel->data(m_sizeModel->index(deltaIndex, 3)).toString();
        double limitValue = limitValueStr.toDouble();



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

            sigmaValue = nxValue/ height;

            QString nxValueStr = QString::number(nxValue, 'f', 4).remove(QRegularExpression("\\.?0+$"));
            QString uxValueStr = QString::number(uxValue, 'f', 4).remove(QRegularExpression("\\.?0+$"));
            QString sigmaValueStr = QString::number(sigmaValue, 'f', 4).remove(QRegularExpression("\\.?0+$"));
            if (nxValueStr == "-0") nxValueStr = "0";
            if (uxValueStr == "-0") uxValueStr = "0";
            if (uxValueStr == "-0") uxValueStr = "0";

            m_tableModel->setItem(rowIndex, 0, new QStandardItem(QString::number(deltaIndex + 1)));
            m_tableModel->setItem(rowIndex, 1, new QStandardItem(QString::number(localCoordinate)));
            m_tableModel->setItem(rowIndex, 2, new QStandardItem(QString::number(globalCoordinateRepeated)));
            m_tableModel->setItem(rowIndex, 3, new QStandardItem(nxValueStr));
            m_tableModel->setItem(rowIndex, 4, new QStandardItem(uxValueStr));

            QStandardItem *sigmaItem = new QStandardItem(sigmaValueStr);
            if (sigmaValue > limitValue) {
                sigmaItem->setData(QColor(Qt::red), Qt::BackgroundRole);
            }
            m_tableModel->setItem(rowIndex, 5, sigmaItem);

            m_tableModel->setItem(rowIndex, 6, new QStandardItem(limitValueStr));

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

const double Processor::calculationUxAtPoint(int number, double x)
{
    double width = m_sizeModel->data(m_sizeModel->index(number, 0)).toDouble();
    double height = m_sizeModel->data(m_sizeModel->index(number, 1)).toDouble();
    double loadDirection = m_sizeModel->data(m_sizeModel->index(number, 2)).toDouble();
    double modulusValue = m_sizeModel->getModulusValue().toDouble();
    return m_vectorDelta[number] + (x / width) * (m_vectorDelta[number + 1] - m_vectorDelta[number]) +
           (loadDirection * width * x) / (2 * modulusValue * height) * (1 - x / width);
}

const double Processor::calculationUxAtGlobalPoint(double globalX)
{
    int number = -1;
    double localX = 0.0;
    double currentX = 0.0;

    for (int i = 0; i < m_sizeModel->rowCount(); ++i) {
        double width = m_sizeModel->data(m_sizeModel->index(i, 0)).toDouble();

        if (globalX <= currentX + width) {
            number = i;
            localX = globalX - currentX;
            break;
        }
        currentX += width;
    }

    if (number == -1) {
        throw std::out_of_range("Глобальная координата за пределами");
    }

    double width = m_sizeModel->data(m_sizeModel->index(number, 0)).toDouble();
    double height = m_sizeModel->data(m_sizeModel->index(number, 1)).toDouble();
    double loadDirection = m_sizeModel->data(m_sizeModel->index(number, 2)).toDouble();
    double modulusValue = m_sizeModel->getModulusValue().toDouble();

    return m_vectorDelta[number] + (localX / width) * (m_vectorDelta[number + 1] - m_vectorDelta[number]) +
           (loadDirection * width * localX) / (2 * modulusValue * height) * (1 - localX / width);
}

const double Processor::calculationNxAtGlobalPoint(double globalX)
{
    int number = -1;
    double localX = 0.0;
    double currentX = 0.0;

    for (int i = 0; i < m_sizeModel->rowCount(); ++i) {
        double width = m_sizeModel->data(m_sizeModel->index(i, 0)).toDouble();

        if (globalX <= currentX + width) {
            number = i;
            localX = globalX - currentX;
            break;
        }
        currentX += width;
    }

    if (number == -1) {
        throw std::out_of_range("Глобальная координата за пределами");
    }

    double width = m_sizeModel->data(m_sizeModel->index(number, 0)).toDouble();
    double height = m_sizeModel->data(m_sizeModel->index(number, 1)).toDouble();
    double loadDirection = m_sizeModel->data(m_sizeModel->index(number, 2)).toDouble();
    double modulusValue = m_sizeModel->getModulusValue().toDouble();

    return modulusValue * height / width * (m_vectorDelta[number + 1] - m_vectorDelta[number]) +
           loadDirection * width * (1.0 - 2.0 * localX / width) / 2.0;
}

const double Processor::calculationSigmaxAtGlobalPoint(double globalX)
{
    int number = -1;
    double localX = 0.0;
    double currentX = 0.0;

    for (int i = 0; i < m_sizeModel->rowCount(); ++i) {
        double width = m_sizeModel->data(m_sizeModel->index(i, 0)).toDouble();

        if (globalX <= currentX + width) {
            number = i;
            localX = globalX - currentX;
            break;
        }
        currentX += width;
    }

    if (number == -1) {
        throw std::out_of_range("Глобальная координата за пределами");
    }

    double width = m_sizeModel->data(m_sizeModel->index(number, 0)).toDouble();
    double height = m_sizeModel->data(m_sizeModel->index(number, 1)).toDouble();
    double loadDirection = m_sizeModel->data(m_sizeModel->index(number, 2)).toDouble();
    double modulusValue = m_sizeModel->getModulusValue().toDouble();

    return (modulusValue * height / width * (m_vectorDelta[number + 1] - m_vectorDelta[number]) +
            loadDirection * width * (1.0 - 2.0 * localX / width) / 2.0) / height;
}

QStandardItemModel *Processor::getTableModel() const
{
    return m_tableModel;
}

QTableView *Processor::getTableView() const
{
    return m_tableView;
}

const QVector<double>& Processor::getVectorNx() const
{
    return m_vectorNx;
}

const QVector<double> &Processor::getVectorUx() const
{
    return m_vectorUx;
}

const QVector<double> &Processor::getVectorSigmax() const
{
    return m_vectorSigmax;
}

void Processor::setNodeModel(const NodeModel *nodeModel) {
    m_nodeModel = nodeModel;
}

void Processor::setSizeModel(const SizeTableModel *sizeModel) {
    m_sizeModel = sizeModel;
}

void Processor::calculate() {
    // m_textEdit->clear();
    double modulusValue = m_sizeModel->getModulusValue().toDouble();

    int nSizeMatrix = m_nodeModel->rowCount();
    m_matrixA.resize(nSizeMatrix);

    for (int i = 0; i < nSizeMatrix; ++i) {
        m_matrixA[i].resize(nSizeMatrix);
        for (int j = 0; j < nSizeMatrix; ++j) {
            m_matrixA[i][j] = 0.0;
        }
    }

    QString supportLeft = m_nodeModel->data(m_nodeModel->index(0, 1)).toString();
    QString supportRight = m_nodeModel->data(m_nodeModel->index(nSizeMatrix - 1, 1)).toString();

    for (int i = 0; i < nSizeMatrix - 1; ++i) {
        double width = m_sizeModel->data(m_sizeModel->index(i, 0)).toDouble();
        double height = m_sizeModel->data(m_sizeModel->index(i, 1)).toDouble();
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
            double width2 = m_sizeModel->data(m_sizeModel->index(i - 1, 0)).toDouble();
            double height2 = m_sizeModel->data(m_sizeModel->index(i - 1, 1)).toDouble();

            m_matrixA[i][i] = (height2 / width2 * modulusValue) + (height / width * modulusValue);

            m_matrixA[i + 1][i + 1] = height / width * modulusValue;
            m_matrixA[i][i + 1] = -(height / width * modulusValue);
            m_matrixA[i + 1][i] = -(height / width * modulusValue);
            continue;
        } else if (i == nSizeMatrix - 2) {
            double width2 = m_sizeModel->data(m_sizeModel->index(i - 1, 0)).toDouble();
            double height2 = m_sizeModel->data(m_sizeModel->index(i - 1, 1)).toDouble();

            m_matrixA[i][i] = (height2 / width2 * modulusValue) + (height / width * modulusValue);
            m_matrixA[i + 1][i + 1] = 1;
            continue;
        }
        double width2 = m_sizeModel->data(m_sizeModel->index(i - 1, 0)).toDouble();
        double height2 = m_sizeModel->data(m_sizeModel->index(i - 1, 1)).toDouble();

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
            double width = m_sizeModel->data(m_sizeModel->index(i, 0)).toDouble();
            double loadDirection = m_sizeModel->data(m_sizeModel->index(i, 2)).toDouble();
            double focusedDirection = m_nodeModel->data(m_nodeModel->index(i, 0)).toDouble();
            m_vectorB[i] = focusedDirection + loadDirection * width / 2;
        }
        if (i == nSizeMatrix - 1) {
            double width = m_sizeModel->data(m_sizeModel->index(i - 1, 0)).toDouble();
            double loadDirection = m_sizeModel->data(m_sizeModel->index(i - 1, 2)).toDouble();
            double focusedDirection = m_nodeModel->data(m_nodeModel->index(i, 0)).toDouble();
            m_vectorB[i] = focusedDirection + loadDirection * width / 2;
        } else {
            double width1 = m_sizeModel->data(m_sizeModel->index(i - 1, 0)).toDouble();
            double width2 = m_sizeModel->data(m_sizeModel->index(i, 0)).toDouble();
            double loadDirection = m_sizeModel->data(m_sizeModel->index(i - 1, 2)).toDouble();
            double loadDirection2 = m_sizeModel->data(m_sizeModel->index(i, 2)).toDouble();
            double focusedDirection = m_nodeModel->data(m_nodeModel->index(i, 0)).toDouble();
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
        double width = m_sizeModel->data(m_sizeModel->index(i / 2, 0)).toDouble();
        double height = m_sizeModel->data(m_sizeModel->index(i / 2, 1)).toDouble();
        double loadDirection = m_sizeModel->data(m_sizeModel->index(i / 2, 2)).toDouble();
        double nxValue;

        if (i % 2 == 0) {
            nxValue = modulusValue * height / width * (m_vectorDelta[k + 1] - m_vectorDelta[k]) +
                            loadDirection * width * (1 - 2 * 0 / width) / 2;
        } else {
            nxValue = modulusValue * height / width * (m_vectorDelta[k + 1] - m_vectorDelta[k]) +
                            loadDirection * width * (1 - 2 * width / width) / 2;
            ++k;
        }
        QString nxValueStr = QString::number(nxValue, 'f', 4).remove(QRegularExpression("\\.?0+$"));
        if (nxValueStr == "-0") nxValueStr = "0";
        m_vectorNx[i] = nxValueStr.toDouble();
    }

    fillTable();
    // logVector(m_vectorNx, "Vector Nx");

    m_vectorUx.resize((nSizeMatrix - 1) * 2);

    k = 0;

    for (int i = 0; i < (nSizeMatrix - 1) * 2; ++i) {
        double width = m_sizeModel->data(m_sizeModel->index(i / 2, 0)).toDouble();
        double height = m_sizeModel->data(m_sizeModel->index(i / 2, 1)).toDouble();
        double loadDirection = m_sizeModel->data(m_sizeModel->index(i / 2, 2)).toDouble();
        double uxValue;
        if (i % 2 == 0) {
            uxValue = m_vectorDelta[k] + 0 + 0;
        } else {
            uxValue = m_vectorDelta[k] + (width / width) * (m_vectorDelta[k + 1] - m_vectorDelta[k]) +
                            (loadDirection * width * width) / (2 * modulusValue * height) * (1 - width / width);
            ++k;
        }
        QString uxValueStr = QString::number(uxValue, 'f', 4).remove(QRegularExpression("\\.?0+$"));
        if (uxValueStr == "-0") uxValueStr = "0";
        m_vectorUx[i] = uxValueStr.toDouble();
    }
    // DEBUG_MATRIX(m_vectorUx);
    // logVector(m_vectorUx, "Vector Ux");

    m_vectorSigmax.resize((nSizeMatrix - 1) * 2);

    k = 0;

    for (int i = 0; i < (nSizeMatrix - 1) * 2; ++i) {
        double height = m_sizeModel->data(m_sizeModel->index(i / 2, 1)).toDouble();
        double sigmaValue = m_vectorNx[i] / height;

        QString sigmaValueStr = QString::number(sigmaValue, 'f', 4).remove(QRegularExpression("\\.?0+$"));
        if (sigmaValueStr == "-0") sigmaValueStr = "0";
        m_vectorSigmax[i] = sigmaValueStr.toDouble();
    }
    // DEBUG_MATRIX(m_vectorSigmax);
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

void Processor::changePointEdit(QString value)
{
    nxInPoint->setText("Nx: " + QString::number(calculationNxAtGlobalPoint(value.toDouble())));
    uxInPoint->setText("Ux: " + QString::number(calculationUxAtGlobalPoint(value.toDouble())));
    sigmaxInPoint->setText("σx: " + QString::number(calculationSigmaxAtGlobalPoint(value.toDouble())));
}
