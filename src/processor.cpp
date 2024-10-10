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
    DEBUG_MATRIX(m_matrixA);

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

    // for (int i = 0; i < nSizeMatrix; ++i) {
    //     m_vectorB[i] = 0.0;

    // }
    // DEBUG_MATRIX(m_vectorB);

    // for (int i = 0; i < nSizeMatrix - 1; ++i) {
    //     if(i == 0 && supportLeft.isEmpty()){
    //         continue;
    //     }else if(i == 0){
    //         continue;
    //     }
    //     if(i == nSizeMatrix - 2 && supportRight.isEmpty()){
    //         continue;
    //     }else if(i == 0){
    //         continue;
    //     }
    //     // int direction = sizeModel->data(sizeModel->index(i, 2)).toInt();
    // }
    // DEBUG_MATRIX(m_vectorB);

}

