#ifndef POSITIVENUMERICDELEGATE_H
#define POSITIVENUMERICDELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QIntValidator>

class PositiveNumericDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit PositiveNumericDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // POSITIVENUMERICDELEGATE_H
