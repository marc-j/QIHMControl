#ifndef SYSTEMSTATUS_H
#define SYSTEMSTATUS_H

#include <QDockWidget>

#include "UAV.h"

namespace Ui {
class SystemStatus;
}

class SystemStatus : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit SystemStatus(QWidget *parent = 0);
    ~SystemStatus();

public slots:
    void mainLoopChange(double);
    void cpuLoadChange(double);
    void batteryVoltageChange(double);
    
private:
    Ui::SystemStatus *ui;

    UAV* uav;
};

#endif // SYSTEMSTATUS_H
