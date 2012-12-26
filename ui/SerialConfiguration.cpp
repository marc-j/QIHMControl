#include <QList>
#include <qextserialenumerator.h>
#include <qextserialport.h>

#include "SerialConfiguration.h"
#include "ui_SerialConfiguration.h"

SerialConfiguration::SerialConfiguration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SerialConfiguration)
{

    ui->setupUi(this);

    setWindowTitle("SerialPort Configuration");

    protocol = Protocol::instance();
    PortSettings portSettings = protocol->getPortSettings();

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    for (int i=0; i<ports.size();i++) {
        ui->portListBox->addItem( ports.at(i).physName, ports.at(i).physName );
    }

    ui->portListBox->setCurrentIndex(ui->portListBox->findData(protocol->getPortName()));

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

    ui->baudRateBox->setCurrentIndex(ui->baudRateBox->findData(portSettings.BaudRate));

    ui->stopBits->clear();
    ui->stopBits->addItem("1", STOP_1);
    ui->stopBits->addItem("2", STOP_2);

    ui->stopBits->setCurrentIndex(ui->stopBits->findData(portSettings.StopBits));

    ui->dataBits->clear();
    ui->dataBits->addItem("5", DATA_5);
    ui->dataBits->addItem("6", DATA_6);
    ui->dataBits->addItem("7", DATA_7);
    ui->dataBits->addItem("8", DATA_8);

    ui->dataBits->setCurrentIndex(ui->dataBits->findData(portSettings.DataBits));

    switch(portSettings.Parity) {
        case PAR_EVEN:
            ui->parityEven->setChecked(true);
            break;
        case PAR_ODD:
            ui->parityOdd->setChecked(true);
            break;
        case PAR_NONE:
        default:
            ui->parityNone->setChecked(true);
            break;
    }

    connect(ui->saveBtn, SIGNAL(clicked()), this, SLOT(updateValues()));
    connect(this,
            SIGNAL(serialSettingsChanged(QString,BaudRateType,DataBitsType,StopBitsType,ParityType)),
            protocol,
            SLOT(portSettingsChange(QString,BaudRateType,DataBitsType,StopBitsType,ParityType))
    );
}

SerialConfiguration::~SerialConfiguration()
{
    delete ui;
    delete protocol;
}

void SerialConfiguration::updateValues(){

    close();

    ParityType parity;
    if (ui->parityEven->isChecked()) {
        parity = PAR_EVEN;
    } else if( ui->parityOdd->isChecked()) {
        parity = PAR_ODD;
    } else {
        parity = PAR_NONE;
    }

    QString portName = ui->portListBox->currentText();
    BaudRateType baud = (BaudRateType)ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt();
    DataBitsType dataBits = (DataBitsType)ui->dataBits->itemData(ui->dataBits->currentIndex()).toInt();
    StopBitsType stopBits = (StopBitsType)ui->stopBits->itemData(ui->stopBits->currentIndex()).toInt();

    emit serialSettingsChanged(portName, baud, dataBits, stopBits, parity);

}
