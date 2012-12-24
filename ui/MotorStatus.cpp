#include "MotorStatus.h"
#include "ui_MotorStatus.h"

MotorStatus::MotorStatus(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::MotorStatus)
{
    ui->setupUi(this);
}

MotorStatus::~MotorStatus()
{
    delete ui;
}
