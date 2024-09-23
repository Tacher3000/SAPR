#ifndef NODEMODEL_H
#define NODEMODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include <QVariant>

class NodeModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    NodeModel(QObject *pobj = nullptr);
    // Функции, переопределенные из QAbstractTableModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int nRole = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int nRole = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setRowCount(int nColumns);
private:
    int m_nRows;
    int m_nColumns;
    QHash<QModelIndex, QVariant> m_hash;
};

#endif
