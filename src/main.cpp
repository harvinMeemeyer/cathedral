#include <QApplication>
#include "gui/main_window.h"
#include "util/logging.h"

int main(int argc, char *argv[]) {
    Cathedral::Logger::SetLogFile("cathedral.log");
    Cathedral::Logger::Log("Starting GUI...", Cathedral::LogLevel::INFO);

    QApplication app(argc, argv);
    MainWindow window;
    window.show();

    return app.exec();
}
