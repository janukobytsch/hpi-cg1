#include "mainwindow.h"

#include "exercise21.h"
#include "exercise22.h"

//
// Qt
//
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QStatusBar>

#include "util/canvas.h"
#include "util/glviewer.h"

using namespace Qt;

MainWindow::MainWindow(QWidget *parent, QFlag flags) :
	QMainWindow(parent, flags)
{
    // Set window title
    setWindowTitle("3D Computergrafik I - SS2015 - Aufgabenblatt 7");

    // Create main container
    QWidget *container = new QWidget();
    setCentralWidget(container);

    // Add list (on the left)
    QListWidget *list = new QListWidget(container);
    list->addItem("Aufgabe 21");
    list->addItem("Aufgabe 22");
    list->setMaximumWidth(120);

    // Add stack of exercise windows (on the right)
    QStackedWidget *stack = new QStackedWidget(container);

    QSurfaceFormat format;

    // Add exercises to widget    

    format.setVersion(2, 1);

    Exercise21 *exercise21 = new Exercise21();
    m_hints.push_back(exercise21->hints());
    GLViewer *exercise21Viewer = new GLViewer(exercise21, format);
    stack->addWidget(exercise21Viewer);

    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);

    Exercise22 *exercise22 = new Exercise22();
    m_hints.push_back(exercise22->hints());
    GLViewer *exercise22Viewer = new GLViewer(exercise22, format);
    stack->addWidget(exercise22Viewer);

    connect(this, SIGNAL(keyPressed(QKeyEvent*)), exercise21Viewer, SLOT(keyPressed(QKeyEvent*)));
    connect(this, SIGNAL(keyPressed(QKeyEvent*)), exercise22Viewer, SLOT(keyPressed(QKeyEvent*)));

    // create status bar
    m_statusBar = new QStatusBar();
    setStatusBar(m_statusBar);

    // Create layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(list);
    layout->addWidget(stack);
    container->setLayout(layout);

    // Connect selection-event of list to select the current visible window
    connect(list, SIGNAL(currentRowChanged(int)), stack, SLOT(setCurrentIndex(int)));
    connect(list, SIGNAL(currentRowChanged(int)), this, SLOT(setCurrentStatusText(int)));
}

MainWindow::~MainWindow()
{
    //
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed(event);
}

void MainWindow::setCurrentStatusText(int index)
{
    m_statusBar->showMessage(m_hints[index]);
}
