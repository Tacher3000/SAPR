#ifndef NUMERICDELEGATE_H
#define NUMERICDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QLineEdit>
#include <QIntValidator>

class NumericDelegate : public QStyledItemDelegate  {
public:
    NumericDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // NUMERICDELEGATE_H
