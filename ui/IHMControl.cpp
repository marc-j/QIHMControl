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
#include "ui/SerialConfiguration.h";
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
    styleFileName(QCoreApplication::applicationDirPath() + "/style-default.css")
{

    loadSettings();

    ui->setupUi(this);
    hide();

    uav = UAV::instance();
    protocol = Protocol::instance();
    connect(protocol, SIGNAL(serialError(QString,QString)), this, SLOT(showCriticalMessage(QString,QString)));

    // Set dock options
    setDockOptions(AnimatedDocks | AllowTabbedDocks | AllowNestedDocks);
    statusBar()->setSizeGripEnabled(true);

    // Setup corners
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    //qApp->setStyle("oxygen");
    loadStylesheet();

    generateToolBar();

   /* QWebView *webView = new QWebView(this);
    webView->resize(1000,500);
    webView->move(10,10);
    QString gMapURL = "England"; // this is where you want to point
    gMapURL = "http://www.google.fr";
    webView->setUrl(QUrl(gMapURL));*/

    uavView = new UAVView(this);
    setCentralWidget(uavView);

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

    pidBox = new PIDBox(this);
    addDockWidget(Qt::RightDockWidgetArea, pidBox);

    QDockWidget* gaugesView1Dock = new QDockWidget(tr("Gauges"),this);
    gaugesView = new GaugesView(tr("Gauges"),this);
    gaugesView1Dock->setWidget(gaugesView);
    gaugesView1Dock->setObjectName("IHMCONTROL_GAUGES_VIEW_DOCK1");
    addDockWidget(Qt::LeftDockWidgetArea, gaugesView1Dock);

    qDebug() << "Settings file: " << settings.fileName();

    // Restore the window position and size
    if (settings.contains(getWindowGeometryKey()))
    {
        qDebug() << "LOAD GEOMETRY";
        restoreGeometry(settings.value(getWindowGeometryKey()).toByteArray());
    }

    // Restore the widget positions and size
    if (settings.contains(getWindowStateKey()))
    {
        restoreState(settings.value(getWindowStateKey()).toByteArray());
    }

    connect(ui->actionSerial, SIGNAL(triggered()), this, SLOT(showSerialConfiguration()));
    show();
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

    ui->toolBar->addSeparator();

    QLabel* isArmedLabel = new QLabel("Armed",this);
    ui->toolBar->addWidget(isArmedLabel);

    armedStatus = new Led(this);
    armedStatus->setColor(Qt::red);
    ui->toolBar->addWidget(armedStatus);
    connect(uav, SIGNAL(armed()), armedStatus, SLOT(turnOn()));
    connect(uav, SIGNAL(disarmed()), armedStatus, SLOT(turnOff()));
}

void IHMControl::showSerialConfiguration()
{
    SerialConfiguration* config = new SerialConfiguration();
    config->show();
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

