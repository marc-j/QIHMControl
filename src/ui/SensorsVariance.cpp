#include "SensorsVariance.h"
#include "ui_SensorsVariance.h"

SensorsVariance::SensorsVariance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorsVariance)
{
    ui->setupUi(this);
    uav = UAV::instance();

    connect(uav, SIGNAL(sensorsVariance(float,float,float,float,float,float)), this, SLOT(sensorsVariance(float,float,float,float,float,float)));
    connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(startCalculateVariance()));
}

SensorsVariance::~SensorsVariance()
{
    disconnect(uav, SIGNAL(sensorsVariance(float,float,float,float,float,float)), this, SLOT(sensorsVariance(float,float,float,float,float,float)));
    delete ui;
}

void SensorsVariance::sensorsVariance(float accX, float accY, float accZ, float gyroX, float gyroY, float gyroZ)
{
    ui->accX->setText(QString::number(accX));
    ui->accY->setText(QString::number(accY));
    ui->accZ->setText(QString::number(accZ));

    ui->gyroX->setText(QString::number(gyroX));
    ui->gyroY->setText(QString::number(gyroY));
    ui->gyroZ->setText(QString::number(gyroZ));

    ui->btnStart->setEnabled(true);
}

void SensorsVariance::startCalculateVariance()
{
    uav->sendGetVariance(); // Send get variance to system and waiting for response.
   // ui->btnStart->setEnabled(false);
}
