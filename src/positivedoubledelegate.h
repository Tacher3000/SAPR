#ifndef POSITIVEDOUBLEDELEGATE_H
#define POSITIVEDOUBLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QDoubleValidator>

class PositiveDoubleDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit PositiveDoubleDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // POSITIVEDOUBLEDELEGATE_H
