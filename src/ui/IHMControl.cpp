#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QWebView>
#include <QApplication>
#include <QtDebug>
#include <QObjectList>
#include <QDockWidget>
#include <QWidget>

#include <QLabel>

#include "IHMControl.h"
#include "ui_IHMControl.h"
#include "ui/SerialConfiguration.h"
#include "ui/JoystickConfiguration.h"
#include "ui/SensorsCalibration.h"
#include "ui/SensorsVariance.h"
#include "ui/Cube3D.h"
#include "ui/UAVView.h"

IHMControl* IHMControl::instance()
{
    static IHMControl* _instance = 0;
    if(_instance == 0)
    {
        _instance = new IHMControl();
    }
    return _instance;
}

IHMControl::IHMControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IHMControl),
    styleFileName(QCoreApplication::applicationDirPath() + "/style-default.css"),
    centerStackActionGroup(new QActionGroup(this))
{

    loadSettings();

    ui->setupUi(this);
    hide();

    uav = UAV::instance();
    protocol = Protocol::instance();
    joystick = Joystick::instance();

    connect(protocol, SIGNAL(serialError(QString,QString)), this, SLOT(showCriticalMessage(QString,QString)));
    connect(protocol,SIGNAL(serialMessage(QString)), this, SLOT(showStatusMessage5s(QString)));


    // Set dock options
    setDockOptions(AnimatedDocks | AllowTabbedDocks | AllowNestedDocks);
    statusBar()->setSizeGripEnabled(true);

    // Setup corners
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    loadStylesheet();

    generateToolBar();

   /* QWebView *webView = new QWebView(this);
    webView->resize(1000,500);
    webView->move(10,10);
    QString gMapURL = "England"; // this is where you want to point
    gMapURL = "http://www.google.fr";
    webView->setUrl(QUrl(gMapURL));*/



    centerStackActionGroup->setExclusive(true);

    centerStack = new QStackedWidget(this);
    setCentralWidget(centerStack);

    hud = new HUD(this);
    addCentralWidget(hud,tr("HUD"), QIcon(":/files/images/gauge.png"));

    console = new Console(this);
    addCentralWidget(console,tr("Console"), QIcon(":/files/images/console.png"));

    plot = new Plot(this);
    addCentralWidget(plot,tr("Plots"), QIcon(":/files/images/analyser.png"));

    uavView = new UAVView(this);
    addCentralWidget(uavView, tr("3D"), QIcon(":/files/images/plane.png"));

    joystickStatus = new JoyStickStatus(this);
    addDockWidget(Qt::LeftDockWidgetArea, joystickStatus);

    systemStatus = new SystemStatus(this);
    addDockWidget(Qt::LeftDockWidgetArea, systemStatus);

    motorStatus = new MotorStatus(this);
    addDockWidget(Qt::LeftDockWidgetArea, motorStatus);


    QDockWidget* cube3dDock = new QDockWidget(tr("Cube 3D"),this);
    cube3d = new Cube3D(this);
    cube3dDock->setWidget(cube3d);
    cube3dDock->setObjectName("IHMCONTROL_CUBE3D_DOCK");
    addDockWidget(Qt::RightDockWidgetArea, cube3dDock);

   /* QDockWidget* uavViewDock = new QDockWidget(tr("UAV View"),this);
    uavView = new UAVView(this);
    uavViewDock->setWidget(uavView);
    uavViewDock->setObjectName("IHMCONTROL_UAVVIEW_DOCK");
    addDockWidget(Qt::RightDockWidgetArea, uavViewDock);*/

    pidBox = new PIDBox(this);
    addDockWidget(Qt::RightDockWidgetArea, pidBox);

    QDockWidget* gaugesView1Dock = new QDockWidget(tr("Gauges"),this);

    gaugesView = new GaugesView(tr("Gauges"),this);
    gaugesView1Dock->setWidget(gaugesView);
    gaugesView1Dock->setObjectName("IHMCONTROL_GAUGES_VIEW_DOCK1");
    addDockWidget(Qt::LeftDockWidgetArea, gaugesView1Dock);

    gaugesView->addGauge("ACCX",0.0f,-2.0f,2.0f);
    gaugesView->addGauge("ACCY",0,-2.0f,2.0f);
    gaugesView->addGauge("ACCZ",0,-2.0f,2.0f);
    gaugesView->addGauge("GYROX",0,-1000,1000);
    gaugesView->addGauge("GYROY",0,-1000,1000);
    gaugesView->addGauge("GYROZ",0,-1000,1000);
    gaugesView->addGauge("MAGX",0,-250.0f,250.0f);
    gaugesView->addGauge("MAGY",0,-250.0f,250.0f);
    gaugesView->addGauge("MAGZ",0,-250.0f,250.0f);
    gaugesView->addGauge("ROLL",0,-180,180);
    gaugesView->addGauge("PITCH",0,-180,180);
    gaugesView->addGauge("YAW",0,-180,180);

    qDebug() << "Settings file: " << settings.fileName();

    // Restore the window position and size
    if (settings.contains(getWindowGeometryKey()))
    {
        restoreGeometry(settings.value(getWindowGeometryKey()).toByteArray());
    }

    // Restore the widget positions and size
    if (settings.contains(getWindowStateKey()))
    {
        restoreState(settings.value(getWindowStateKey()).toByteArray());
    }

    connect(ui->actionSerial, SIGNAL(triggered()), this, SLOT(showSerialConfiguration()));
    connect(ui->actionJoystick, SIGNAL(triggered()), this, SLOT(showJoystickConfiguration()));
    connect(ui->actionSenCalib, SIGNAL(triggered()), this, SLOT(showSensorsCalibration()));
    connect(ui->actionSensorVariance, SIGNAL(triggered()), this, SLOT(showSensosrVariance()));

    joystick->startPolling();
    protocol->openSerial();

    show();
    //this->statusBar()->showMessage("kikoolol");
}

IHMControl::~IHMControl()
{
    delete ui;

    // Get and delete all dockwidgets and contained
    // widgets
    QObjectList childList(this->children());

    QObjectList::iterator i;
    QDockWidget* dockWidget;
    for (i = childList.begin(); i != childList.end(); ++i)
    {
        dockWidget = dynamic_cast<QDockWidget*>(*i);
        if (dockWidget)
        {
            delete dockWidget;
            dockWidget = NULL;
        }
        else if (dynamic_cast<QWidget*>(*i))
        {
            delete dynamic_cast<QWidget*>(*i);
            *i = NULL;
        }
    }
}

void IHMControl::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}

void IHMControl::loadStylesheet()
{
    // Load style sheet
    QFile* styleSheet = new QFile(styleFileName);
    if (!styleSheet->exists())
    {
        styleSheet = new QFile(":files/styles/style-default.css");
    }
    if (styleSheet->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString style = QString(styleSheet->readAll());
        style.replace("ICONDIR", QCoreApplication::applicationDirPath()+ "files/styles/");
        qApp->setStyleSheet(style);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("IHMControl did lot load a new style"));
        msgBox.setInformativeText(tr("Stylesheet file %1 was not readable").arg(styleFileName));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
    }
    delete styleSheet;
}

void IHMControl::generateToolBar()
{
    QLabel* serialLabel = new QLabel("Serial Link",this);
    ui->toolBar->addWidget(serialLabel);

    serialStatus = new Led(this);
    ui->toolBar->addWidget(serialStatus);
    connect(protocol, SIGNAL(serialOpened()), serialStatus, SLOT(turnOn()));
    connect(protocol, SIGNAL(serialClosed()), serialStatus, SLOT(turnOff()));

    ui->toolBar->addSeparator();

    QLabel* joyStickLabel = new QLabel("JoyStick",this);
    ui->toolBar->addWidget(joyStickLabel);

    joyStickStatus = new Led(this);
    ui->toolBar->addWidget(joyStickStatus);
    connect(joystick, SIGNAL(joystickStarted(bool)), joyStickStatus, SLOT(turn(bool)));

    ui->toolBar->addSeparator();

    QLabel* isArmedLabel = new QLabel("Armed",this);
    ui->toolBar->addWidget(isArmedLabel);

    armedStatus = new Led(this);
    armedStatus->setColor(Qt::red);
    ui->toolBar->addWidget(armedStatus);
    connect(uav, SIGNAL(armed()), armedStatus, SLOT(turnOn()));
    connect(uav, SIGNAL(disarmed()), armedStatus, SLOT(turnOff()));

    ui->toolBar->addSeparator();

    QLabel* flightModeLabel = new QLabel("FlightMode",this);
    ui->toolBar->addWidget(flightModeLabel);
    flightMode = new QComboBox;
    flightMode->addItem( "Waiting" , QVariant(UAV::FLIGHTMODE_WAITING));
    flightMode->addItem( "Normal" , QVariant(UAV::FLIGHTMODE_NORMAL) );
    flightMode->addItem( "Covariance" , QVariant(UAV::FLIGHTMODE_VARIANCE) );
    ui->toolBar->addWidget(flightMode);

    connect(flightMode, SIGNAL(currentIndexChanged(int)), this, SLOT(flightModeChange(int)));

    ui->toolBar->addSeparator();

}

void IHMControl::flightModeChange(int idx)
{
   QVariant var = flightMode->itemData(idx);
   uav->changeFlightMode(var.toInt());
}

void IHMControl::addCentralWidget(QWidget* widget, const QString& title, QIcon icon)
{
    if (centerStack->indexOf(widget) == -1)
    {
        centerStack->addWidget(widget);
        QAction* tmpAction = ui->toolBar->addAction(icon,title);
        tmpAction->setCheckable(true);
        tmpAction->setText(title);
        QVariant var;
        var.setValue((QWidget*)widget);
        tmpAction->setData(var);
        centerStackActionGroup->addAction(tmpAction);
        connect(tmpAction,SIGNAL(triggered()),this, SLOT(showCentralWidget()));
        connect(widget, SIGNAL(visibilityChanged(bool)), tmpAction, SLOT(setChecked(bool)));
        tmpAction->setChecked(widget->isVisible());
    }
}

void IHMControl::showCentralWidget()
{
    QAction* action = qobject_cast<QAction *>(sender());
    QWidget* widget = qVariantValue<QWidget *>(action->data());
    centerStack->setCurrentWidget(widget);
}

void IHMControl::showSerialConfiguration()
{
    SerialConfiguration* config = new SerialConfiguration();
    config->show();
}

void IHMControl::showJoystickConfiguration()
{
    JoystickConfiguration* joystick = new JoystickConfiguration();
    joystick->show();
}

void IHMControl::showSensorsCalibration()
{
    SensorsCalibration* calibration = new SensorsCalibration();
    calibration->show();
}

void IHMControl::showSensosrVariance()
{
    SensorsVariance* variance = new SensorsVariance();
    variance->show();
}

QString IHMControl::getWindowStateKey()
{
    return "windowstate";
}

QString IHMControl::getWindowGeometryKey()
{
    return "windowgeometry";
}

void IHMControl::loadSettings()
{
    QSettings settings;
    settings.beginGroup("IHMCONTROL_MAINWINDOW");
    settings.endGroup();
}

void IHMControl::saveSettings()
{
    QSettings settings;
    settings.beginGroup("IHMCONTROL_MAINWINDOW");
    settings.endGroup();

    settings.setValue(getWindowGeometryKey(), saveGeometry());
    settings.setValue(getWindowStateKey(), saveState());
    settings.sync();
}

void IHMControl::showCriticalMessage(const QString& title, const QString& message)
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText(title);
    msgBox.setInformativeText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void IHMControl::showInfoMessage(const QString& title, const QString& message)
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(title);
    msgBox.setInformativeText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}

void IHMControl::showStatusMessage(const QString& status, int timeout)
{
    statusBar()->showMessage(status, timeout);
}

void IHMControl::showStatusMessage(const QString& status)
{
    statusBar()->showMessage(status, 0);
}

void IHMControl::showStatusMessage5s(const QString& status)
{
    statusBar()->showMessage(status, 5000);
}

