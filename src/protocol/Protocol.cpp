#include <QSettings>
#include <QDebug>
#include <QByteArray>

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
    QObject(parent),
    step(0),
    len(0),
    crc(0)
{

    readSettings();
    connect(this,SIGNAL(serialOpened()), this, SLOT(serialLinkUp()));
    connect(this, SIGNAL(serialClosed()), this, SLOT(serialLinkUp()));
    connect(this, SIGNAL(datasReceive(QByteArray*)), this, SLOT(onMessageReceive(QByteArray*)));

    serial = new QextSerialPort(portName, portSettings);
    emit serialClosed();
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
            serial->disconnect();
            emit serialClosed();
            emit serialError("Error open serial",serial->errorString());
        } else {
            connect(serial, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
            emit serialOpened();
        }
    }
}

void Protocol::closeSerial()
{
    if (serial->isOpen()) {
        serial->close();
        serial->disconnect();
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

void Protocol::onDataAvailable()
{
    if (!serial->bytesAvailable()) {
        return;
    }

    QByteArray datas = serial->readAll();
    for (int i=0; i<datas.length(); i++) {

        const unsigned char data = datas.at(i);
        emit receiveByte(data);

        switch(step) {
            case STX1:
                if (data == 0xFF)
                    step = STX2;
                break;
            case STX2:
                if (data == 0xFF){
                    step = CTX;
                    len = 0;
                    buffer = new QByteArray;
                    crc = 0;
                } else {
                    step = STX1;
                }
                break;
            case CTX:
                buffer->append(data);
                crc ^= data;
                step = LTX;
                break;
            case LTX:
                buffer->append(data);
                crc ^= data;
                len = data;
                if  ( len <= 0 ) {
                    step = STX1;
                } else {
                    step = DTX;
                }
                break;
            case DTX:
                buffer->append(data);
                crc ^= data;
                len--;
                if (len == 0) {
                    step = CRC;
                }
                break;
            case CRC:
                if( crc != data ){
                    qDebug() << "ERROR CRC";
                } else {
                    emit datasReceive(buffer);
                }
                step = STX1;
                break;
        }
    }

}


void Protocol::onMessageReceive(QByteArray* datas)
{
    uavlink_message_t msg;
    msg.cmd = (uint8_t) datas->at(0);
    msg.len = (uint8_t) datas->at(1);
    for( int i=2, j=0; i<datas->length(); i++, j++) {
        msg.datas[j] = (uint8_t) datas->at(i);
    }

    emit receiveMessage(msg);
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
