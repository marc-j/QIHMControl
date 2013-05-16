#ifndef SENSORSVARIANCE_H
#define SENSORSVARIANCE_H

#include <QWidget>
#include "UAV.h"

namespace Ui {
class SensorsVariance;
}

class SensorsVariance : public QWidget
{
    Q_OBJECT
    
public:
    explicit SensorsVariance(QWidget *parent = 0);
    ~SensorsVariance();
    
private slots:
    void sensorsVariance(float accX, float accY, float accZ, float gyroX, float gyroY, float gyroZ);
    void startCalculateVariance();

private:
    Ui::SensorsVariance *ui;
    UAV* uav;
};

#endif // SENSORSVARIANCE_H
