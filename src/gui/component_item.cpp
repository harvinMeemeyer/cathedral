#include "gui/component_item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "gui/main_window.h"

ComponentItem::ComponentItem(const QString& type, int x, int y)
    : componentType(type), isDragging(false) {
    setPos(x, y);
}

QRectF ComponentItem::boundingRect() const {
    return QRectF(-15, -10, 30, 20);
}

void ComponentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setRenderHint(QPainter::Antialiasing);  // ✅ Enable smooth edges

    // **Set component colors**
    QColor fillColor, borderColor;
    if (componentType == "Resistor") {
        fillColor = QColor("#FFA500");  // Bright Orange
        borderColor = QColor("#FF8C00");  // Slightly darker orange
    } else if (componentType == "Capacitor") {
        fillColor = QColor("#00AFFF");  // Electric Blue
        borderColor = QColor("#0088CC");  // Darker Blue
    } else {
        fillColor = Qt::white;  // Default
        borderColor = Qt::black;
    }

    painter->setBrush(fillColor);
    painter->setPen(QPen(borderColor, 2));  // ✅ Thicker border for visibility

    // **Draw shapes**
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

    // **Selection Glow**
    if (isSelected()) {
        painter->setPen(QPen(QColor("#00FFFF"), 3));  // Cyan glow effect
        painter->drawRect(boundingRect());
    }
}

// **Mouse Press - Start dragging or start wire**
void ComponentItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        MainWindow* mainWin = qobject_cast<MainWindow*>(scene()->views().first()->parentWidget());
        if (!mainWin) return;

        if (mainWin->isWireModeEnabled()) {  // ✅ Use getter function instead of accessing private member
            if (mainWin->isDrawingWire()) {
                mainWin->finishWire(scenePos());
            } else {
                mainWin->startWire(scenePos());
            }
        }
    }
}

// **Mouse Release - Stop dragging**
void ComponentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    isDragging = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void ComponentItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isDragging) {
        setPos(event->scenePos());
    }
    QGraphicsItem::mouseMoveEvent(event);
}
