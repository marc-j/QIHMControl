#ifndef MOTORSTATUS_H
#define MOTORSTATUS_H

#include <QDockWidget>

namespace Ui {
class MotorStatus;
}

class MotorStatus : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit MotorStatus(QWidget *parent = 0);
    ~MotorStatus();
    
private:
    Ui::MotorStatus *ui;
};

#endif // MOTORSTATUS_H
