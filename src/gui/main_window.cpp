#include "gui/main_window.h"
#include "gui/component_item.h"
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QGraphicsScene>
#include <QTextEdit>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), componentX(0) {  // Start placing components at x=0
    setWindowTitle("Cathedral EDA");
    resize(1200, 800);

    createMenuBar();
    createToolBar();
    createDockWidgets();

    // **Set up the schematic editor**
    schematicView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QColor("1E1E1E"));

    const int gridSize = 20;
    for (int x = -500; x <= 500; x += gridSize) {
        scene->addLine(x, -500, x, 500, QPen(QColor("#444444")));  // Vertical grid lines
    }
    for (int y = -500; y <= 500; y += gridSize) {
        scene->addLine(-500, y, 500, y, QPen(QColor("#444444")));  // Horizontal grid lines
    }

    schematicView->setScene(scene);
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

    toolBar->addAction(addResistorAction);
    toolBar->addAction(addCapacitorAction);

    connect(addResistorAction, &QAction::triggered, this, &MainWindow::addResistor);
    connect(addCapacitorAction, &QAction::triggered, this, &MainWindow::addCapacitor);
    QAction *toggleWireModeAction = new QAction("Wire Mode", this);
    toggleWireModeAction->setCheckable(true);  // ✅ Make it a toggle button
    connect(toggleWireModeAction, &QAction::toggled, this, &MainWindow::toggleWireMode);

    toolBar->addAction(toggleWireModeAction);
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

    // **Place the resistor visually**
    ComponentItem *resistor = new ComponentItem("Resistor", componentX, 0);
    scene->addItem(resistor);
    componentX += 50;  // Move placement position for next component
}

void MainWindow::addCapacitor() {
    circuit.addComponent("Capacitor", 0.01, 2, 3);
    logConsole->append("Added Capacitor (10nF) between nodes 2 and 3.");

    // **Place the capacitor visually**
    ComponentItem *capacitor = new ComponentItem("Capacitor", componentX, 0);
    scene->addItem(capacitor);
    componentX += 50;  // Move placement position for next component
}
void MainWindow::startWire(QPointF start) {
    if (!wireMode || drawingWire) return;  // ✅ Only allow in Wire Mode

    drawingWire = true;
    wireStartPoint = start;
    currentWire = new WireItem(start, start);
    scene->addItem(currentWire);
}

void MainWindow::finishWire(QPointF end) {
    if (!wireMode || !drawingWire) return;  // ✅ Only allow in Wire Mode

    currentWire->setEndPoint(end);
    drawingWire = false;
    currentWire = nullptr;
}
void MainWindow::listCircuit() {
    logConsole->append("Listing all circuit components:");
    circuit.listComponents();
}

void MainWindow::toggleWireMode(bool enabled) {
    wireMode = enabled;
}


