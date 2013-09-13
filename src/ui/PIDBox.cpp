#include <QDebug>

#include "PIDBox.h"
#include "ui_PIDBox.h"

PIDBox::PIDBox(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PIDBox)
{
    ui->setupUi(this);
    readSettings();

    uav = UAV::instance();
    connect(ui->saveBtn, SIGNAL(clicked()), this, SLOT(savePID()));
    connect(this, SIGNAL(rollPIDchanged(float,float,float)), uav, SLOT(updateRollPID(float,float,float)));
    connect(this, SIGNAL(pitchPIDchanged(float,float,float)), uav, SLOT(updatePitchPID(float,float,float)));
    connect(this, SIGNAL(yawPIDchanged(float,float,float)), uav, SLOT(updateYawPID(float,float,float)));

    connect(uav, SIGNAL(pidRollKI(int)), this, SLOT(rollKi(int)));
    connect(uav, SIGNAL(pidRollKP(int)), this, SLOT(rollKp(int)));
}

PIDBox::~PIDBox()
{
    delete ui;
}

void PIDBox::rollKi(int i) {
    ui->pitchKi->setValue((double)ui->pitchKi->value() + (i/1000.0f));
    ui->rollKi->setValue((double)ui->rollKi->value() + (i/1000.0f));

    emit pitchPIDchanged((float)ui->pitchKp->value(), (float)ui->pitchKi->value(), (float)ui->pitchKd->value());
    emit pitchPIDchanged((float)ui->rollKp->value(), (float)ui->rollKi->value(), (float)ui->rollKd->value());
}

void PIDBox::rollKp(int i) {
     ui->pitchKp->setValue((double)ui->pitchKp->value() + (i/100.0f));
     ui->rollKp->setValue((double)ui->rollKp->value() + (i/100.0f));

     emit pitchPIDchanged((float)ui->pitchKp->value(), (float)ui->pitchKi->value(), (float)ui->pitchKd->value());
     emit pitchPIDchanged((float)ui->rollKp->value(), (float)ui->rollKi->value(), (float)ui->rollKd->value());
}

void PIDBox::readSettings()
{
    settings.beginGroup("PID");
    ui->rollKp->setValue(settings.value("ROLL_KP", 0.0).toDouble());
    ui->rollKi->setValue(settings.value("ROLL_KI", 0.0).toDouble());
    ui->rollKd->setValue(settings.value("ROLL_KD", 0.0).toDouble());


    ui->pitchKp->setValue(settings.value("PITCH_KP", 0.0).toDouble());
    ui->pitchKi->setValue(settings.value("PITCH_KI", 0.0).toDouble());
    ui->pitchKd->setValue(settings.value("PITCH_KD", 0.0).toDouble());


    ui->yawKp->setValue(settings.value("YAW_KP", 0.0).toDouble());
    ui->yawKi->setValue(settings.value("YAW_KI", 0.0).toDouble());
    ui->yawKd->setValue(settings.value("YAW_KD", 0.0).toDouble());


    settings.endGroup();

}

void PIDBox::savePID()
{
    qDebug() << "CLICK";
    emit rollPIDchanged((float)ui->rollKp->value(), (float)ui->rollKi->value(), (float)ui->rollKd->value());
    emit pitchPIDchanged((float)ui->pitchKp->value(), (float)ui->pitchKi->value(), (float)ui->pitchKd->value());
    emit yawPIDchanged((float)ui->yawKp->value(), (float)ui->yawKi->value(), (float)ui->yawKd->value());
}
