#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTextEdit>
#include "core/circuit.h"
#include "gui/component_item.h"
#include "gui/wire_item.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool isWireModeEnabled() const { return wireMode; }
    bool isDrawingWire() const { return drawingWire; }
    void startWire(QPointF start);
    void finishWire(QPointF end);

private slots:
    void addResistor();
    void addCapacitor();
    void listCircuit();
    void toggleWireMode(bool enabled);

private:
    void createMenuBar();
    void createToolBar();
    void createDockWidgets();

    QGraphicsView *schematicView;
    QGraphicsScene *scene;
    QTextEdit *logConsole;
    Cathedral::Circuit circuit;
    int componentX = 0;
    bool wireMode = false;
    bool drawingWire = false;
    QPointF wireStartPoint;
    WireItem* currentWire = nullptr;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MAIN_WINDOW_H
