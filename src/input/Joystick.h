#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QObject>
#include <QString>
#include <QList>
#include <QTimer>

#include "SDL/SDL.h"

#define JOY_CENTER 0
#define JOY_MAX 90
#define JOY_MIN -90

#define mapNumber(value, min, max) ( (value-min)*(JOY_MAX-JOY_MIN)/(max-min)+JOY_MIN )

#define JOY_PPM_MIN 1000
#define JOY_PPM_MAX 2000
#define mapNumberPPM(value, min, max) ( (value-min)*(JOY_PPM_MAX-JOY_PPM_MIN)/(max-min)+JOY_PPM_MIN )

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

#define XAXIS 0
#define YAXIS 1
#define ZAXIS 2
#define TAXIS 3

#define BTN_ARMED 4

class Joystick : public QObject
{
    Q_OBJECT
public:
    static Joystick* instance();
    ~Joystick();

    struct joydata{
        QString name;
        unsigned int number_axes;
        unsigned int number_btn;
        QList<int> axisNorm;
        QList<double> axis;
        QList<bool> button;
        struct {
            QList<double> axisMin;
            QList<double> axisMax;
            QList<int> inversed;
            QList<double> offset;
            QList<bool> hasOffset;
        } joyCalibration;
    };

    int availableJoysticks();
    int currentJoystick();
    QString joystickName(int id);
    int joystickNumAxes();
    int joystickNumButtons();
    joydata* getCurrentJoystick();

    void startCalibration();
    void stopCalibration();
    bool calibrationIsStarted();

    QList<joydata*> getJoysticks();
    
signals:
    void joystickStarted(bool);
    void joystickChanged(QList<int>, QList<double>, QList<bool>);

public slots:
    void setJoystick(int jsNumber);
    void startPolling();
    void stopPolling();

private slots:
    void updateData();

private:
    explicit Joystick(QObject *parent = 0);
    SDL_Joystick* m_joystick;

    QList<joydata*> joystickList;
    int joyAvailable;
    int current_joystick;

    QTimer* pollingTimer;
    bool calibrationStarted;
    double deadZone;

    void init();

};

#endif // JOYSTICK_H
