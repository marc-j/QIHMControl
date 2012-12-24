#ifndef PIDBOX_H
#define PIDBOX_H

#include <QDockWidget>
#include <QSettings>

#include "UAV.h"

namespace Ui {
class PIDBox;
}

class PIDBox : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit PIDBox(QWidget *parent = 0);
    ~PIDBox();

signals:
    void pitchPIDchanged(float kP, float kI, float kD);
    void rollPIDchanged(float kP, float kI, float kD);
    void yawPIDchanged(float kP, float kI, float kD);

protected slots:
    void savePID();

protected:
    UAV* uav;
    QSettings settings;
    void readSettings();
    
private:
    Ui::PIDBox *ui;
};

#endif // PIDBOX_H
