#include "sizetablemodel.h"

SizeTableModel::SizeTableModel(QObject *pobj)
    : QAbstractTableModel(pobj), m_nRows(1), m_nColumns(4), m_maxSection(0.0) {
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

        if (index.column() == 1) { // Столбец "Сечение"
            double sectionValue = value.toDouble();
            if (sectionValue > m_maxSection) {
                m_maxSection = sectionValue;
            }
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
            case 3:
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

double SizeTableModel::getMaxSection() const {
    return m_maxSection;
}

