#include "bouncingtext.h"

BouncingText::BouncingText(const QString &text, QGraphicsItem *parent)
    : QGraphicsTextItem(text, parent), dx(4), dy(4) {
    setDefaultTextColor(Qt::red);
}
void BouncingText::advance(int step) {
    if (!step)
        return;
    moveBy(dx, dy);

    if (x() <= 0 || (x() + boundingRect().width()) >= scene()->width()) {
        dx = -dx;
    }
    if (y() <= 0 || (y() + boundingRect().height()) >= scene()->height()) {
        dy = -dy;
    }

    QList<QGraphicsItem*> collidingItems = scene()->collidingItems(this);
    for (QGraphicsItem* item : collidingItems) {
        if (item != this) {
            dx = -dx;
            dy = -dy;
        }
    }
}
