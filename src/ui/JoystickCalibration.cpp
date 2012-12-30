#include "JoystickCalibration.h"

JoystickCalibration::JoystickCalibration(QWidget *parent) :
    QWidget(parent)
{
    joystick = Joystick::instance();
    joyData = joystick->getCurrentJoystick();

    setWindowTitle("Joystick calibration");

    initUI();

    connect(joystick, SIGNAL(joystickChanged(QList<int>,QList<double>,QList<bool>)), this, SLOT(joystickChanged(QList<int>,QList<double>,QList<bool>)));
}


void JoystickCalibration::joystickChanged(QList<int> axisNorm, QList<double> axis, QList<bool> buttons)
{
    for (int i=0; i<joyData->number_axes;i ++) {
        this->axis.at(i)->bar->setValue(axis.at(i));
        this->axis.at(i)->value->setText(QString("%1").arg(axis.at(i)));

        this->axis.at(i)->min->setText(QString("%1").arg(joyData->joyCalibration.axisMin.at(i)));
        this->axis.at(i)->max->setText(QString("%1").arg(joyData->joyCalibration.axisMax.at(i)));
    }

    for (int i=0; i<joyData->number_btn;i ++) {
        checklist[i]->setChecked(buttons.at(i));
    }
}

void JoystickCalibration::startCalibration()
{
    if (joystick->calibrationIsStarted()) {
        joystick->stopCalibration();
        btnStart->setText("Start Calibration");
    } else {
        joystick->startCalibration();
        btnStart->setText("Stop Calibration");
    }
}

void JoystickCalibration::initUI()
{
    mainLayout = new QVBoxLayout;
    QHBoxLayout *axes_Layout = new QHBoxLayout;
    for (int i=0; i<joyData->number_axes;i ++) {

        AxisConfiguration *axisConfig = new AxisConfiguration();

        axisConfig->value = new QLabel(QString("0"));
        axisConfig->value->setAlignment(Qt::AlignLeft);
        axisConfig->value->setFixedWidth(50);

        axisConfig->min = new QLabel(QString("0"));
        axisConfig->min->setAlignment(Qt::AlignLeft);
        axisConfig->min->setFixedWidth(50);

        axisConfig->max = new QLabel(QString("0"));
        axisConfig->max->setAlignment(Qt::AlignLeft);
        axisConfig->max->setFixedWidth(50);

        axisConfig->bar = new QProgressBar;
        axisConfig->bar->setOrientation(Qt::Vertical);
        axisConfig->bar->setMinimum(-1);
        axisConfig->bar->setMaximum(1);
        axisConfig->bar->setValue(0);
        axisConfig->bar->setAlignment(Qt::AlignRight);

        QVBoxLayout *tmpLayout = new QVBoxLayout();
        tmpLayout->addWidget(axisConfig->bar);
        tmpLayout->addWidget(axisConfig->value);
        tmpLayout->addWidget(axisConfig->min);
        tmpLayout->addWidget(axisConfig->max);

        axes_Layout->addLayout(tmpLayout);

        axis.append(axisConfig);
    }

    QHBoxLayout *buttons_Layout = new QHBoxLayout;
    for (int j=0; j<joyData->number_btn; j++) {
        QCheckBox *tempBox = new QCheckBox;
        tempBox->setChecked(false);
        tempBox->setDisabled(true);
        checklist.append(tempBox);
        buttons_Layout->addWidget(checklist[j]);
    }

    mainLayout->addLayout(axes_Layout);
    mainLayout->addStretch(1);
    mainLayout->addLayout(buttons_Layout);
    mainLayout->addStretch(1);

    btnStart = new QPushButton;
    btnStart->setText("Start Calibration");
    connect(btnStart, SIGNAL(clicked()), this, SLOT(startCalibration()));

    btnClose = new QPushButton;
    btnClose->setText("Close");
    connect(btnClose, SIGNAL(clicked()), this, SLOT(close()));


    QHBoxLayout *actions_layout = new QHBoxLayout;
    actions_layout->addWidget(btnStart);
    actions_layout->addWidget(btnClose);

    mainLayout->addLayout(actions_layout);

    this->setLayout(mainLayout);
}
