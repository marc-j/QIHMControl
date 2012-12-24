#ifndef IHMCONTROL_H
#define IHMCONTROL_H

#include <QSettings>
#include <QMainWindow>
#include <QStackedWidget>
#include <QStatusBar>
#include <QPointer>

#include "ui/SystemStatus.h"
#include "ui/JoyStickStatus.h"
#include "ui/MotorStatus.h"
#include "ui/PIDBox.h"
#include "ui/GaugesView.h"

namespace Ui {
class IHMControl;
}

class IHMControl : public QMainWindow
{
    Q_OBJECT
    
public:
    static IHMControl* instance();
    ~IHMControl();
    
private:
    Ui::IHMControl *ui;

    /** @brief Reload the CSS style sheet */
    void loadStylesheet();
    QString getWindowStateKey();
    QString getWindowGeometryKey();

public slots:
    void showSerialConfiguration();
    void closeEvent(QCloseEvent* event);

protected:
    IHMControl(QWidget *parent = 0);

    QSettings settings;
    QStackedWidget *centerStack;
    QPointer<SystemStatus> systemStatus;
    QPointer<JoyStickStatus> joystickStatus;
    QPointer<MotorStatus> motorStatus;
    QPointer<PIDBox> pidBox;
    QPointer<GaugesView> gaugesView;

    QString styleFileName;

    void loadSettings();
    void saveSettings();
};

#endif // IHMCONTROL_H
