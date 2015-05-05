#include "mainwindow.h"

#include "exercise5.h"
#include "exercise6.h"

//
// Qt
//
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>

#include "util/glviewer.h"

using namespace Qt;

MainWindow::MainWindow(QWidget *parent, QFlag flags) :
	QMainWindow(parent, flags)
{
    // Set window title
    setWindowTitle("3D Computergrafik I - SS2015 - Aufgabenblatt 2");

    // Create main container
    QWidget *container = new QWidget();
    setCentralWidget(container);

    // Add list (on the left)
    QListWidget *list = new QListWidget(container);
    list->addItem("Aufgabe 5");
    list->addItem("Aufgabe 6");
    list->setMaximumWidth(150);

    // Add stack of exercise windows (on the right)
    QStackedWidget *stack = new QStackedWidget(container);

    // Add exercises
    Exercise5 *e5 = new Exercise5();
    Exercise6 *e6 = new Exercise6();
    GLViewer *glviewer = new GLViewer(e6);

    // Add widgets to stack
    stack->addWidget(e5);
    stack->addWidget(glviewer);

    // Create layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(list);
    layout->addWidget(stack);
    container->setLayout(layout);

    // Connect selection-event of list to select the current visible window
    connect(list, SIGNAL(currentRowChanged(int)), stack, SLOT(setCurrentIndex(int)));
}

MainWindow::~MainWindow()
{
    //
}
