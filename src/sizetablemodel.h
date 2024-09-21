#ifndef SIZETABLEMODEL_H
#define SIZETABLEMODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include <QVariant>

class SizeTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    // Конструктор
    SizeTableModel(QObject *pobj = nullptr);

    // Функции, переопределенные из QAbstractTableModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int nRole = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int nRole = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // bool removeRows(int row, int count, const QModelIndex &parent) override;
    // bool removeRow(int row, const QModelIndex &parent);
    // bool insertRows(int row, int count, const QModelIndex &parent) override;
    double getMaxSection() const;
private:
    int m_nRows;
    int m_nColumns;
    double m_maxSection;
    QHash<QModelIndex, QVariant> m_hash;
};

#endif // SIZETABLEMODEL_H

