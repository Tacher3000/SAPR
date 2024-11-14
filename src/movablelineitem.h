#ifndef MOVABLELINEITEM_H
#define MOVABLELINEITEM_H

#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>

class MovableLineItem : public QGraphicsLineItem {
public:
    MovableLineItem(qreal x, qreal y, qreal height, qreal minX, qreal maxX,
                    QGraphicsItem *parent = nullptr);

    qreal getMinX() const;

    qreal getMaxX() const;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
private:
    qreal m_minX;
    qreal m_maxX;
};


#endif // MOVABLELINEITEM_H
