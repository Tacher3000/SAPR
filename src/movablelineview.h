#ifndef MOVABLELINEVIEW_H
#define MOVABLELINEVIEW_H

#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include "movablelineitem.h"
#include "scenedrawer.h"


class MovableLineView : public QGraphicsView {
    Q_OBJECT
public:
    explicit MovableLineView(MovableLineItem *line, QWidget *parent = nullptr)
        : QGraphicsView(parent), m_movableLine(line) {}

    void setLine(MovableLineItem *line);

    void setLinePosition(QString x) {
        double boundedX = qBound(m_movableLine->getMinX(), x.toDouble(), m_movableLine->getMaxX());
        m_movableLine->setPos(boundedX * RECT_WIDTH_MULTIPLIER, 0);
    }

signals:
    void lineMoved(QString x);

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            m_dragging = true;
            moveLineToMouse(event->pos());
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (m_dragging) {
            moveLineToMouse(event->pos());
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            m_dragging = false;
        }
    }

private:
    void moveLineToMouse(const QPoint &mousePos) {
        QPointF scenePos = mapToScene(mousePos);

        qreal newX = qBound(m_movableLine->getMinX(), scenePos.x(), m_movableLine->getMaxX());
        m_movableLine->setPos(newX, 0);
        emit lineMoved(QString::number(newX / RECT_WIDTH_MULTIPLIER));
    }

    MovableLineItem *m_movableLine;
    bool m_dragging;
};

#endif // MOVABLELINEVIEW_H
