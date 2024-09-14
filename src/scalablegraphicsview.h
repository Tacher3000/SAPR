#ifndef SCALABLEGRAPHICSVIEW_H
#define SCALABLEGRAPHICSVIEW_H

#include "qevent.h"
#include <QGraphicsView>
#include <QWheelEvent>
#include <QScrollBar>
#include <QMouseEvent>

constexpr qreal SCALE_FACTOR = 1.2;
constexpr qreal INVERSE_SCALE_FACTOR = 1.0 / SCALE_FACTOR;

class ScalableGraphicsView : public QGraphicsView
{
public:
    ScalableGraphicsView(QWidget *parent = nullptr);

protected:
    // Обработка событий колеса мыши для изменения масштаба
    void wheelEvent(QWheelEvent *event) override;
    // Обработка событий нажатия кнопки мыши для начала перемещения сцены
    void mousePressEvent(QMouseEvent *event) override;
    // Обработка событий перемещения мыши для изменения положения сцены
    void mouseMoveEvent(QMouseEvent *event) override;
    // Обработка событий отпускания кнопки мыши для завершения перемещения сцены
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    bool m_isDragging;
    QPoint m_lastMousePos;
};

#endif // SCALABLEGRAPHICSVIEW_H
