#include "nodemodel.h"

NodeModel::NodeModel(QObject *pobj)
    : QAbstractTableModel(pobj), m_nRows(1), m_nColumns(2) {
}

QVariant NodeModel::data(const QModelIndex& index, int nRole) const
{
    if (!index.isValid() || index.row() >= m_nRows) {
        return QVariant();
    }

    return (nRole == Qt::DisplayRole || nRole == Qt::EditRole)
               ? m_hash.value(index, QVariant(""))
               : QVariant();
}

bool NodeModel::setData(const QModelIndex& index, const QVariant& value, int nRole)
{
    if (index.isValid() && nRole == Qt::EditRole) {

        m_hash[index] = value;
        emit dataChanged(index, index);

        return true;
    }
    return false;
}


int NodeModel::rowCount(const QModelIndex & /*parent*/) const {
    return m_nRows;
}

int NodeModel::columnCount(const QModelIndex & /*parent*/) const {
    return m_nColumns;
}

Qt::ItemFlags NodeModel::flags(const QModelIndex &index) const {
    return index.isValid() ? (QAbstractTableModel::flags(index) | Qt::ItemIsEditable) : QAbstractTableModel::flags(index);
}

QVariant NodeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                return "Сосредоточенная продольная нагрузка";
            case 1:
                return "Опора";
            default:
                return QVariant();
            }
        } else if (orientation == Qt::Vertical) {
            return section + 1;
        }
    }
    return QVariant();
}

void NodeModel::setRowCount(int nRows) {
    if (nRows != m_nRows) {
        beginResetModel();
        m_nRows = nRows;
        endResetModel();
    }
}

void NodeModel::setColumnCount(int nColumns) {
    if (nColumns != m_nColumns) {
        beginResetModel();
        m_nColumns = nColumns;
        endResetModel();
    }
}

void NodeModel::clearData() {
    beginResetModel();
    m_hash.clear();
    m_nRows = 1;
    m_nColumns = 2;
    endResetModel();
}

bool NodeModel::isEmpty() const {
    for (const auto &value : m_hash) {
        if (!value.toString().isEmpty()) {
            return false;
        }
    }
    return true;
}
