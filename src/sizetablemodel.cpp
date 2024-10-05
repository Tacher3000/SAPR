#include "sizetablemodel.h"

SizeTableModel::SizeTableModel(QObject *pobj)
    : QAbstractTableModel(pobj), m_nRows(1), m_nColumns(3) {
}

QVariant SizeTableModel::data(const QModelIndex& index, int nRole) const
{
    if (!index.isValid() || index.row() >= m_nRows) {
        return QVariant();
    }

    return (nRole == Qt::DisplayRole || nRole == Qt::EditRole)
               ? m_hash.value(index, QVariant(""))
               : QVariant();
}


bool SizeTableModel::setData(const QModelIndex& index, const QVariant& value, int nRole)
{
    if (index.isValid() && nRole == Qt::EditRole) {
        QVariant oldValue = m_hash.value(index, QVariant(""));

        m_hash[index] = value;

        if (index.row() == m_nRows - 1 && !value.toString().isEmpty() && oldValue.toString().isEmpty()) {
            beginInsertRows(QModelIndex(), m_nRows, m_nRows);
            ++m_nRows;
            endInsertRows();
        }

        if (index.row() == m_nRows - 2 && value.toString().isEmpty()) {
            bool isRowEmpty = true;
            for (int col = 0; col < m_nColumns; ++col) {
                QModelIndex checkIndex = this->index(index.row(), col);
                if (!m_hash.value(checkIndex, QVariant("")).toString().isEmpty()) {
                    isRowEmpty = false;
                    break;
                }
            }

            if (isRowEmpty) {
                beginRemoveRows(QModelIndex(), m_nRows - 1, m_nRows - 1);
                --m_nRows;
                endRemoveRows();
            }
        }
        emit dataChanged(index, index);

        return true;
    }
    return false;
}



int SizeTableModel::rowCount(const QModelIndex & /*parent*/) const {
    return m_nRows;
}

int SizeTableModel::columnCount(const QModelIndex & /*parent*/) const {
    return m_nColumns;
}

Qt::ItemFlags SizeTableModel::flags(const QModelIndex &index) const {
    return index.isValid() ? (QAbstractTableModel::flags(index) | Qt::ItemIsEditable) : QAbstractTableModel::flags(index);
}

QVariant SizeTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
            case 0:
                return "Длина";
            case 1:
                return "Сечение";
            case 2:
                return "Распределенная продольная нагрузка";
            default:
                return QVariant();
            }
        } else if (orientation == Qt::Vertical) {
            return section + 1;
        }
    }
    return QVariant();
}

double SizeTableModel::getMaxSection() const {
    double maxSection = 0.0;

    for (int row = 0; row < m_nRows; ++row) {
        QModelIndex index = this->index(row, 1);
        QVariant data = this->data(index, Qt::DisplayRole);

        bool ok;
        double sectionValue = data.toDouble(&ok);
        if (ok && sectionValue > maxSection) {
            maxSection = sectionValue;
        }
    }

    return maxSection;
}

void SizeTableModel::setRowCount(int nRows) {
    if (nRows != m_nRows) {
        beginResetModel();
        m_nRows = nRows;
        endResetModel();
    }
}

void SizeTableModel::setColumnCount(int nColumns) {
    if (nColumns != m_nColumns) {
        beginResetModel();
        m_nColumns = nColumns;
        endResetModel();
    }
}

void SizeTableModel::clearData() {
    beginResetModel();
    m_hash.clear();
    m_nRows = 1;
    m_nColumns = 3;
    endResetModel();
}

bool SizeTableModel::removeRows(int position, int rows, const QModelIndex &parent) {
    if (position < 0 || position >= m_nRows || rows <= 0 || (position + rows) > m_nRows) {
        return false;
    }

    beginRemoveRows(parent, position, position + rows - 1);
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < m_nColumns; ++col) {
            QModelIndex index = this->index(position + row, col);
            m_hash.remove(index);
        }
    }
    m_nRows -= rows;
    endRemoveRows();
    return true;
}

bool SizeTableModel::isEmpty() const {
    for (const auto &value : m_hash) {
        if (!value.toString().isEmpty()) {
            return false;
        }
    }
    return true;
}
