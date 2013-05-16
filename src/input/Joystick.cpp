#include "Joystick.h"

#include <QDebug>

#define POLL_INTERVAL 20

Joystick* Joystick::instance()
{
    static Joystick* _instance = 0;
    if(_instance == 0)
    {
        _instance = new Joystick();
    }
    return _instance;
}

Joystick::Joystick(QObject *parent) :
    QObject(parent),
    calibrationStarted(false),
    deadZone(0.1f)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

    current_joystick = 0;
    m_joystick = SDL_JoystickOpen(current_joystick);


    init();

    pollingTimer = new QTimer();
    pollingTimer->setInterval(POLL_INTERVAL);

    connect(pollingTimer, SIGNAL(timeout()), this, SLOT(updateData()));
}

Joystick::~Joystick()
{
    SDL_JoystickClose(m_joystick);
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
}

int Joystick::currentJoystick()
{
    return SDL_JoystickIndex(m_joystick);
}

QString Joystick::joystickName(int js)
{
    Q_ASSERT(js < availableJoysticks());
    Q_ASSERT(js >= 0);
    return QString(SDL_JoystickName(js));
}

int Joystick::joystickNumAxes()
{
    Q_ASSERT(js < availableJoysticks());
    Q_ASSERT(js >= 0);
    return (SDL_JoystickNumAxes(m_joystick));
}

int Joystick::joystickNumButtons()
{
    Q_ASSERT(js < availableJoysticks());
    Q_ASSERT(js >= 0);
    return (SDL_JoystickNumButtons(m_joystick));
}

void Joystick::setJoystick(int js)
{
    Q_ASSERT(js < availableJoysticks());
    Q_ASSERT(js >= 0);

    SDL_JoystickClose(m_joystick);
    m_joystick = SDL_JoystickOpen(js);
    current_joystick = js;
}

int Joystick::availableJoysticks()
{
     return SDL_NumJoysticks();
}

QList<Joystick::joydata*> Joystick::getJoysticks()
{
    return joystickList;
}

Joystick::joydata* Joystick::getCurrentJoystick()
{
    return joystickList.at(currentJoystick());
}



void Joystick::init()
{
    joyAvailable = availableJoysticks();

    for (int i=0; i<joyAvailable; i++) {
        joydata *tmpjoy;
        tmpjoy = new joydata;
        setJoystick(i);

        tmpjoy->name = joystickName(i);
        // Axis
        tmpjoy->number_axes = joystickNumAxes();

        for(unsigned int j=0; j<tmpjoy->number_axes;j++)
        {
            tmpjoy->axis.append(0);
            tmpjoy->axisNorm.append(0);
            tmpjoy->joyCalibration.axisMax.append(32767.0f);
            tmpjoy->joyCalibration.axisMin.append(-32767.0f);
            tmpjoy->joyCalibration.inversed.append(1);

        }

        // Buttons
        tmpjoy->number_btn = joystickNumButtons();

        for(unsigned int j=0; j<tmpjoy->number_btn;j++)
        {
            tmpjoy->button.append(false);
        }

        joystickList.append(tmpjoy);
    }

    if (joyAvailable > 0) {
        setJoystick(current_joystick);
    }
}

void Joystick::updateData()
{
    joydata* data = joystickList.at(current_joystick);

    SDL_Event event;
    SDL_PollEvent(&event);

    for(int i=0;i<SDL_JoystickNumAxes(m_joystick);i++)
    {
        int axisValue = SDL_JoystickGetAxis(m_joystick, i);
        if(calibrationStarted) {

           /* double min = MIN(data->joyCalibration.axisMin.at(i), (double)axisValue);
            data->joyCalibration.axisMin[i] = min;

            double max = MAX(data->joyCalibration.axisMax.at(i), (double)axisValue);
            data->joyCalibration.axisMax[i] = max;*/
        }

        double axis = ((double)axisValue - data->joyCalibration.axisMin.at(i)) / (data->joyCalibration.axisMax.at(i) - data->joyCalibration.axisMin.at(i));
        axis = 1.0f - axis;
        axis = axis * 2.0f - 1.0f;

        // Bound rounding errors
        if (axis > 1.0f) axis = 1.0f;
        if (axis < -1.0f) axis = -1.0f;

        axis = (axis <= -deadZone || deadZone <= axis) ? axis : 0.0f;

        if (i == TAXIS) {
            data->axisNorm[i] = mapNumberPPM(axis , -1, 1);
        } else {
            data->axisNorm[i] = mapNumber(axis , -1, 1);
        }
        data->axisNorm[i] *= data->joyCalibration.inversed.at(i);

        data->axis[i] = axis;
    }

    for(int i=0;i<SDL_JoystickNumButtons(m_joystick);i++)
    {
        data->button[i] = SDL_JoystickGetButton(m_joystick,i);
    }

    emit joystickChanged(data->axisNorm, data->axis, data->button);
}


void Joystick::startCalibration()
{
    joydata *joyData = getCurrentJoystick();

    for(int i=0;i<SDL_JoystickNumAxes(m_joystick);i++)
    {
        joyData->joyCalibration.axisMin[i] = 32767.0f;
        joyData->joyCalibration.axisMax[i] = -32767.0f;
    }

    calibrationStarted = true;
}

void Joystick::stopCalibration()
{
    calibrationStarted = false;
}

bool Joystick::calibrationIsStarted()
{
    return calibrationStarted;
}

void Joystick::startPolling()
{
    setJoystick(0);
    if (availableJoysticks() > 0) {
        pollingTimer->start();
        emit joystickStarted(true);
    } else {
        pollingTimer->stop();
        emit joystickStarted(false);
    }
}

void Joystick::stopPolling()
{
    pollingTimer->stop();
    emit joystickStarted(false);
}
