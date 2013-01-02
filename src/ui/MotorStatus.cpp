#include "MotorStatus.h"
#include "ui_MotorStatus.h"

#include <QDebug>

MotorStatus::MotorStatus(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::MotorStatus)
{
    ui->setupUi(this);

    uav = UAV::instance();
    connect(uav, SIGNAL(motorChange(int,int,int,int)), this, SLOT(motorChange(int,int,int,int)));
}

MotorStatus::~MotorStatus()
{
    delete ui;
}

void MotorStatus::motorChange(int fl, int fr, int rl, int rr)
{
    ui->frontLeftBar->setValue(fl);
    ui->frontRightBar->setValue(fr);
    ui->rearLeftBar->setValue(rl);
    ui->rearRightBar->setValue(rr);
}
