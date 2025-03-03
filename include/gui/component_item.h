#ifndef COMPONENT_ITEM_H
#define COMPONENT_ITEM_H

#include <QGraphicsItem>
#include <QString>

class ComponentItem : public QGraphicsItem {
public:
    ComponentItem(const QString& type, int x, int y);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString componentType;
    bool isDragging;
    QPointF lastPosition;
};

#endif // COMPONENT_ITEM_H
