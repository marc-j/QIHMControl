#include "UAV.h"

#include "protocol/message/Messages.h"

UAV* UAV::instance()
{
    static UAV* _instance = 0;
    if(_instance == 0)
    {
        _instance = new UAV();
    }
    return _instance;
}

UAV::UAV(QObject *parent) :
    QObject(parent),
    pitch(0.0),
    roll(0.0),
    yaw(0.0),
    latitude(0.0),
    longitude(0.0),
    altitude(0.0),
    systemIsArmed(false)
{
    readSettings();

    protocol = Protocol::instance();
    connect(protocol, SIGNAL(receiveMessage(protocol_message_t)), this, SLOT(receiveMessage(protocol_message_t)));

    //emit disarmed();
}

UAV::~UAV()
{
    saveSettings();
}

void UAV::updateRollPID(float kP, float kI, float kD)
{
    rollPID.kP = kP;
    rollPID.kI = kI;
    rollPID.kD = kD;
}

void UAV::updatePitchPID(float kP, float kI, float kD)
{
    pitchPID.kP = kP;
    pitchPID.kI = kI;
    pitchPID.kD = kD;
}

void UAV::updateYawPID(float kP, float kI, float kD)
{
    yawPID.kP = kP;
    yawPID.kI = kI;
    yawPID.kD = kD;
}

void UAV::receiveMessage(protocol_message_t msg)
{
    switch(msg.cmd){
        case PROTOCOL_MSG_SYSTEM:
            protocol_message_system_t system;
            protocol_message_system_decode(&msg,&system);
            cpuLoad = system.cpuLoad/1000.0f;
            emit cpuLoadChange(cpuLoad);

            mainLoop = system.mainLoopTime/1000.0f;
            emit mainLoopChange(mainLoop);

            batteryVoltage = system.batteryVoltage/1000.0f;
            emit batteryVoltageChange(batteryVoltage);

            break;
    }
}


void UAV::saveSettings()
{
    settings.beginGroup("PID");
    settings.setValue("ROLL_KP",rollPID.kP);
    settings.setValue("ROLL_KI",rollPID.kI);
    settings.setValue("ROLL_KD",rollPID.kD);

    settings.setValue("PITCH_KP",pitchPID.kP);
    settings.setValue("PITCH_KI",pitchPID.kI);
    settings.setValue("PITCH_KD",pitchPID.kD);

    settings.setValue("YAW_KP",yawPID.kP);
    settings.setValue("YAW_KI",yawPID.kI);
    settings.setValue("YAW_KD",yawPID.kD);
    settings.endGroup();

    settings.sync();
}

void UAV::readSettings()
{
    settings.beginGroup("PID");
    rollPID.kP = settings.value("ROLL_KP", 0.0f).toFloat();
    rollPID.kI = settings.value("ROLL_KI", 0.0f).toFloat();
    rollPID.kD = settings.value("ROLL_KD", 0.0f).toFloat();

    pitchPID.kP = settings.value("PITCH_KP", 0.0f).toFloat();
    pitchPID.kI = settings.value("PITCH_KI", 0.0f).toFloat();
    pitchPID.kD = settings.value("PITCH_KD", 0.0f).toFloat();

    yawPID.kP = settings.value("YAW_KP", 0.0f).toFloat();
    yawPID.kI = settings.value("YAW_KI", 0.0f).toFloat();
    yawPID.kD = settings.value("YAW_KD", 0.0f).toFloat();
    settings.endGroup();

}
