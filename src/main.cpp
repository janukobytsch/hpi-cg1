#include "mainwindow.h"

//
// Qt
//
#include <QApplication>

//[-------------------------------------------------------]
//[ Main function                                         ]
//[-------------------------------------------------------]
int main(int argc, char *argv[])
{
    // Create application
    QApplication app(argc, argv);

    // Create main window
    MainWindow *window = new MainWindow();
    window->show();

    // Run application
    return app.exec();
}
