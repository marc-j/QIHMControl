#ifndef UAV_H
#define UAV_H

#include <QSettings>
#include <QDebug>
#include <QTimer>

#include "protocol/Protocol.h"
#include "input/Joystick.h"

#define mapValue(value, src_min, src_max, dst_min, dst_max) ( (value-src_min)*(dst_max-dst_min)/(src_max-src_min)+dst_min )

#define ACCX "ACCX"
#define ACCY "ACCY"
#define ACCZ "ACCZ"
#define GYROX "GYROX"
#define GYROY "GYROY"
#define GYROZ "GYROZ"
#define MAGX "MAGX"
#define MAGY "MAGY"
#define MAGZ "MAGZ"
#define ROLL "ROLL"
#define PITCH "PITCH"
#define YAW "YAW"

class UAV : public QObject
{
    Q_OBJECT
public:
    static UAV* instance();
    ~UAV();

    /** @brief Euler angle */
    double getPitch() const
    {
        return pitch;
    }
    double getRoll() const
    {
        return roll;
    }
    double getYaw() const
    {
        return yaw;
    }

    /** @brief GPS Position */
    double getLatitude() const
    {
        return latitude;
    }
    double getLongitude() const
    {
        return longitude;
    }
    double getAltitude() const
    {
        return altitude;
    }

    double getMainLoop() const
    {
        return mainLoop;
    }
    double getBatteryVoltage() const
    {
        return batteryVoltage;
    }
    double getCpuLoad() const
    {
        return cpuLoad;
    }

    bool isArmed() const
    {
        return systemIsArmed;
    }

    void sendGetVariance();
    void sendPID();

    enum {
        FLIGHTMODE_WAITING,
        FLIGHTMODE_VARIANCE,
        FLIGHTMODE_COMPASS_CALIBRATION,
        FLIGHTMODE_NORMAL
    };

protected:
    explicit UAV(QObject *parent = 0);

    struct PID {
        float kP;
        float kI;
        float kD;
    };

    double accX;
    double accY;
    double accZ;
    double gyroX;
    double gyroY;
    double gyroZ;
    double magX;
    double magY;
    double magZ;

    double pitch;
    double roll;
    double yaw;

    double latitude;
    double longitude;
    double altitude;

    bool systemIsArmed;
    uint8_t flightMode;
    bool lastBtnStatus;

    int commandRollAngle;
    int commandPitchAngle;
    int commandYawAngle;
    int commandThrust;

    double mainLoop;
    double batteryVoltage;
    double cpuLoad;

    struct PID rollPID;
    struct PID pitchPID;
    struct PID yawPID;

    QSettings settings;

    Protocol* protocol;
    Joystick* joystick;

    QTimer* poolTimer;

    void saveSettings();
    void readSettings();

signals:

    void armed();
    void disarmed();

    void cpuLoadChange(double);
    void mainLoopChange(double);
    void batteryVoltageChange(double);
    void accChange(double x, double y, double z);
    void gyroChange(double x, double y, double z);
    void eulerChange(double roll, double pitch, double yaw);

    void motorChange(int fl, int fr, int rl, int rr);

    void updateValue(QString name, float);
    void updateValue(QString name, double);
    void updateValue(QString name, int);

    void accRawChange(int16_t, int16_t, int16_t);
    void gyroRawChange(int16_t, int16_t, int16_t);
    void magRawChange(int16_t, int16_t, int16_t);
    void compassRawChange(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);

    void sensorsVariance(float accX, float accY, float accZ, float gyroX, float gyroY, float gyroZ);
    void sendMessage(uavlink_message_t);

    void pidRollKP(int i);
    void pidRollKI(int i);

public slots:
    void updateRollPID(float kP, float kI, float kD);
    void updatePitchPID(float kP, float kI, float kD);
    void updateYawPID(float kP, float kI, float kD);

    void receiveMessage(uavlink_message_t);
    void changeFlightMode(uint8_t mode);

    void joystickChanged(QList<int>,QList<double>, QList<bool>);

private slots:
    void timerHandle();

};

#endif // UAV_H
