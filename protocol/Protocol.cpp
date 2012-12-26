#include <QSettings>
#include <QDebug>

#include "Protocol.h"

Protocol* Protocol::instance()
{
    static Protocol* _instance = 0;
    if(_instance == 0)
    {
        _instance = new Protocol();
    }
    return _instance;
}

Protocol::Protocol(QObject *parent) :
    QObject(parent)
{

    readSettings();
    connect(this,SIGNAL(serialOpened()), this, SLOT(serialLinkUp()));
    connect(this, SIGNAL(serialClosed()), this, SLOT(serialLinkUp()));

    serial = new QextSerialPort(portName, portSettings);
    emit serialClosed();

    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial error: " << serial->errorString();
        emit serialError("Error open serial",serial->errorString());
    } else {
        qDebug() << "Serial opened";
        emit serialOpened();
    }
}

Protocol::~Protocol()
{
    saveSettings();
    delete serial;
}

void Protocol::openSerial()
{
    if (!serial->isOpen()) {
        if (!serial->open(QIODevice::ReadWrite)) {
            emit serialClosed();
            qDebug() << "Serial error: " << serial->errorString();
            emit serialError("Error open serial",serial->errorString());
        } else {
            emit serialOpened();
        }
    }
}

void Protocol::closeSerial()
{
    if (serial->isOpen()) {
        serial->close();
        emit serialClosed();
    }
}

void Protocol::portSettingsChange(QString portName, BaudRateType baud, DataBitsType bit,  StopBitsType stop, ParityType parity)
{
    closeSerial();
    serial->setPortName(portName);
    serial->setBaudRate(baud);
    serial->setDataBits(bit);
    serial->setStopBits(stop);
    serial->setParity(parity);

    portSettings.BaudRate = baud;
    portSettings.DataBits = bit;
    portSettings.StopBits = stop;
    portSettings.Parity = parity;
    this->portName = portName;
    openSerial();
}

PortSettings Protocol::getPortSettings()
{
    return portSettings;
}

QString Protocol::getPortName()
{
    return portName;
}

void Protocol::saveSettings()
{
    QSettings settings;

    settings.beginGroup("SERIAL");
    settings.setValue("PORT", serial->portName());
    settings.setValue("BAUDRATE", serial->baudRate());
    settings.setValue("DATABITS", serial->dataBits());
    settings.setValue("STOPBITS", serial->stopBits());
    settings.setValue("FLOWCONTROL", serial->flowControl());
    settings.setValue("PARITY", serial->parity());
    settings.endGroup();
    settings.sync();
}

void Protocol::readSettings()
{
    QSettings settings;

    settings.beginGroup("SERIAL");
    portSettings.BaudRate = (BaudRateType)settings.value("BAUDRATE",BAUD115200).toInt();
    portSettings.DataBits = (DataBitsType)settings.value("DATABITS", DATA_8).toInt();
    portSettings.StopBits = (StopBitsType)settings.value("STOPBITS", STOP_2).toInt();
    portSettings.Parity = (ParityType)settings.value("PARITY", PAR_NONE).toInt();
    portName = settings.value("PORT", "/dev/ttyS0").toString();
    settings.endGroup();

}
