#include "mainwindow.h"

#include "exercise16.h"
#include "exercise17.h"
#include "exercise18.h"
#include "util/canvas.h"

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
    setWindowTitle("3D Computergrafik I - SS2015 - Aufgabenblatt 5");

    // Create main container
    QWidget *container = new QWidget();
    setCentralWidget(container);

    // Add list (on the left)
    QListWidget *list = new QListWidget(container);
    list->addItem("Aufgabe 15 / 16");
    list->addItem("Aufgabe 17");
    list->addItem("Aufgabe 18");
    list->setMaximumWidth(120);

    // add slider (in the middle)
    m_inputSlider = new QSlider(Qt::Vertical);
    m_inputSlider->setRange(0, 100);
    m_inputSlider->setSingleStep(1);
    m_inputSlider->setTickInterval(10);
    m_inputSlider->setTickPosition(QSlider::TicksRight);
    m_inputSlider->setValue(10);

    // Add stack of exercise windows (on the right)
    QStackedWidget *stack = new QStackedWidget(container);

    // Add exercises to widget    
    GLViewer *exercise1516 = new GLViewer(new Exercise16());
    stack->addWidget(exercise1516);
    GLViewer *exercise17 = new GLViewer(new Exercise17());
    stack->addWidget(exercise17);
    GLViewer *exercise18 = new GLViewer(new Exercise18());
    stack->addWidget(exercise18);

    connect(this, SIGNAL(animationFrameChanged(const float)), exercise1516, SLOT(animationFrameChanged(const float)));
    connect(this, SIGNAL(keyPressed(QKeyEvent*)), exercise1516, SLOT(keyPressed(QKeyEvent*)));
    connect(this, SIGNAL(animationFrameChanged(const float)), exercise17, SLOT(animationFrameChanged(const float)));
    connect(this, SIGNAL(keyPressed(QKeyEvent*)), exercise17, SLOT(keyPressed(QKeyEvent*)));
    connect(this, SIGNAL(animationFrameChanged(const float)), exercise18, SLOT(animationFrameChanged(const float)));
    connect(this, SIGNAL(keyPressed(QKeyEvent*)), exercise18, SLOT(keyPressed(QKeyEvent*)));

    handleSliderInput(m_inputSlider->value());

    // Create layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(list);
    layout->addWidget(m_inputSlider);
    layout->addWidget(stack);
    container->setLayout(layout);

    // Connect selection-event of list to select the current visible window
    connect(list, SIGNAL(currentRowChanged(int)), stack, SLOT(setCurrentIndex(int)));
    connect(m_inputSlider, SIGNAL(valueChanged(int)), this, SLOT(handleSliderInput(const int)));
}

MainWindow::~MainWindow()
{
    //
}

void MainWindow::handleSliderInput(const int value)
{
    float tickPos = static_cast<float>(value) / static_cast<float>(m_inputSlider->maximum());
    emit animationFrameChanged(tickPos);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    emit keyPressed(event);
}