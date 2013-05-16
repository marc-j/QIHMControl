#include "SensorsCalibration.h"
#include "ui_SensorsCalibration.h"
#include "math.h"

SensorsCalibration::SensorsCalibration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorsCalibration),
    logFile("datas/calibration.csv"),
    sampling(false),
    countAquisition(0)
{
    ui->setupUi(this);

    uav = UAV::instance();

    maxX = maxY = maxZ = 200;
    minX = minY = minZ = -200;

    /*plotXY = new QCustomPlot(this);
    plotYZ = new QCustomPlot(this);
    plotZX = new QCustomPlot(this);*/

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);


    QVBoxLayout* layoutXY = new QVBoxLayout;
    layoutXY->addWidget(imageLabel);
    ui->sensorsXY->setLayout(layoutXY);

    QVBoxLayout* layoutYZ = new QVBoxLayout;
    //layoutYZ->addWidget(plotYZ);
    ui->sensorsYZ->setLayout(layoutYZ);

    QVBoxLayout* layoutZX = new QVBoxLayout;
    //layoutZX->addWidget(plotZX);
    ui->sensorsZX->setLayout(layoutZX);

    /*plotXY->setColor(Qt::darkGray);
    plotXY->yAxis->setRange(-acc_range, acc_range);
    plotXY->xAxis->setRange(-acc_range, acc_range);
    plotXY->addGraph();
    plotXY->graph(0)->setPen(QPen(Qt::red));
    plotXY->graph(0)->setAntialiasedFill(false);

    plotYZ->setColor(Qt::darkGray);
    plotYZ->yAxis->setRange(-acc_range, acc_range);
    plotYZ->xAxis->setRange(-acc_range, acc_range);
    plotYZ->addGraph();
    plotYZ->graph(0)->setPen(QPen(Qt::green));
    plotYZ->graph(0)->setAntialiasedFill(false);

    plotZX->setColor(Qt::darkGray);
    plotZX->yAxis->setRange(-acc_range, acc_range);
    plotZX->xAxis->setRange(-acc_range, acc_range);
    plotZX->addGraph();
    plotZX->graph(0)->setPen(QPen(Qt::blue));
    plotZX->graph(0)->setAntialiasedFill(false);*/

   // logFile.setFileName("acc_raw.log");
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text )){
        //
    }

    connect(ui->btnSampling, SIGNAL(clicked()), this, SLOT(startSampling()));
    connect(ui->sensorsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(sensorChange(int)));
    connect(uav, SIGNAL(accRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
}

SensorsCalibration::~SensorsCalibration() {

    delete ui;
}

void SensorsCalibration::rawChange(int16_t x, int16_t y, int16_t z) {
    datas[0].append((double) x);
    datas[1].append((double) y);
    datas[2].append((double) z);

    if( sampling ) {
        countAquisition++;
        ui->lineSampling->setText(QString::number(countAquisition));
        QTextStream s(&logFile);
        s << QString().sprintf("%d,%d,%d\n",x,y,z);

        generatePlot(logFile.fileName());
    }
}

void SensorsCalibration::compassChange(int16_t accX, int16_t accY, int16_t accZ, int16_t magX, int16_t magY, int16_t magZ)
{
    float accXG = accX / 16384.0f;
    float accYG = accY / 16384.0f;
    float accZG = accZ / 16384.0f;

    minX = std::min(magX, minX);
    minY = std::min(magY, minY);
    minZ = std::min(magZ, minZ);
    maxX = std::max(magX, maxX);
    maxY = std::max(magY, maxY);
    maxZ = std::max(magZ, maxZ);

    float offsetX = ((float)minX + (float)maxX) / 2.0f;
    float offsetY = ((float)minY + (float)maxY) / 2.0f;
    float offsetZ = ((float)minZ + (float)maxZ) / 2.0f;

    /*magX -= offsetX;
    magY -= offsetY;
    magZ -= offsetZ;*/

    /*
        xt_raw = x_raw - offsetx;
        yt_raw = y_raw - offsety;
        zt_raw = z_raw - offsetz;
        x_calibrated = scalefactor_x[1] * xt_raw + scalefactor_x[2] * yt_raw + scalefactor_x[3] * zt_raw;
        y_calibrated = scalefactor_y[1] * xt_raw + scalefactor_y[2] * yt_raw + scalefactor_y[3] * zt_raw;
        z_calibrated = scalefactor_z[1] * xt_raw + scalefactor_z[2] * yt_raw + scalefactor_z[3] * zt_raw;
    */

    float magXC = (magX - offsetX) / 1090.0f; // - offsetX;// (magX - minX) / (maxX - minX) * 2.0f - 1.0f;
    float magYC = (magY - offsetY) / 1090.0f; // (magY - minY) / (maxY - minY) * 2.0f - 1.0f;
    float magZC = (magZ - offsetZ) / 1090.0f; // (magZ - minZ) / (maxZ - minZ) * 2.0f - 1.0f;

    /*magXC = 0.927567 * magXC + 0.000568 * magYC + 0.000629 * magZC;
    magYC = 0.000568 * magXC + 0.954393 * magYC + 0.020703 * magZC;
    magZC = 0.000629 * magXC + 0.020703 * magYC + 1.086950 * magZC;*/

    float heading = atan2(magYC, magXC) * 57.29f;

    float roll = atan2(accXG, accZG);// * 57.29f; // - 90°
    float pitch = atan2(accYG, accZG);// * 57.29f; // -90°

    //float Xh = (magXC * cos(pitch)) + (magZC * sin(pitch));
    //float Yh = (magXC * sin(roll) * sin(pitch)) + (magYC * cos(roll)) - (magZC * sin(roll) * cos(pitch));

    //float Xh = (magXC * cos(pitch)) + (magYC * sin(pitch) * sin(roll)) - (magZC * sin(pitch) * cos(roll));
    //float Yh = (magYC * cos(roll)) + (magZC * sin(roll));
    float Zh = (magXC * sin(pitch)) - (magYC * sin(roll) * cos(pitch)) + (magZC * cos(roll) * cos(pitch));

    //Xh = bx * cos(theta) + by * sin(phi) * sin(theta) + bz * cos(phi) * sin(theta)
    //Yh = by * cos(phi) - bz * sin(phi)
    float Xh = (magXC * cos(roll)) + (magYC * sin(pitch) * sin(roll)) - (magZC * cos(pitch) * sin(roll));
    float Yh = (magYC * cos(pitch)) - (magZC * sin(pitch));

    float tiltheading = atan2(Yh, Xh) * 57.29f;
    /*if (Xh < 0 ){
        tiltheading += 180;
    } else if (Xh > 0 && Yh <= 0) {
        tiltheading += 360;
    } else if (Xh == 0 && Yh < 0) {
        tiltheading = 90;
    } else if (Xh == 0 && Yh > 0) {
        tiltheading = 270;
    }*/

    ui->offsetX->setText(QString::number(offsetX));
    ui->offsetY->setText(QString::number(offsetY));
    ui->offsetZ->setText(QString::number(offsetZ));
    ui->scaleX->setText(QString::number(accXG));
    ui->scaleY->setText(QString::number(accYG));
    ui->minX->setText(QString::number(Xh));
    ui->minY->setText(QString::number(Yh));
    ui->maxX->setText(QString::number(magXC));
    ui->maxY->setText(QString::number(magYC));

    if( sampling ) {
        countAquisition++;
        ui->lineSampling->setText(QString::number(countAquisition));
        QTextStream s(&logFile);
        s << QString().sprintf("%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f\n",roll,pitch,accZG, magXC, magYC, magZC, Xh, Yh, Zh);

        generatePlot(logFile.fileName());
    }
}

void SensorsCalibration::calibrate()
{
    int sMin;
    int sMax;
    float gravity = ui->oneG->text().toFloat();
    float offset, scale;

    for (int i=0;i<3;i++) {
        sMin = 999999999999.0;
        sMax = 0;
        for (int j=0;j<datas[i].size();j++) {
            sMin = std::min(sMin, (int) datas[i].at(j));
            sMax = std::max(sMax, (int) datas[i].at(j));
        }

        offset = ((float)sMin + (float)sMax) / 2.0f;
        scale = gravity / ( ((float)sMax) - offset);

        //oneG = (scaleX(sensorX - offsetX))² + (scaleY(sensorY - offsetY))² + (scaleZ(sensorZ - offsetZ))² = 9.81²

        switch (i) {
            case 0: //X
                ui->minX->setText( QString::number(sMin) );
                ui->maxX->setText( QString::number(sMax) );
                ui->offsetX->setText( QString::number(offset) );
                ui->scaleX->setText( QString::number(scale) );
                break;
            case 1: //Y
                ui->minY->setText( QString::number(sMin) );
                ui->maxY->setText( QString::number(sMax) );
                ui->offsetY->setText( QString::number(offset) );
                ui->scaleY->setText( QString::number(scale) );
                break;
            case 2: //Z
                ui->minZ->setText( QString::number(sMin) );
                ui->maxZ->setText( QString::number(sMax) );
                ui->offsetZ->setText( QString::number(offset) );
                ui->scaleZ->setText( QString::number(scale) );
                break;
        }
    }
}

void SensorsCalibration::generatePlot(QString file)
{
    QFile datasFile(file);
    if (!datasFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Unable to open file " << file;

        return;
    }
    datasFile.close();

    QProcess gnuplot;
    gnuplot.start("gnuplot");
    if (!gnuplot.waitForStarted()) return;
    gnuplot.write("set datafile separator ','\n");
    gnuplot.write("unset key\n");
    gnuplot.write("set terminal png\n");
    gnuplot.write("set output 'datas/plots.png'\n");
    gnuplot.write(QString("splot '" + file + "' using 4:5:6 with lines\n").toAscii());
    gnuplot.write("q\n");
    gnuplot.closeWriteChannel();
    if (!gnuplot.waitForFinished())  return;

    QImage image("datas/plots.png");
    imageLabel->setPixmap(QPixmap::fromImage(image));

}

void SensorsCalibration::startSampling()
{
    sampling = ui->btnSampling->text() == "Sampling";
    if (sampling) {
        countAquisition = 0;
        logFile.close();
        logFile.open(QIODevice::WriteOnly | QIODevice::Text  | QFile::Truncate);
        ui->btnSampling->setText("Stop");
    } else {
        ui->btnSampling->setText("Sampling");
        //calibrate();
    }
}

void SensorsCalibration::sensorChange(int i)
{
    if (sampling) {
        startSampling();
    }
   /* plotXY->graph(0)->clearData();
    plotYZ->graph(0)->clearData();
    plotZX->graph(0)->clearData();*/

    /*datas[0].clear();
    datas[1].clear();
    datas[2].clear();

    plotXY->replot();
    plotYZ->replot();
    plotZX->replot();*/

    if (i == 0) {
        //acc
        disconnect(uav, SIGNAL(compassRawChange(int16_t,int16_t,int16_t,int16_t,int16_t,int16_t)), this, SLOT(compassChange(int16_t,int16_t,int16_t,int16_t,int16_t,int16_t)));
        disconnect(uav, SIGNAL(magRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
        connect(uav, SIGNAL(accRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
    } else if (i == 1) {
        disconnect(uav, SIGNAL(compassRawChange(int16_t,int16_t,int16_t,int16_t,int16_t,int16_t)), this, SLOT(compassChange(int16_t,int16_t,int16_t,int16_t,int16_t,int16_t)));
        disconnect(uav, SIGNAL(accRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
        connect(uav, SIGNAL(magRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
    } else {
        disconnect(uav, SIGNAL(accRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
        disconnect(uav, SIGNAL(magRawChange(int16_t,int16_t,int16_t)), this, SLOT(rawChange(int16_t,int16_t,int16_t)));
        connect(uav, SIGNAL(compassRawChange(int16_t,int16_t,int16_t,int16_t,int16_t,int16_t)), this, SLOT(compassChange(int16_t,int16_t,int16_t,int16_t,int16_t,int16_t)));
    }
}
