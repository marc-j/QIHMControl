#ifndef JOYSTICKCALIBRATION_H
#define JOYSTICKCALIBRATION_H

#include <QWidget>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include "input/Joystick.h"

class JoystickCalibration : public QWidget
{
    Q_OBJECT
public:
    explicit JoystickCalibration(QWidget *parent = 0);
    
signals:
    
public slots:

private slots:
    void joystickChanged(QList<int>, QList<double>, QList<bool>);
    void startCalibration();

private:

    Joystick* joystick;
    Joystick::joydata* joyData;

    QVBoxLayout *mainLayout;
    QList<QProgressBar*> pbarlist;
    QList<QLabel*> pbarlablist;
    QList<QCheckBox*> checklist;

    struct AxisConfiguration {
        QProgressBar* bar;
        QLabel* value;
        QLabel* min;
        QLabel* max;
    };

    QList<AxisConfiguration*> axis;

    QPushButton *btnStart;
    QPushButton *btnClose;

    void initUI();
    
};

#endif // JOYSTICKCALIBRATION_H
