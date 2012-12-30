#ifndef JOYSTICKCONFIGURATION_H
#define JOYSTICKCONFIGURATION_H

#include <QWidget>
#include <QList>

#include "input/Joystick.h"

namespace Ui {
class JoystickConfiguration;
}

class JoystickConfiguration : public QWidget
{
    Q_OBJECT
    
public:
    explicit JoystickConfiguration(QWidget *parent = 0);
    ~JoystickConfiguration();
    
private slots:
    void onSave();
    void updateSelection(int index);

private:
    Ui::JoystickConfiguration *ui;

    Joystick* joystick;

    void init();
};

#endif // JOYSTICKCONFIGURATION_H
