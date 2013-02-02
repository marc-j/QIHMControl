#include "UAV.h"

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
    connect(protocol, SIGNAL(receiveMessage(uavlink_message_t)), this, SLOT(receiveMessage(uavlink_message_t)));

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

void UAV::receiveMessage(uavlink_message_t msg)
{
    switch(msg.cmd){
        case UAVLINK_MSG_SYSTEM:
            uavlink_message_system_t system;
            uavlink_message_system_decode(&msg,&system);
            cpuLoad = system.cpuLoad/1000.0f;
            emit cpuLoadChange(cpuLoad);

            mainLoop = system.mainLoopTime/1000.0f;
            emit mainLoopChange(mainLoop);

            batteryVoltage = system.batteryVoltage/1000.0f;
            emit batteryVoltageChange(batteryVoltage);

            break;
        case UAVLINK_MSG_SENSOR:
            uavlink_message_sensor_t sensor;
            uavlink_message_sensor_decode(&msg,&sensor);

            accX = sensor.accX/1000.0f;
            accY = sensor.accY/1000.0f;
            accZ = sensor.accZ/1000.0f;
            gyroX = sensor.gyroX/10.0f;
            gyroY = sensor.gyroY/10.0f;
            gyroZ = sensor.gyroZ/10.0f;
            magX = sensor.magX/10.0f;
            magY = sensor.magY/10.0f;
            magZ = sensor.magZ/10.0f;
            roll = sensor.roll/10.0f;
            pitch = sensor.pitch/10.0f;
            yaw = sensor.yaw/10.0f;

            emit updateValue(ACCX, accX);
            emit updateValue(ACCY, accY);
            emit updateValue(ACCZ, accZ);
            emit updateValue(GYROX, gyroX);
            emit updateValue(GYROY, gyroY);
            emit updateValue(GYROZ, gyroZ);
            emit updateValue(MAGX, magX);
            emit updateValue(MAGY, magY);
            emit updateValue(MAGZ, magZ);
            emit updateValue(ROLL, roll);
            emit updateValue(PITCH, pitch);
            emit updateValue(YAW, yaw);


            emit accChange(accX, accY, accZ);
            emit gyroChange(gyroX, gyroY, gyroZ);
            emit eulerChange(roll, pitch, yaw);
            break;
        case UAVLINK_MSG_MOTOR:
            uavlink_message_motor_t motor;
            uavlink_message_motor_decode(&msg,&motor);

            emit motorChange(motor.motorFrontLeft, motor.motorFrontRight, motor.motorRearLeft, motor.motorRearRight);
            break;
        case UAVLINK_MSG_SENSOR_RAW:
            uavlink_message_sensor_raw_t sensorRaw;
            uavlink_message_sensor_raw_decode(&msg,&sensorRaw);

            emit accRawChange(sensorRaw.accX, sensorRaw.accY, sensorRaw.accZ);
            emit gyroRawChange(sensorRaw.gyroX, sensorRaw.gyroY, sensorRaw.gyroZ);
            emit magRawChange(sensorRaw.magX, sensorRaw.magY, sensorRaw.magZ);
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
