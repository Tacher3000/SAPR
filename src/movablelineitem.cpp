#include "movablelineitem.h"

MovableLineItem::MovableLineItem(qreal x, qreal y, qreal height, qreal minX,
                                 qreal maxX, QGraphicsItem *parent)
    : QGraphicsLineItem(x, y, x, height, parent), m_minX(minX), m_maxX(maxX) {
    setPen(QPen(Qt::blue, 2));
    setFlags(QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemSendsScenePositionChanges);
}

void MovableLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    qreal newX = qBound(m_minX, event->scenePos().x(), m_maxX);
    setPos(newX, 0);
}

qreal MovableLineItem::getMaxX() const
{
    return m_maxX;
}

qreal MovableLineItem::getMinX() const
{
    return m_minX;
}


