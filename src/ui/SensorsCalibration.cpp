#include "SensorsCalibration.h"
#include "ui_SensorsCalibration.h"

SensorsCalibration::SensorsCalibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorsCalibration),
    logFile("acc_raw.log"),
    sampling(false)
{
    ui->setupUi(this);

    int acc_range = 25000;

    uav = UAV::instance();

    plotXY = new QCustomPlot(this);
    plotYZ = new QCustomPlot(this);
    plotZX = new QCustomPlot(this);


    QVBoxLayout* layoutXY = new QVBoxLayout;
    layoutXY->addWidget(plotXY);
    ui->sensorsXY->setLayout(layoutXY);

    QVBoxLayout* layoutYZ = new QVBoxLayout;
    layoutYZ->addWidget(plotYZ);
    ui->sensorsYZ->setLayout(layoutYZ);

    QVBoxLayout* layoutZX = new QVBoxLayout;
    layoutZX->addWidget(plotZX);
    ui->sensorsZX->setLayout(layoutZX);

    plotXY->setColor(Qt::darkGray);
    plotXY->yAxis->setRange(-acc_range, acc_range);
    plotXY->xAxis->setRange(-acc_range, acc_range);
    plotXY->addGraph();
    plotXY->graph(0)->setPen(QPen(Qt::red));
    plotXY->graph(0)->setAntialiasedFill(false);

    plotYZ->setColor(Qt::darkGray);
    plotYZ->yAxis->setRange(-acc_range, acc_range);
    plotYZ->xAxis->setRange(-acc_range, acc_range);
    plotYZ->addGraph();
    plotYZ->graph(0)->setPen(QPen(Qt::green));
    plotYZ->graph(0)->setAntialiasedFill(false);

    plotZX->setColor(Qt::darkGray);
    plotZX->yAxis->setRange(-acc_range, acc_range);
    plotZX->xAxis->setRange(-acc_range, acc_range);
    plotZX->addGraph();
    plotZX->graph(0)->setPen(QPen(Qt::blue));
    plotZX->graph(0)->setAntialiasedFill(false);

   // logFile.setFileName("acc_raw.log");
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        //
    }

    connect(ui->btnSampling, SIGNAL(clicked()), this, SLOT(startSampling()));
    connect(ui->sensorsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sensorChange(int)));
    connect(uav, SIGNAL(accRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
}

SensorsCalibration::~SensorsCalibration() {

    delete plotXY;
    delete plotYZ;
    delete plotZX;
    delete ui;
}

void SensorsCalibration::rawChange(int16_t x, int16_t y, int16_t z) {
    datas[0].append((double) x);
    datas[1].append((double) y);
    datas[2].append((double) z);

    plotXY->graph(0)->setData(datas[0],datas[1]);
    plotYZ->graph(0)->addData(datas[1],datas[2]);
    plotZX->graph(0)->addData(datas[2],datas[0]);

    plotXY->replot();
    plotYZ->replot();
    plotZX->replot();

    if( sampling ) {
        QTextStream s(&logFile);
        s << QString().sprintf("%d %d %d\n",x,y,z);
    }
}

void SensorsCalibration::startSampling()
{
    sampling = ui->btnSampling->text() == "Sampling";
    if (sampling) {
        ui->btnSampling->setText("Stop");
    } else {
        ui->btnSampling->setText("Sampling");
    }
}

void SensorsCalibration::sensorChange(int i)
{
    if (sampling) {
        startSampling();
    }
    plotXY->graph(0)->clearData();
    plotYZ->graph(0)->clearData();
    plotZX->graph(0)->clearData();

    datas[0].clear();
    datas[1].clear();
    datas[2].clear();

    plotXY->replot();
    plotYZ->replot();
    plotZX->replot();

    if (i == 0) {
        //acc
        disconnect(uav, SIGNAL(magRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
        connect(uav, SIGNAL(accRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
    } else {
        disconnect(uav, SIGNAL(accRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
        connect(uav, SIGNAL(magRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
    }
}
