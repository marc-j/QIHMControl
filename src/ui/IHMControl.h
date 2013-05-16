#ifndef IHMCONTROL_H
#define IHMCONTROL_H

#include <QSettings>
#include <QMainWindow>
#include <QStackedWidget>
#include <QActionGroup>
#include <QStatusBar>
#include <QPointer>
#include <QPushButton>
#include <QComboBox>

#include "ui/SystemStatus.h"
#include "ui/JoyStickStatus.h"
#include "ui/MotorStatus.h"
#include "ui/PIDBox.h"
#include "ui/GaugesView.h"
#include "ui/Led.h"
#include "ui/Cube3D.h"
#include "ui/UAVView.h"
#include "ui/Console.h"
#include "ui/Plot.h"
#include "ui/HUD.h"

#include "protocol/Protocol.h"
#include "input/Joystick.h"

#include "UAV.h"

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
    void addCentralWidget(QWidget* widget, const QString& title, QIcon icon);
    QString getWindowStateKey();
    QString getWindowGeometryKey();

public slots:
    void showSerialConfiguration();
    void showJoystickConfiguration();
    void showSensorsCalibration();
    void showSensosrVariance();
    void closeEvent(QCloseEvent* event);
    void showCentralWidget();
    void showCriticalMessage(const QString& title, const QString& message);
    void showInfoMessage(const QString& title, const QString& message);
    void showStatusMessage(const QString& status, int timeout);
    void showStatusMessage5s(const QString& status);
    void showStatusMessage(const QString& status);

private slots:
    void flightModeChange(int);

protected:
    IHMControl(QWidget *parent = 0);

    UAV* uav;
    Protocol* protocol;
    Joystick* joystick;

    QSettings settings;
    QStackedWidget *centerStack;
    QActionGroup* centerStackActionGroup;
    QPointer<SystemStatus> systemStatus;
    QPointer<JoyStickStatus> joystickStatus;
    QPointer<MotorStatus> motorStatus;
    QPointer<PIDBox> pidBox;
    QPointer<GaugesView> gaugesView;

    QString styleFileName;

    Led* serialStatus;
    Led* joyStickStatus;
    Led* armedStatus;

    Cube3D* cube3d;
    UAVView* uavView;
    Console* console;
    Plot* plot;
    HUD* hud;

    QComboBox* flightMode;

    void loadSettings();
    void saveSettings();
    void generateToolBar();
};

#endif // IHMCONTROL_H
