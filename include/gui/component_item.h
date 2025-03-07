#ifndef COMPONENT_ITEM_H
#define COMPONENT_ITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QString>
#include <QList>
#include <QPointF>

class ComponentItem : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    ComponentItem(const QString& type, int x, int y);
    QString getType() const { return componentType; }
    QPointF getNearestTerminal(const QPointF& clickPos) const;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

signals:
    void positionChanged(ComponentItem *component);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString componentType;
    bool isDragging;
    QPointF lastPosition;
    QList<QPointF> terminals;  // List of terminal positions relative to component center
    void initializeTerminals();  // Initialize terminal positions based on component type
};

#endif // COMPONENT_ITEM_H
