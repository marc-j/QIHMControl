#ifndef MOTORSTATUS_H
#define MOTORSTATUS_H

#include <QDockWidget>


#include <UAV.h>

namespace Ui {
class MotorStatus;
}

class MotorStatus : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit MotorStatus(QWidget *parent = 0);
    ~MotorStatus();

public slots:
    void motorChange(int fl, int fr, int rl, int rr);
    
private:
    Ui::MotorStatus *ui;

    UAV* uav;
};

#endif // MOTORSTATUS_H
