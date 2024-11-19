#ifndef DOUBLEDELEGATE_H
#define DOUBLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QDoubleValidator>

class DoubleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DoubleDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // DOUBLEDELEGATE_H
