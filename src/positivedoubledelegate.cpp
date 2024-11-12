#include "positivedoubledelegate.h"

PositiveDoubleDelegate::PositiveDoubleDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QWidget *PositiveDoubleDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    QDoubleValidator *validator = new QDoubleValidator(0.0, std::numeric_limits<double>::max(), 4, editor);
    validator->setNotation(QDoubleValidator::StandardNotation);
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

void PositiveDoubleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem itemOption(option);

    if (itemOption.state & QStyle::State_HasFocus) {
        itemOption.state &= ~QStyle::State_HasFocus;
    }

    QStyledItemDelegate::paint(painter, itemOption, index);
}
