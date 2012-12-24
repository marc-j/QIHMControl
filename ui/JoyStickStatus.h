#ifndef JOYSTICKSTATUS_H
#define JOYSTICKSTATUS_H

#include <QDockWidget>

namespace Ui {
class JoyStickStatus;
}

class JoyStickStatus : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit JoyStickStatus(QWidget *parent = 0);
    ~JoyStickStatus();
    
private:
    Ui::JoyStickStatus *ui;
};

#endif // JOYSTICKSTATUS_H
