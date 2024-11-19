#ifndef ROTATINGPIXMAPITEM_H
#define ROTATINGPIXMAPITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>

class RotatingPixmapItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    RotatingPixmapItem(const QPixmap& pixmap)
        : QGraphicsPixmapItem(pixmap), m_rotation(0) {}

    qreal rotation() const { return m_rotation; }
    void setRotation(qreal angle) {
        if (m_rotation != angle) {
            m_rotation = angle;
            QGraphicsPixmapItem::setRotation(angle);
            emit rotationChanged();
        }
    }

signals:
    void rotationChanged();

private:
    qreal m_rotation;
};

#endif // ROTATINGPIXMAPITEM_H
