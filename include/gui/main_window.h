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

    bool isWireModeEnabled() const { return wireMode; }  // ✅ Getter for wireMode
    bool isDrawingWire() const { return drawingWire; }   // ✅ Getter for drawingWire

    void startWire(QPointF start);
    void finishWire(QPointF end);

private slots:
    void addResistor();
    void addCapacitor();
    void listCircuit();

private:
    void createMenuBar();
    void createToolBar();
    void createDockWidgets();

    QGraphicsView *schematicView;
    QGraphicsScene *scene;
    QTextEdit *logConsole;
    Cathedral::Circuit circuit;
    int componentX;  // Track position of new components
    bool wireMode = false;  // ✅ Tracks if wire mode is on
    bool drawingWire = false;
    QPointF wireStartPoint;
    WireItem* currentWire = nullptr;

    void toggleWireMode(bool enabled);
};

#endif // MAIN_WINDOW_H
