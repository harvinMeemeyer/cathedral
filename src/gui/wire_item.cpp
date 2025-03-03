#include "gui/wire_item.h"
#include <QPen>

WireItem::WireItem(QPointF start, QPointF end) {
    setLine(QLineF(start, end));

    // Style the wire
    QPen wirePen(QColor("#00FF00"));  // Neon green
    wirePen.setWidth(2);
    setPen(wirePen);
}

void WireItem::setEndPoint(QPointF end) {
    prepareGeometryChange();
    setLine(QLineF(line().p1(), end));  // ✅ Correct way to update the line
}
