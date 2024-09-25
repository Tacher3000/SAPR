#include "scalablegraphicsview.h"

ScalableGraphicsView::ScalableGraphicsView(QWidget *parent) : QGraphicsView(parent), m_isDragging(false) {}

void ScalableGraphicsView::wheelEvent(QWheelEvent *event){
    QPoint angle = event->angleDelta();
    if (!angle.isNull()) {
        qreal scaleFactor = (angle.y() > 0) ? SCALE_FACTOR : INVERSE_SCALE_FACTOR;
        scale(scaleFactor, scaleFactor);
    }
    event->accept();
}

//вращение колесиком
// void ScalableGraphicsView::wheelEvent(QWheelEvent *event){
//     QPoint angle = event->angleDelta();
//     if (!angle.isNull()) {
//         if (event->modifiers() & Qt::ControlModifier) {
//             qreal rotationAngle = (angle.y() > 0) ? 10 : -10;
//             rotate(rotationAngle);
//         } else {
//             qreal scaleFactor = (angle.y() > 0) ? SCALE_FACTOR : INVERSE_SCALE_FACTOR;
//             scale(scaleFactor, scaleFactor);
//         }
//     }
//     event->accept();
// }

void ScalableGraphicsView::mousePressEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        m_isDragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    event->accept();
}



void ScalableGraphicsView::mouseMoveEvent(QMouseEvent *event){
    if (m_isDragging) {
        QPoint delta = event->pos() - m_lastMousePos;

        if (event->modifiers() & Qt::ControlModifier) {
            qreal rotationAngle = -delta.x();
            rotate(rotationAngle);
        } else {
            if (horizontalScrollBar() && verticalScrollBar()) {
                horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
                verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
            }
        }

        m_lastMousePos = event->pos();
    }
    event->accept();
}

void ScalableGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
    event->accept();
}

void ScalableGraphicsView::resetView()
{
    resetTransform();

    if (horizontalScrollBar() && verticalScrollBar()) {
        horizontalScrollBar()->setValue(0);
        verticalScrollBar()->setValue(0);
    }
}
