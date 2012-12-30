#include "JoyStickStatus.h"
#include "ui_JoyStickStatus.h"

#include <QDebug>

JoyStickStatus::JoyStickStatus(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::JoyStickStatus)
{
    ui->setupUi(this);

    joystick = Joystick::instance();
    connect(joystick, SIGNAL(joystickChanged(QList<int>,QList<double>, QList<bool>)), this, SLOT(joystickChanged(QList<int>,QList<double>, QList<bool>)));


}

JoyStickStatus::~JoyStickStatus()
{
    delete ui;
}

void JoyStickStatus::joystickChanged(QList<int> axisNorm,QList<double> axis, QList<bool> buttons)
{
    Q_UNUSED(axis);
    Q_UNUSED(buttons);

    ui->xAxis->display(axisNorm.at(XAXIS));
    ui->yAxis->display(axisNorm.at(YAXIS));
    ui->zAxis->display(axisNorm.at(ZAXIS));
    ui->throttleAxis->display(axisNorm.at(TAXIS));
}
