#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <qextserialport.h>
#include <QByteArray>

#include "UAVLink.h"

class Protocol : public QObject
{
    Q_OBJECT
    Q_ENUMS(CMD)
public:
    static Protocol* instance();
    ~Protocol();

    enum STEP {
        STX1,
        STX2,
        CTX,
        LTX,
        DTX,
        CRC
    };

    PortSettings getPortSettings();
    QString getPortName();
    
signals:
    void receiveMessage(uavlink_message_t msg);
    void serialOpened();
    void serialClosed();
    void serialError(QString,QString);
    void serialMessage(QString);

    void receiveByte(const unsigned char);
    void datasReceive(QByteArray* buffer);

protected:
    explicit Protocol(QObject *parent = 0);

    QextSerialPort* serial;
    PortSettings portSettings;
    QString portName;

    unsigned char step;
    unsigned char len;
    QByteArray* buffer;
    unsigned char crc;

    void saveSettings();
    void readSettings();

public slots:
    void closeSerial();
    void openSerial();
    void portSettingsChange(QString portName, BaudRateType baud, DataBitsType bit,  StopBitsType stop, ParityType parity);
    void serialLinkUp() {
        emit serialMessage("Serial link connected.");
    }
    void serialLinkDown() {
        emit serialMessage("Serial link disconnected.");
    }


private slots:
    void onDataAvailable();
    void onMessageReceive(QByteArray*);
};

#endif // PROTOCOL_H
