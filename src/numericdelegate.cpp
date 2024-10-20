#include "numericdelegate.h"

NumericDelegate::NumericDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QWidget *NumericDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    QIntValidator *validator = new QIntValidator(editor);
    validator->setBottom(INT_MIN);
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
void NumericDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem itemOption(option);

    if (itemOption.state & QStyle::State_HasFocus) {
        itemOption.state &= ~QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, itemOption, index);
}

