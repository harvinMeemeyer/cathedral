#include "gui/main_window.h"
#include "gui/component_item.h"
#include "gui/wire_item.h"
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QGraphicsScene>
#include <QTextEdit>
#include <QAction>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

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
    schematicView->setDragMode(QGraphicsView::RubberBandDrag);
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

    addResistorAction = new QAction("➕ R", this);
    addCapacitorAction = new QAction("➕ C", this);
    toggleWireModeAction = new QAction("Wire Mode", this);
    toggleWireModeAction->setCheckable(true);
    toggleDeleteModeAction = new QAction("Delete Mode", this);
    toggleDeleteModeAction->setCheckable(true);

    toolBar->addAction(addResistorAction);
    toolBar->addAction(addCapacitorAction);
    toolBar->addAction(toggleWireModeAction);
    toolBar->addAction(toggleDeleteModeAction);

    connect(addResistorAction, &QAction::triggered, this, &MainWindow::addResistor);
    connect(addCapacitorAction, &QAction::triggered, this, &MainWindow::addCapacitor);
    connect(toggleWireModeAction, &QAction::toggled, this, &MainWindow::toggleWireMode);
    connect(toggleDeleteModeAction, &QAction::toggled, this, &MainWindow::toggleDeleteMode);
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
    connect(resistor, &ComponentItem::positionChanged, this, &MainWindow::onComponentMoved);
    componentX += 50;
}

void MainWindow::addCapacitor() {
    circuit.addComponent("Capacitor", 0.01, 2, 3);
    logConsole->append("Added Capacitor (10nF) between nodes 2 and 3.");
    ComponentItem *capacitor = new ComponentItem("Capacitor", componentX, 0);
    scene->addItem(capacitor);
    connect(capacitor, &ComponentItem::positionChanged, this, &MainWindow::onComponentMoved);
    componentX += 50;
}

void MainWindow::startWire(QPointF start) {
    if (!wireMode || drawingWire || deleteMode) return;
    drawingWire = true;
    wireStartPoint = start;
    currentWire = nullptr;
    logConsole->append("Wire started at (" + QString::number(start.x()) + ", " + QString::number(start.y()) + ")");
    qDebug() << "Wire started at" << start;
}

void MainWindow::finishWire(QPointF end) {
    if (!wireMode || !drawingWire || deleteMode) return;

    // Find the components and their nearest terminals
    ComponentItem *startComponent = nullptr;
    ComponentItem *endComponent = nullptr;
    QPointF startTerminal, endTerminal;
    for (QGraphicsItem *item : scene->items()) {
        if (auto *comp = dynamic_cast<ComponentItem*>(item)) {
            if (comp->sceneBoundingRect().contains(wireStartPoint)) {
                startComponent = comp;
                startTerminal = comp->getNearestTerminal(wireStartPoint);
            }
            if (comp->sceneBoundingRect().contains(end)) {
                endComponent = comp;
                endTerminal = comp->getNearestTerminal(end);
            }
        }
    }

    if (!startComponent || !endComponent) {
        logConsole->append("Error: Must connect two components.");
        drawingWire = false;
        return;
    }

    // Snap to grid
    int gridSize = 20;
    startTerminal.setX(qRound(startTerminal.x() / gridSize) * gridSize);
    startTerminal.setY(qRound(startTerminal.y() / gridSize) * gridSize);
    endTerminal.setX(qRound(endTerminal.x() / gridSize) * gridSize);
    endTerminal.setY(qRound(endTerminal.y() / gridSize) * gridSize);

    qDebug() << "Snapped start terminal:" << startTerminal << "Snapped end terminal:" << endTerminal;

    // Create Manhattan path
    QList<WireItem*> wireSegments;
    if (startTerminal != endTerminal) {
        // First segment: horizontal to end's x
        WireItem *segment1 = new WireItem(startTerminal, QPointF(endTerminal.x(), startTerminal.y()));
        wireSegments.append(segment1);

        // Second segment: vertical to end's y
        WireItem *segment2 = new WireItem(QPointF(endTerminal.x(), startTerminal.y()), endTerminal);
        wireSegments.append(segment2);

        // Add segments to scene
        for (WireItem *segment : wireSegments) {
            scene->addItem(segment);
        }
    }

    // Store the wire connection
    WireConnection connection;
    connection.startComponent = startComponent;
    connection.startTerminal = startTerminal;
    connection.endComponent = endComponent;
    connection.endTerminal = endTerminal;
    connection.segments = wireSegments;
    wireConnections.append(connection);

    qDebug() << "Wire segments created:" << wireSegments.count();
    for (int i = 0; i < wireSegments.count(); ++i) {
        qDebug() << "Segment" << i << ": Line =" << wireSegments[i]->line();
    }

    drawingWire = false;
    logConsole->append("Wire completed between (" + QString::number(startTerminal.x()) + ", " + QString::number(startTerminal.y()) +
                       ") and (" + QString::number(endTerminal.x()) + ", " + QString::number(endTerminal.y()) + ")");
    scene->update();
}

void MainWindow::onComponentMoved(ComponentItem *component) {
    qDebug() << "Component moved:" << component->getType() << "to" << component->pos();

    // Update all wires connected to this component
    for (WireConnection &connection : wireConnections) {
        if (connection.startComponent == component || connection.endComponent == component) {
            // Remove old wire segments from scene
            for (WireItem *segment : connection.segments) {
                scene->removeItem(segment);
                delete segment;
            }
            connection.segments.clear();

            // Recalculate terminals based on new positions
            QPointF startTerminal = connection.startComponent->getNearestTerminal(connection.startComponent->pos());
            QPointF endTerminal = connection.endComponent->getNearestTerminal(connection.endComponent->pos());

            // Snap to grid
            int gridSize = 20;
            startTerminal.setX(qRound(startTerminal.x() / gridSize) * gridSize);
            startTerminal.setY(qRound(startTerminal.y() / gridSize) * gridSize);
            endTerminal.setX(qRound(endTerminal.x() / gridSize) * gridSize);
            endTerminal.setY(qRound(endTerminal.y() / gridSize) * gridSize);

            // Update stored terminals
            connection.startTerminal = startTerminal;
            connection.endTerminal = endTerminal;

            // Create new Manhattan path
            if (startTerminal != endTerminal) {
                WireItem *segment1 = new WireItem(startTerminal, QPointF(endTerminal.x(), startTerminal.y()));
                WireItem *segment2 = new WireItem(QPointF(endTerminal.x(), startTerminal.y()), endTerminal);
                connection.segments.append(segment1);
                connection.segments.append(segment2);

                // Add new segments to scene
                for (WireItem *segment : connection.segments) {
                    scene->addItem(segment);
                }
            }

            qDebug() << "Updated wire between" << startTerminal << "and" << endTerminal;
        }
    }
    scene->update();
}

void MainWindow::deleteComponent(ComponentItem *component) {
    // Remove all wires connected to this component
    for (int i = wireConnections.size() - 1; i >= 0; --i) {
        WireConnection &connection = wireConnections[i];
        if (connection.startComponent == component || connection.endComponent == component) {
            // Remove wire segments from scene
            for (WireItem *segment : connection.segments) {
                scene->removeItem(segment);
                delete segment;
            }
            connection.segments.clear();
            wireConnections.removeAt(i);
        }
    }

    // Remove the component from the scene
    scene->removeItem(component);
    delete component;
    logConsole->append("Deleted component: " + component->getType());
}

void MainWindow::deleteWire(WireItem *wire) {
    // Find the wire connection containing this wire segment
    for (int i = 0; i < wireConnections.size(); ++i) {
        WireConnection &connection = wireConnections[i];
        if (connection.segments.contains(wire)) {
            // Remove all segments of this connection from the scene
            for (WireItem *segment : connection.segments) {
                scene->removeItem(segment);
                delete segment;
            }
            connection.segments.clear();
            wireConnections.removeAt(i);
            logConsole->append("Deleted wire");
            break;
        }
    }
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
    toggleWireModeAction->setChecked(wireMode);
    if (wireMode) {
        if (deleteMode) {
            toggleDeleteMode(false);
        }
        schematicView->setDragMode(QGraphicsView::NoDrag);
        logConsole->append("Wire Mode ON");
    } else {
        if (!deleteMode) {
            schematicView->setDragMode(QGraphicsView::RubberBandDrag);
        }
        logConsole->append("Wire Mode OFF");
    }
}

void MainWindow::toggleDeleteMode(bool enabled) {
    deleteMode = enabled;
    toggleDeleteModeAction->setChecked(deleteMode);
    if (deleteMode) {
        if (wireMode) {
            toggleWireMode(false);
        }
        schematicView->setDragMode(QGraphicsView::NoDrag);  // Explicitly disable drag
        logConsole->append("Delete Mode ON");
    } else {
        if (!wireMode) {
            schematicView->setDragMode(QGraphicsView::RubberBandDrag);  // Re-enable drag only if not in wire mode
        }
        logConsole->append("Delete Mode OFF");
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    qDebug() << "Mouse press in mode - Wire:" << wireMode << "Delete:" << deleteMode;  // Debug mode state
    if (deleteMode) {
        QPointF pos = schematicView->mapToScene(event->pos());
        QGraphicsItem *item = scene->itemAt(pos, QTransform());
        qDebug() << "Clicked item at:" << pos << (item ? item->type() : -1);  // Debug click
        if (auto *comp = dynamic_cast<ComponentItem*>(item)) {
            deleteComponent(comp);
            event->accept();
        } else if (auto *wire = dynamic_cast<WireItem*>(item)) {
            deleteWire(wire);
            event->accept();
        } else {
            qDebug() << "No deletable item found at click position";  // Debug miss
        }
        return;
    }

    if (wireMode) {
        QPointF pos = schematicView->mapToScene(event->pos());
        QGraphicsItem *item = scene->itemAt(pos, QTransform());
        if (!drawingWire && item && dynamic_cast<ComponentItem*>(item)) {
            startWire(pos);
        } else if (drawingWire && (!item || dynamic_cast<ComponentItem*>(item))) {
            finishWire(pos);
        }
        event->accept();
    } else {
        QMainWindow::mousePressEvent(event);  // Allow drag in default mode
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W) {
        toggleWireMode(!wireMode);
        event->accept();
    } else if (event->key() == Qt::Key_R) {
        addResistor();
        event->accept();
    } else if (event->key() == Qt::Key_C) {
        addCapacitor();
        event->accept();
    } else if (event->key() == Qt::Key_D) {
        toggleDeleteMode(!deleteMode);
        event->accept();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}
