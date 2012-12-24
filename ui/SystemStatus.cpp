#include "SystemStatus.h"
#include "ui_SystemStatus.h"

SystemStatus::SystemStatus(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SystemStatus)
{
    ui->setupUi(this);
}

SystemStatus::~SystemStatus()
{
    delete ui;
}
