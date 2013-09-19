#include "Plot.h"
#include <QVBoxLayout>

Plot::Plot(QWidget *parent) :
    QWidget(parent)
{

    uav = UAV::instance();

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
    delete plotX;
    delete plotY;
    delete plotZ;
}

void Plot::drawUI()
{

    QVBoxLayout* layout = new QVBoxLayout;

    plotX = new QCustomPlot(this);
    //plotX->setColor(Qt::darkGray);
    layout->addWidget(plotX);

    plotY = new QCustomPlot(this);
    //plotY->setColor(Qt::darkGray);
    layout->addWidget(plotY);

    plotZ = new QCustomPlot(this);
    //plotZ->setColor(Qt::darkGray);
    layout->addWidget(plotZ);

    this->setLayout(layout);

}

void Plot::drawPlot()
{
    //
    // X
    plotX->legend->setVisible(true);
    plotX->legend->setFont(QFont("Helvetica",9));
    plotX->setLocale(QLocale(QLocale::French, QLocale::France));

    plotX->addGraph();
    plotX->graph(0)->setName("ACCX");
    plotX->graph(0)->setPen(QPen(Qt::blue));
    plotX->graph(0)->setAntialiasedFill(false);

    plotX->addGraph();
    plotX->graph(1)->setName("GYRX");
    plotX->graph(1)->setPen(QPen(Qt::red));
    plotX->graph(1)->setAntialiasedFill(false);

    plotX->addGraph();
    plotX->graph(2)->setName("ROLL");
    plotX->graph(2)->setPen(QPen(Qt::green));
    plotX->graph(2)->setAntialiasedFill(false);

    plotX->addGraph();
    plotX->graph(3)->setName("MAGX");
    plotX->graph(3)->setPen(QPen(Qt::yellow));
    plotX->graph(3)->setAntialiasedFill(false);

    plotX->xAxis->setTickLabelType(QCPAxis::ltNumber);
    plotX->xAxis->setAutoTickStep(false);
    plotX->xAxis->setTickStep(1);

    plotX->xAxis->setRange(0, 100, Qt::AlignLeft);
   // plotX->setupFullAxesBox();

    connect(plotX->xAxis, SIGNAL(rangeChanged(QCPRange)), plotX->xAxis2, SLOT(setRange(QCPRange)));
    connect(plotX->yAxis, SIGNAL(rangeChanged(QCPRange)), plotX->yAxis2, SLOT(setRange(QCPRange)));

    //
    // Y
    plotY->legend->setVisible(true);
    plotY->legend->setFont(QFont("Helvetica",9));
    plotY->setLocale(QLocale(QLocale::French, QLocale::France));

    plotY->addGraph();
    plotY->graph(0)->setName("ACCY");
    plotY->graph(0)->setPen(QPen(Qt::blue));
    plotY->graph(0)->setAntialiasedFill(false);

    plotY->addGraph();
    plotY->graph(1)->setName("GYRY");
    plotY->graph(1)->setPen(QPen(Qt::red));
    plotY->graph(1)->setAntialiasedFill(false);

    plotY->addGraph();
    plotY->graph(2)->setName("PITCH");
    plotY->graph(2)->setPen(QPen(Qt::green));
    plotY->graph(2)->setAntialiasedFill(false);

    plotY->addGraph();
    plotY->graph(3)->setName("MAGY");
    plotY->graph(3)->setPen(QPen(Qt::yellow));
    plotY->graph(3)->setAntialiasedFill(false);

    plotY->xAxis->setTickLabelType(QCPAxis::ltNumber);
    plotY->xAxis->setAutoTickStep(false);
    plotY->xAxis->setTickStep(1);
    plotY->xAxis->setRange(0, 100, Qt::AlignLeft);
    //plotY->setupFullAxesBox();

    connect(plotY->xAxis, SIGNAL(rangeChanged(QCPRange)), plotY->xAxis2, SLOT(setRange(QCPRange)));
    connect(plotY->yAxis, SIGNAL(rangeChanged(QCPRange)), plotY->yAxis2, SLOT(setRange(QCPRange)));

    //
    // Z
    plotZ->legend->setVisible(true);
    plotZ->legend->setFont(QFont("Helvetica",9));
    plotZ->setLocale(QLocale(QLocale::French, QLocale::France));

    plotZ->addGraph();
    plotZ->graph(0)->setName("ACCZ");
    plotZ->graph(0)->setPen(QPen(Qt::blue));
    plotZ->graph(0)->setAntialiasedFill(false);

    plotZ->addGraph();
    plotZ->graph(1)->setName("GYRZ");
    plotZ->graph(1)->setPen(QPen(Qt::red));
    plotZ->graph(1)->setAntialiasedFill(false);

    plotZ->addGraph();
    plotZ->graph(2)->setName("YAW");
    plotZ->graph(2)->setPen(QPen(Qt::green));
    plotZ->graph(2)->setAntialiasedFill(false);

    plotZ->addGraph();
    plotZ->graph(3)->setName("MAGZ");
    plotZ->graph(3)->setPen(QPen(Qt::yellow));
    plotZ->graph(3)->setAntialiasedFill(false);

    plotZ->xAxis->setTickLabelType(QCPAxis::ltNumber);
    plotZ->xAxis->setAutoTickStep(false);
    plotZ->xAxis->setTickStep(1);
    plotZ->xAxis->setRange(0, 50, Qt::AlignLeft);
    //plotZ->setupFullAxesBox();

    connect(plotZ->xAxis, SIGNAL(rangeChanged(QCPRange)), plotZ->xAxis2, SLOT(setRange(QCPRange)));
    connect(plotZ->yAxis, SIGNAL(rangeChanged(QCPRange)), plotZ->yAxis2, SLOT(setRange(QCPRange)));

}

void Plot::updateValue(QString name, double v)
{
    if (name == "ACCX") {
        plotX->graph(0)->addData(plotX->graph(0)->data()->count()+1, v*100.0);
    } else if (name == "GYROX" ) {
        plotX->graph(1)->addData(plotX->graph(1)->data()->count()+1, v);
    } else if (name == "ROLL") {
        plotX->graph(2)->addData(plotX->graph(2)->data()->count()+1, v);
    } else if (name == "MAGX") {
        plotX->graph(3)->addData(plotX->graph(2)->data()->count()+1, v*100.0);
    } else if (name == "ACCY") {
        plotY->graph(0)->addData(plotY->graph(0)->data()->count()+1, v*100.0);
    } else if (name == "GYROY" ) {
        plotY->graph(1)->addData(plotY->graph(1)->data()->count()+1, v);
    } else if (name == "PITCH") {
        plotY->graph(2)->addData(plotY->graph(2)->data()->count()+1, v);
    } else if (name == "MAGY") {
        plotY->graph(3)->addData(plotY->graph(2)->data()->count()+1, v*100.0);
    } else if (name == "ACCZ") {
        plotZ->graph(0)->addData(plotZ->graph(0)->data()->count()+1, v*100.0);
    } else if (name == "GYROZ" ) {
        plotZ->graph(1)->addData(plotZ->graph(1)->data()->count()+1, v);
    } else if (name == "YAW") {
        plotZ->graph(2)->addData(plotZ->graph(2)->data()->count()+1, v);
    } else if (name == "MAGZ") {
        plotZ->graph(3)->addData(plotZ->graph(2)->data()->count()+1, v*100.0);
    }

    plotX->graph(0)->rescaleAxes();
    plotX->graph(1)->rescaleAxes();
    plotX->graph(2)->rescaleAxes();
    plotX->graph(3)->rescaleAxes(true);
    plotX->xAxis->setRange(plotX->graph(0)->data()->count(), 60, Qt::AlignRight);
    plotX->replot();


    plotY->graph(0)->rescaleAxes();
    plotY->graph(1)->rescaleAxes();
    plotY->graph(2)->rescaleAxes();
    plotY->graph(3)->rescaleAxes(true);
    plotY->xAxis->setRange(plotY->graph(0)->data()->count(), 60, Qt::AlignRight);
    plotY->replot();


    plotZ->graph(0)->rescaleAxes();
    plotZ->graph(1)->rescaleAxes();
    plotZ->graph(2)->rescaleAxes();
    plotZ->graph(3)->rescaleAxes(true);
    plotZ->xAxis->setRange(plotZ->graph(0)->data()->count(), 60, Qt::AlignRight);
    plotZ->replot();

}

void Plot::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
    emit visibilityChanged(true);
    connect(uav, SIGNAL(updateValue(QString,double)), this, SLOT(updateValue(QString,double)));
}

void Plot::hideEvent(QHideEvent* event)
{
    Q_UNUSED(event);
    emit visibilityChanged(false);
    disconnect(uav, SIGNAL(updateValue(QString,double)), this, SLOT(updateValue(QString,double)));
}
