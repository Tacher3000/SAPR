#ifndef SIZETABLEMODEL_H
#define SIZETABLEMODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include <QVariant>

class SizeTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    SizeTableModel(QObject *pobj = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int nRole = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int nRole = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    double getMaxSection() const;
    void setRowCount(int nRows);
    void setColumnCount(int nColumns);
    void clearData();
    bool removeRows(int position, int rows, const QModelIndex &parent) override;
    bool isEmpty() const;
private:
    int m_nRows;
    int m_nColumns;
    QHash<QModelIndex, QVariant> m_hash;
};

#endif // SIZETABLEMODEL_H

