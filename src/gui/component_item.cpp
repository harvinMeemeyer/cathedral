#include "gui/component_item.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include "gui/main_window.h"
#include <QDebug>
#include <cmath>

ComponentItem::ComponentItem(const QString& type, int x, int y)
    : QObject(nullptr),
      QGraphicsItem(),
      componentType(type),
      isDragging(false) {
    setPos(x, y);
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    initializeTerminals();  // Set up terminals
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
        painter->drawLine(-15, 0, -10, 0);  // Left terminal line
        painter->drawLine(10, 0, 15, 0);   // Right terminal line
    } else if (componentType == "Capacitor") {
        painter->drawLine(-10, -5, -10, 5);  // Left plate
        painter->drawLine(10, -5, 10, 5);    // Right plate
        painter->drawLine(-15, 0, -10, 0);   // Left terminal line
        painter->drawLine(10, 0, 15, 0);     // Right terminal line
    }

    // Draw terminal markers (for visualization)
    painter->setPen(QPen(Qt::red, 2));
    for (const QPointF& terminal : terminals) {
        painter->drawEllipse(terminal.x() - 2, terminal.y() - 2, 4, 4);
    }

    if (isSelected()) {
        painter->setPen(QPen(QColor("#00FFFF"), 3));
        painter->drawRect(boundingRect());
    }
}

void ComponentItem::initializeTerminals() {
    terminals.clear();
    if (componentType == "Resistor" || componentType == "Capacitor") {
        // Add terminals at left and right sides (relative to center)
        terminals.append(QPointF(-15, 0));  // Left terminal
        terminals.append(QPointF(15, 0));   // Right terminal
    }
}

QPointF ComponentItem::getNearestTerminal(const QPointF& clickPos) const {
    QPointF localClick = mapFromScene(clickPos);
    QPointF nearestTerminal = terminals.isEmpty() ? QPointF(0, 0) : terminals.first();
    qreal minDistance = std::numeric_limits<qreal>::max();

    for (const QPointF& terminal : terminals) {
        qreal distance = QLineF(localClick, terminal).length();
        if (distance < minDistance) {
            minDistance = distance;
            nearestTerminal = terminal;
        }
    }
    return mapToScene(nearestTerminal);
}

void ComponentItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    MainWindow* mainWin = qobject_cast<MainWindow*>(scene()->views().first()->parentWidget());
    if (!mainWin) return;

    if (mainWin->isWireModeEnabled()) {
        qDebug() << "Wire mode active, handling wire at" << scenePos();
        if (mainWin->isDrawingWire()) {
            mainWin->finishWire(getNearestTerminal(event->scenePos()));
        } else {
            mainWin->startWire(getNearestTerminal(event->scenePos()));
        }
        event->accept();
    } else {
        qDebug() << "Starting drag on" << componentType << "at" << scenePos();
        isDragging = true;
        lastPosition = event->scenePos();
        QGraphicsItem::mousePressEvent(event);
    }
}

void ComponentItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    MainWindow* mainWin = qobject_cast<MainWindow*>(scene()->views().first()->parentWidget());
    if (!mainWin || mainWin->isWireModeEnabled()) {
        qDebug() << "Wire mode active, ignoring drag on" << componentType;
        event->accept();
        return;
    }

    if (isDragging) {
        QPointF delta = event->scenePos() - lastPosition;
        setPos(pos() + delta);
        lastPosition = event->scenePos();
        qDebug() << "Dragging" << componentType << "to" << pos();
        emit positionChanged(this);
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void ComponentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    isDragging = false;
    update();
    emit positionChanged(this);
    QGraphicsItem::mouseReleaseEvent(event);
}
