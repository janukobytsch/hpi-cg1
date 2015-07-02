#include "mainwindow.h"

#include "exercise19.h"
#include "exercise20.h"

//
// Qt
//
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>

#include "util/canvas.h"
#include "util/glviewer.h"

using namespace Qt;

MainWindow::MainWindow(QWidget *parent, QFlag flags) :
	QMainWindow(parent, flags)
{
    // Set window title
    setWindowTitle("3D Computergrafik I - SS2015 - Aufgabenblatt 6");

    // Create main container
    QWidget *container = new QWidget();
    setCentralWidget(container);

    // Add list (on the left)
    QListWidget *list = new QListWidget(container);
    list->addItem("Aufgabe 19");
    list->addItem("Aufgabe 20");
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

    QSurfaceFormat format;

    // Add exercises to widget    

    format.setVersion(2, 1);

    GLViewer *exercise19 = new GLViewer(new Exercise19(), format);
    stack->addWidget(exercise19);

    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);

    GLViewer *exercise20 = new GLViewer(new Exercise20(), format);
    stack->addWidget(exercise20);

    connect(this, SIGNAL(keyPressed(QKeyEvent*)), exercise19, SLOT(keyPressed(QKeyEvent*)));
    connect(this, SIGNAL(animationFrameChanged(const float)), exercise20, SLOT(animationFrameChanged(const float)));
    connect(this, SIGNAL(keyPressed(QKeyEvent*)), exercise20, SLOT(keyPressed(QKeyEvent*)));

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
