#include "movablelineview.h"

void MovableLineView::setLine(MovableLineItem *line)
{
    m_movableLine = line;
}

void MovableLineView::wheelEvent(QWheelEvent *event){
    QPoint angle = event->angleDelta();
    if (!angle.isNull()) {
        qreal scaleFactor = (angle.y() > 0) ? SCALE_FACTOR_IN_MOVABLELINEVIEW : INVERSE_SCALE_FACTOR_IN_MOVABLELINEVIEW;
        scale(scaleFactor, scaleFactor);
    }
    event->accept();
}

void MovableLineView::setLinePosition(QString x) {
    double boundedX =
        qBound(m_movableLine->getMinX(), x.toDouble(), m_movableLine->getMaxX());
    m_movableLine->setPos(boundedX * RECT_WIDTH_MULTIPLIER, 0);
}

void MovableLineView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        moveLineToMouse(event->pos());
    }
}

void MovableLineView::mouseMoveEvent(QMouseEvent *event) {
    if (m_dragging) {
        moveLineToMouse(event->pos());
    }
}

void MovableLineView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
    }
}

void MovableLineView::moveLineToMouse(const QPoint &mousePos) {
    QPointF scenePos = mapToScene(mousePos);

    qreal newX =
        qBound(m_movableLine->getMinX(), scenePos.x(), m_movableLine->getMaxX());
    m_movableLine->setPos(newX, 0);
    emit lineMoved(QString::number(newX / RECT_WIDTH_MULTIPLIER));
}
