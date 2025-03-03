#ifndef WIRE_ITEM_H
#define WIRE_ITEM_H

#include <QGraphicsLineItem>

class WireItem : public QGraphicsLineItem {
public:
    WireItem(QPointF start, QPointF end);
    void setEndPoint(QPointF end);
};

#endif // WIRE_ITEM_H
