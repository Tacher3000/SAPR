#ifndef MOVABLELINEVIEW_H
#define MOVABLELINEVIEW_H

#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include "movablelineitem.h"
#include "scenedrawer.h"

constexpr qreal SCALE_FACTOR_IN_MOVABLELINEVIEW = 1.2;
constexpr qreal INVERSE_SCALE_FACTOR_IN_MOVABLELINEVIEW = 1.0 / SCALE_FACTOR_IN_MOVABLELINEVIEW;

class MovableLineView : public QGraphicsView {
    Q_OBJECT
public:
    explicit MovableLineView(MovableLineItem *line, QWidget *parent = nullptr)
        : QGraphicsView(parent), m_movableLine(line) {}

    void setLine(MovableLineItem *line);

    void setLinePosition(QString x);

signals:
    void lineMoved(QString x);

protected:
    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void moveLineToMouse(const QPoint &mousePos);

    MovableLineItem *m_movableLine;
    bool m_dragging;
};

#endif // MOVABLELINEVIEW_H
