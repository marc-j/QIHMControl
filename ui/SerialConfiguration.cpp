#include <QList>
#include <qextserialenumerator.h>

#include "SerialConfiguration.h"
#include "ui_SerialConfiguration.h"

SerialConfiguration::SerialConfiguration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialConfiguration)
{
    setWindowTitle("SerialPort Configuration");

    ui->setupUi(this);

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    for (int i=0; i<ports.size();i++) {
        ui->portListBox->addItem( ports.at(i).portName + " [" + ports.at(i).friendName + "]" );
    }
    // Set up baud rates
    ui->baudRateBox->clear();

    // Keep track of all desired baud rates by OS. These are iterated through
    // later and added to ui.baudRate.
    QList<int> supportedBaudRates;

        // Baud rates supported only by POSIX systems
    #if defined(Q_OS_UNIX) || defined(Q_OS_LINUX) || defined(Q_OS_DARWIN)
        supportedBaudRates << 50;
        supportedBaudRates << 75;
        supportedBaudRates << 134;
        supportedBaudRates << 150;
        supportedBaudRates << 200;
        supportedBaudRates << 1800;
    #endif

        // Baud rates supported only by Windows
    #if defined(Q_OS_WIN)
        supportedBaudRates << 14400;
        supportedBaudRates << 56000;
        supportedBaudRates << 128000;
        supportedBaudRates << 256000;
    #endif

        // Baud rates supported by everyone
        supportedBaudRates << 110;
        supportedBaudRates << 300;
        supportedBaudRates << 600;
        supportedBaudRates << 1200;
        supportedBaudRates << 2400;
        supportedBaudRates << 4800;
        supportedBaudRates << 9600;
        supportedBaudRates << 19200;
        supportedBaudRates << 38400;
        supportedBaudRates << 57600;
        supportedBaudRates << 115200;
        supportedBaudRates << 230400;
        supportedBaudRates << 460800;

    #if defined(Q_OS_LINUX)
        // Baud rates supported only by Linux
        supportedBaudRates << 500000;
        supportedBaudRates << 576000;
    #endif

        supportedBaudRates << 921600;

    // Now actually add all of our supported baud rates to the UI.
    qSort(supportedBaudRates.begin(), supportedBaudRates.end());
    for (int i = 0; i < supportedBaudRates.size(); ++i) {
        ui->baudRateBox->addItem(QString::number(supportedBaudRates.at(i)), supportedBaudRates.at(i));
    }
}

SerialConfiguration::~SerialConfiguration()
{
    delete ui;
}
