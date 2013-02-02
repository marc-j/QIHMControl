#ifndef SENSORSCALIBRATION_H
#define SENSORSCALIBRATION_H

#include <QWidget>
#include "ui/QtPlot/qcustomplot.h"
#include "UAV.h"
#include <QFile>
#include <QTextStream>

namespace Ui {
class SensorsCalibration;
}

class SensorsCalibration : public QWidget
{
    Q_OBJECT
    
public:
    explicit SensorsCalibration(QWidget *parent = 0);
    ~SensorsCalibration();
    
private slots:
    void rawChange(int16_t, int16_t, int16_t);
    void startSampling();
    void sensorChange(int);

private:
    Ui::SensorsCalibration *ui;

    QCustomPlot *plotXY;
    QCustomPlot *plotYZ;
    QCustomPlot *plotZX;

    QVector<double> datas[3];

    UAV* uav;
    QFile logFile;
    QTextStream fileStream;
    bool sampling;

};

#endif // SENSORSCALIBRATION_H
