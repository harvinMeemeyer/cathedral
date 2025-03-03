#include "gui/main_window.h"
#include "gui/component_item.h"
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QGraphicsScene>
#include <QTextEdit>
#include <QAction>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), componentX(0) {
    setWindowTitle("Cathedral EDA");
    resize(1200, 800);

    createMenuBar();
    createToolBar();
    createDockWidgets();

    schematicView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QColor("#1E1E1E"));

    const int gridSize = 20;
    for (int x = -500; x <= 500; x += gridSize) {
        scene->addLine(x, -500, x, 500, QPen(QColor("#444444")));
    }
    for (int y = -500; y <= 500; y += gridSize) {
        scene->addLine(-500, y, 500, y, QPen(QColor("#444444")));
    }

    schematicView->setScene(scene);
    schematicView->setDragMode(QGraphicsView::RubberBandDrag);  // Default mode for dragging
    setCentralWidget(schematicView);
}

MainWindow::~MainWindow() {}

void MainWindow::createMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("&File");
    QAction *exitAction = new QAction("Exit", this);
    fileMenu->addAction(exitAction);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);

    QMenu *circuitMenu = menuBar->addMenu("&Circuit");
    QAction *addResistorAction = new QAction("Add Resistor", this);
    QAction *addCapacitorAction = new QAction("Add Capacitor", this);
    QAction *listCircuitAction = new QAction("List Components", this);

    circuitMenu->addAction(addResistorAction);
    circuitMenu->addAction(addCapacitorAction);
    circuitMenu->addAction(listCircuitAction);

    connect(addResistorAction, &QAction::triggered, this, &MainWindow::addResistor);
    connect(addCapacitorAction, &QAction::triggered, this, &MainWindow::addCapacitor);
    connect(listCircuitAction, &QAction::triggered, this, &MainWindow::listCircuit);
}

void MainWindow::createToolBar() {
    QToolBar *toolBar = addToolBar("Main Toolbar");

    QAction *addResistorAction = new QAction("➕ R", this);
    QAction *addCapacitorAction = new QAction("➕ C", this);
    QAction *toggleWireModeAction = new QAction("Wire Mode", this);
    toggleWireModeAction->setCheckable(true);

    toolBar->addAction(addResistorAction);
    toolBar->addAction(addCapacitorAction);
    toolBar->addAction(toggleWireModeAction);

    connect(addResistorAction, &QAction::triggered, this, &MainWindow::addResistor);
    connect(addCapacitorAction, &QAction::triggered, this, &MainWindow::addCapacitor);
    connect(toggleWireModeAction, &QAction::toggled, this, &MainWindow::toggleWireMode);
}

void MainWindow::createDockWidgets() {
    QDockWidget *logDock = new QDockWidget("Console", this);
    logConsole = new QTextEdit();
    logConsole->setReadOnly(true);
    logDock->setWidget(logConsole);
    addDockWidget(Qt::BottomDockWidgetArea, logDock);
}

void MainWindow::addResistor() {
    circuit.addComponent("Resistor", 1000, 1, 2);
    logConsole->append("Added Resistor (1kΩ) between nodes 1 and 2.");
    ComponentItem *resistor = new ComponentItem("Resistor", componentX, 0);
    scene->addItem(resistor);
    componentX += 50;
}

void MainWindow::addCapacitor() {
    circuit.addComponent("Capacitor", 0.01, 2, 3);
    logConsole->append("Added Capacitor (10nF) between nodes 2 and 3.");
    ComponentItem *capacitor = new ComponentItem("Capacitor", componentX, 0);
    scene->addItem(capacitor);
    componentX += 50;
}

void MainWindow::startWire(QPointF start) {
    if (!wireMode || drawingWire) return;
    drawingWire = true;
    wireStartPoint = start;
    currentWire = new WireItem(start, start);
    scene->addItem(currentWire);
    logConsole->append("Wire started at (" + QString::number(start.x()) + ", " + QString::number(start.y()) + ")");
}

void MainWindow::finishWire(QPointF end) {
    if (!wireMode || !drawingWire) return;
    currentWire->setEndPoint(end);
    drawingWire = false;
    currentWire = nullptr;
    logConsole->append("Wire completed at (" + QString::number(end.x()) + ", " + QString::number(end.y()) + ")");
}

void MainWindow::listCircuit() {
    logConsole->append("Listing all circuit components:");
    circuit.listComponents();
    QString visualList = "Visual Components:\n";
    for (auto *item : scene->items()) {
        if (auto *comp = dynamic_cast<ComponentItem*>(item)) {
            visualList += comp->getType() + " at (" + QString::number(comp->pos().x()) + ", " +
                          QString::number(comp->pos().y()) + ")\n";
        }
    }
    logConsole->append(visualList);
}

void MainWindow::toggleWireMode(bool enabled) {
    wireMode = enabled;
    if (wireMode) {
        schematicView->setDragMode(QGraphicsView::NoDrag);  // Disable dragging for wire mode
        logConsole->append("Wire Mode ON");
    } else {
        schematicView->setDragMode(QGraphicsView::RubberBandDrag);  // Restore dragging
        if (drawingWire && currentWire) {
            scene->removeItem(currentWire);
            delete currentWire;
            currentWire = nullptr;
            drawingWire = false;
        }
        logConsole->append("Wire Mode OFF");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (wireMode && !drawingWire) {
        QPointF startPos = schematicView->mapToScene(event->pos());
        startWire(startPos);
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (wireMode && drawingWire && currentWire) {
        QPointF currentPos = schematicView->mapToScene(event->pos());
        currentWire->setEndPoint(currentPos);
    }
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (wireMode && drawingWire) {
        QPointF endPos = schematicView->mapToScene(event->pos());
        finishWire(endPos);
    }
    QMainWindow::mouseReleaseEvent(event);
}
