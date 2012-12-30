#include "SystemStatus.h"
#include "ui_SystemStatus.h"

SystemStatus::SystemStatus(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SystemStatus)
{
    ui->setupUi(this);

    uav = UAV::instance();
    connect(uav, SIGNAL(cpuLoadChange(double)), this, SLOT(cpuLoadChange(double)));
    connect(uav, SIGNAL(mainLoopChange(double)), this, SLOT(mainLoopChange(double)));

}

SystemStatus::~SystemStatus()
{
    delete ui;
}

void SystemStatus::mainLoopChange(double ms)
{
    ui->mainLoopLabel->setText(QString("%1").arg(ms));
    //ui->cpuLoadbar->setValue(percent);
}

void SystemStatus::cpuLoadChange(double value)
{
    int percent = (int)mapValue(value, 0, 1, 0, 100);
    ui->cpuLoadLabel->setText(QString("%1").arg(percent));
    ui->cpuLoadbar->setValue(percent);
}

void SystemStatus::batteryVoltageChange(double volt)
{

}
