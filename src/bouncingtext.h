#ifndef BOUNCINGTEXT_H
#define BOUNCINGTEXT_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QRandomGenerator>

class BouncingText : public QGraphicsTextItem {
public:
    BouncingText(const QString &text, QGraphicsItem *parent = nullptr);

    void advance(int step) override;

private:
    qreal dx;
    qreal dy;
};

#endif // BOUNCINGTEXT_H
