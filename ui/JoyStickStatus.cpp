#include "JoyStickStatus.h"
#include "ui_JoyStickStatus.h"

JoyStickStatus::JoyStickStatus(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::JoyStickStatus)
{
    ui->setupUi(this);
}

JoyStickStatus::~JoyStickStatus()
{
    delete ui;
}
