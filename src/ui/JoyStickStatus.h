#ifndef JOYSTICKSTATUS_H
#define JOYSTICKSTATUS_H

#include <QDockWidget>
#include <QList>


#include "input/Joystick.h"

namespace Ui {
class JoyStickStatus;
}

class JoyStickStatus : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit JoyStickStatus(QWidget *parent = 0);
    ~JoyStickStatus();

private slots:
    void joystickChanged(QList<int>,QList<double>, QList<bool>);
    
private:
    Ui::JoyStickStatus *ui;

    Joystick* joystick;
};

#endif // JOYSTICKSTATUS_H
