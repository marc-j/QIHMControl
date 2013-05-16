#ifndef SENSORSCALIBRATION_H
#define SENSORSCALIBRATION_H

#include <QWidget>
#include "ui/QtPlot/qcustomplot.h"
#include "UAV.h"
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QLabel>

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
    void compassChange(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
    void startSampling();
    void sensorChange(int);

private:
    void calibrate();
    Ui::SensorsCalibration *ui;

    void generatePlot(QString file);

    int16_t maxX;
    int16_t maxY;
    int16_t maxZ;
    int16_t minX;
    int16_t minY;
    int16_t minZ;
    /*QCustomPlot *plotXY;
    QCustomPlot *plotYZ;
    QCustomPlot *plotZX;*/
    QLabel *imageLabel;
    int countAquisition;

    QVector<double> datas[3];

    UAV* uav;
    QFile logFile;
    QTextStream fileStream;
    bool sampling;

};

#endif // SENSORSCALIBRATION_H
