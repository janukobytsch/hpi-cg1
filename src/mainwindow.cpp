#include "mainwindow.h"
#include "exercise123.h"
#include "exercise4a.h"
#include "exercise4b.h"

//
// Qt
//
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>

using namespace Qt;

QWidget* MainWindow::createMultiViewWidget(QList<Exercise123*> &listViews, Exercise123 *orig)
{
    QGridLayout *layout = new QGridLayout;
    for(int i = 0; i < listViews.size(); i++)
    {
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listViews.at(i)->sizePolicy().hasHeightForWidth());
        listViews.at(i)->setSizePolicy(sizePolicy);
        listViews.at(i)->setMinimumSize(QSize(256, 256));
        listViews.at(i)->setFocusPolicy(Qt::WheelFocus);

        layout->addWidget(listViews.at(i), (i/2), i%2, 1, 1);

        // synchronize
        for(int j = 0; j < listViews.size(); j++)
        {
            if(i != j)
            {
                listViews.at(i)->synchronize(listViews.at(j));
            }
        }

        orig->synchronize(listViews.at(i));
        listViews.at(i)->synchronize(orig);
    }
    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}

MainWindow::MainWindow(QWidget *parent, QFlag flags) :
	QMainWindow(parent, flags)
{
    // Set window title
    setWindowTitle("3D Computergrafik I - SS2015 - Aufgabenblatt 1");

    // Create main container
    QWidget *container = new QWidget();
    setCentralWidget(container);

    // Add list (on the left)
    QListWidget *list = new QListWidget(container);
    list->addItem("Beispiel");
    list->addItem("Aufgabe 1");
    list->addItem("Aufgabe 2");
    list->addItem("Aufgabe 3");
    list->addItem("Aufgabe 4a");
    list->addItem("Aufgabe 4b");
    list->setMaximumWidth(150);

    // Add stack of exercise windows (on the right)
    QStackedWidget *stack = new QStackedWidget(container);

    // Add exercises
    Exercise123 *orig = new Exercise123(Exercise123::ORIGINAL);

    QList<Exercise123*> e1Views;
    e1Views.append(new Exercise123(Exercise123::GRAYSCALE));
    e1Views.append(new Exercise123(Exercise123::INVERT));

    QList<Exercise123*> e2Views;
    e2Views.append(new Exercise123(Exercise123::LAPLACE));
    e2Views.append(new Exercise123(Exercise123::GAUSS));
    e2Views.append(new Exercise123(Exercise123::SOBEL));
    e2Views.append(new Exercise123(Exercise123::MEANFILTER));

    Exercise123 *e3 = new Exercise123(Exercise123::DITHERED);

    Exercise4a *e4a = new Exercise4a();
    Exercise4b *e4b = new Exercise4b();


    QWidget *e1Window = createMultiViewWidget(e1Views,orig);
    QWidget *e2Window = createMultiViewWidget(e2Views,orig);

    stack->addWidget(orig);
    stack->addWidget(e1Window);
    stack->addWidget(e2Window);
    stack->addWidget(e3);
    stack->addWidget(e4a);
    stack->addWidget(e4b);

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
