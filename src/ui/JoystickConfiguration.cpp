#include "JoystickConfiguration.h"
#include "ui_JoystickConfiguration.h"

#include "ui/JoystickCalibration.h"

JoystickConfiguration::JoystickConfiguration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JoystickConfiguration)
{
    ui->setupUi(this);
    this->setWindowTitle("Joystick");

    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(onSave()));

    connect(ui->joyBox,SIGNAL(currentIndexChanged(int)), this, SLOT(updateSelection(int)));

    joystick = Joystick::instance();

    init();
}

JoystickConfiguration::~JoystickConfiguration()
{
    delete ui;
}

void JoystickConfiguration::init()
{
    ui->joyBox->clear();
    QList<Joystick::joydata*> list = joystick->getJoysticks();

    switch (list.length()) {
        case 0:
            ui->joyBox->addItem("No joysticks found.");
            ui->joyBox->setDisabled(true);
            ui->btnSave->setDisabled(true);
            break;
        default:
            ui->joyBox->setDisabled(false);
            ui->btnSave->setDisabled(false);
            for (int i=0; i<list.length(); i++) {
                joystick->setJoystick(i);
                ui->joyBox->addItem(list.at(i)->name);
            }

            joystick->setJoystick(ui->joyBox->currentIndex());
            ui->countAxis->setText(QString("%1").arg(list.at(0)->number_axes));
            ui->countButtons->setText(QString("%1").arg(list.at(0)->number_btn));
            break;
    }

}

void JoystickConfiguration::onSave()
{
    joystick->setJoystick(ui->joyBox->currentIndex());
    this->close();
}

void JoystickConfiguration::updateSelection(int index)
{
    QList<Joystick::joydata*> list = joystick->getJoysticks();

    joystick->setJoystick(index);
    ui->countAxis->setText(QString("%1").arg(list.at(index)->number_axes));
    ui->countButtons->setText(QString("%1").arg(list.at(index)->number_btn));
}
