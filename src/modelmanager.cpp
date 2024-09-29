#include "modelmanager.h"

ModelManager::ModelManager(QObject *parent)
    : QObject(parent),
    m_sizeModel(new SizeTableModel),
    m_nodeModel(new NodeModel) {}

void ModelManager::initializeModels(QTableView *sizeTableView, QTableView *nodeTableView) {
    sizeTableView->setModel(m_sizeModel);
    sizeTableView->resizeRowsToContents();
    sizeTableView->resizeColumnsToContents();

    nodeTableView->setModel(m_nodeModel);
    nodeTableView->resizeRowsToContents();
    nodeTableView->resizeColumnsToContents();

    connect(m_sizeModel, &SizeTableModel::dataChanged, this, &ModelManager::updateNodeModel);
    connect(m_nodeModel, &NodeModel::dataChanged, this, &ModelManager::nodeModelUpdated);
}

void ModelManager::updateNodeModel() {
    m_nodeModel->setRowCount(m_sizeModel->rowCount());
    emit nodeModelUpdated();
}

void ModelManager::saveModels(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);

        int sizeRowCount = m_sizeModel->rowCount();
        int sizeColCount = m_sizeModel->columnCount();
        out << sizeRowCount << sizeColCount;
        for (int row = 0; row < sizeRowCount; ++row) {
            for (int col = 0; col < sizeColCount; ++col) {
                QVariant data = m_sizeModel->data(m_sizeModel->index(row, col));
                out << data;
            }
        }

        int nodeRowCount = m_nodeModel->rowCount();
        int nodeColCount = m_nodeModel->columnCount();
        out << nodeRowCount << nodeColCount;
        for (int row = 0; row < nodeRowCount; ++row) {
            for (int col = 0; col < nodeColCount; ++col) {
                QVariant data = m_nodeModel->data(m_nodeModel->index(row, col));
                out << data;
            }
        }

        file.close();
    }
}

void ModelManager::loadModels(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        m_nodeModel->clearData();
        m_sizeModel->clearData();

        QDataStream in(&file);

        int sizeRowCount, sizeColCount;
        in >> sizeRowCount >> sizeColCount;
        m_sizeModel->setRowCount(sizeRowCount);
        m_sizeModel->setColumnCount(sizeColCount);
        for (int row = 0; row < sizeRowCount; ++row) {
            for (int col = 0; col < sizeColCount; ++col) {
                QVariant data;
                in >> data;
                m_sizeModel->setData(m_sizeModel->index(row, col), data);
            }
        }

        int nodeRowCount, nodeColCount;
        in >> nodeRowCount >> nodeColCount;
        m_nodeModel->setRowCount(nodeRowCount);
        m_nodeModel->setColumnCount(nodeColCount);
        for (int row = 0; row < nodeRowCount; ++row) {
            for (int col = 0; col < nodeColCount; ++col) {
                QVariant data;
                in >> data;
                m_nodeModel->setData(m_nodeModel->index(row, col), data);
            }
        }

        file.close();
    }
}

void ModelManager::clearModels() {
    m_nodeModel->clearData();
    m_sizeModel->clearData();
}
