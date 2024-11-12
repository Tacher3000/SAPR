#include "positiveNumericDelegate.h"

PositiveNumericDelegate::PositiveNumericDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QWidget *PositiveNumericDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    QIntValidator *validator = new QIntValidator(1, INT_MAX, editor);
    editor->setValidator(validator);

    connect(editor, &QLineEdit::textChanged, [editor, validator](const QString &text) {
        if (text.isEmpty()) {
            editor->setValidator(nullptr);
        } else {
            editor->setValidator(validator);
        }
    });

    return editor;
}

void PositiveNumericDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem itemOption(option);

    if (itemOption.state & QStyle::State_HasFocus) {
        itemOption.state &= ~QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, itemOption, index);
}
