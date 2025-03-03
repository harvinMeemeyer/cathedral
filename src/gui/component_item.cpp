#include "gui/component_item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "gui/main_window.h"

ComponentItem::ComponentItem(const QString& type, int x, int y)
    : componentType(type), isDragging(false) {
    setPos(x, y);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);  // Enable dragging
}

QRectF ComponentItem::boundingRect() const {
    return QRectF(-15, -10, 30, 20);
}

void ComponentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);

    QColor fillColor, borderColor;
    if (componentType == "Resistor") {
        fillColor = QColor("#FFA500");
        borderColor = QColor("#FF8C00");
    } else if (componentType == "Capacitor") {
        fillColor = QColor("#00AFFF");
        borderColor = QColor("#0088CC");
    } else {
        fillColor = Qt::white;
        borderColor = Qt::black;
    }

    painter->setBrush(fillColor);
    painter->setPen(QPen(borderColor, 2));

    if (componentType == "Resistor") {
        painter->drawRect(-10, -5, 20, 10);
        painter->drawLine(-15, 0, -10, 0);
        painter->drawLine(10, 0, 15, 0);
    } else if (componentType == "Capacitor") {
        painter->drawLine(-10, -5, -10, 5);
        painter->drawLine(10, -5, 10, 5);
        painter->drawLine(-15, 0, -10, 0);
        painter->drawLine(10, 0, 15, 0);
    }

    if (isSelected()) {
        painter->setPen(QPen(QColor("#00FFFF"), 3));
        painter->drawRect(boundingRect());
    }
}

void ComponentItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    MainWindow* mainWin = qobject_cast<MainWindow*>(scene()->views().first()->parentWidget());
    if (!mainWin) return;

    if (mainWin->isWireModeEnabled()) {
        if (mainWin->isDrawingWire()) {
            mainWin->finishWire(scenePos());
        } else {
            mainWin->startWire(scenePos());
        }
    } else {
        isDragging = true;  // Start dragging only if not in wire mode
        lastPosition = event->scenePos();
    }
    QGraphicsItem::mousePressEvent(event);
}

void ComponentItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (!isDragging || qobject_cast<MainWindow*>(scene()->views().first()->parentWidget())->isWireModeEnabled()) {
        QGraphicsItem::mouseMoveEvent(event);  // Let Qt handle moving if ItemIsMovable
        return;
    }
    QPointF delta = event->scenePos() - lastPosition;
    setPos(pos() + delta);
    lastPosition = event->scenePos();
    QGraphicsItem::mouseMoveEvent(event);
}

void ComponentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    isDragging = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
