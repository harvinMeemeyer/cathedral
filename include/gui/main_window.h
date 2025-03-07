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
    bool isDeleteModeEnabled() const { return deleteMode; }  // Add for Delete Mode
    void startWire(QPointF start);
    void finishWire(QPointF end);

private slots:
    void addResistor();
    void addCapacitor();
    void listCircuit();
    void toggleWireMode(bool enabled);
    void toggleDeleteMode(bool enabled);  // Add slot for Delete Mode
    void onComponentMoved(ComponentItem *component);

private:
    void createMenuBar();
    void createToolBar();
    void createDockWidgets();
    void deleteComponent(ComponentItem *component);  // Helper to delete a component
    void deleteWire(WireItem *wire);  // Helper to delete a wire

    QGraphicsView *schematicView;
    QGraphicsScene *scene;
    QTextEdit *logConsole;
    Cathedral::Circuit circuit;
    int componentX = 0;
    bool wireMode = false;
    bool drawingWire = false;
    bool deleteMode = false;  // Add Delete Mode state
    QPointF wireStartPoint;
    WireItem* currentWire = nullptr;
    QAction *toggleWireModeAction = nullptr;
    QAction *toggleDeleteModeAction = nullptr;  // Add for Delete Mode
    QAction *addResistorAction = nullptr;
    QAction *addCapacitorAction = nullptr;

    struct WireConnection {
        ComponentItem *startComponent;
        QPointF startTerminal;
        ComponentItem *endComponent;
        QPointF endTerminal;
        QList<WireItem*> segments;
    };
    QList<WireConnection> wireConnections;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAIN_WINDOW_H
