#include "mainwindow.h"

#include "exercise12.h"
#include "exercise13.h"
#include "exercise14.h"

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
    setWindowTitle("3D Computergrafik I - SS2015 - Aufgabenblatt 4");

    // Create main container
    QWidget *container = new QWidget();
    setCentralWidget(container);

    // Add list (on the left)
    QListWidget *list = new QListWidget(container);
    list->addItem("Aufgabe 12");
    list->addItem("Aufgabe 13");
    list->addItem("Aufgabe 14");
    list->setMaximumWidth(150);

    // Add stack of exercise windows (on the right)
    QStackedWidget *stack = new QStackedWidget(container);

    // Add exercises to widget
    stack->addWidget(new GLViewer(new Exercise12()));
    stack->addWidget(new GLViewer(new Exercise13()));
    stack->addWidget(new GLViewer(new Exercise14()));

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
