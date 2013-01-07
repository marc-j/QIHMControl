#include "Plot.h"
#include <QVBoxLayout>

Plot::Plot(QWidget *parent) :
    QWidget(parent)
{
    colors = new QList<QColor>();
    colors->append(Qt::blue);
    colors->append(Qt::red);
    colors->append(Qt::green);
    colors->append(Qt::cyan);
    colors->append(Qt::yellow);

    drawUI();
    drawPlot();
}

Plot::~Plot()
{
    delete plot;
}

void Plot::drawUI()
{
    plot = new QCustomPlot(this);
    QVBoxLayout* layout = new QVBoxLayout;

    plot->setColor(Qt::darkGray);

    layout->addWidget(plot);

    this->setLayout(layout);

}

void Plot::drawPlot()
{
    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica",9));
    // set locale to english, so we get english decimal separator:
    plot->setLocale(QLocale(QLocale::French, QLocale::France));

    plot->addGraph();
    plot->graph(0)->setName("ACC X");
    plot->graph(0)->setPen(QPen(Qt::blue));
    plot->graph(0)->setAntialiasedFill(false);

    plot->addGraph();
    plot->graph(1)->setName("ACC Y");
    plot->graph(1)->setPen(QPen(Qt::red));
    plot->graph(1)->setAntialiasedFill(false);

    plot->xAxis->setTickLabelType(QCPAxis::ltNumber);
   // plot->xAxis->setDateTimeFormat("hh:mm:ss");
    plot->xAxis->setAutoTickStep(false);
    plot->xAxis->setTickStep(1);
    plot->setupFullAxesBox();

    for (int i=0; i< 150; i++) {
        double y = cos(i*i*M_PI)*10;
        plot->graph(0)->addData(i,y);
    }

    plot->graph(0)->rescaleAxes();

    for (int i=0; i< 150; i++) {
        double y = sin(i*i);
        plot->graph(1)->addData(i,y);
    }

    plot->graph(1)->rescaleAxes(true);

    //plot->xAxis->setRange(2, 8, Qt::AlignRight);
    plot->replot();
}

void Plot::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    emit visibilityChanged(true);
}

void Plot::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);
    emit visibilityChanged(false);
}
