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
        if (index.row() == m_nRows - 1) {
            beginInsertRows(QModelIndex(), m_nRows - 1, m_nRows - 1);
            ++m_nRows;
            endInsertRows();
        }

        m_hash[index] = value;
        emit dataChanged(index, index);

        return true;
    }
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



