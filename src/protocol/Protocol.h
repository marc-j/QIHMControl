#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <qextserialport.h>

#include "protocol/message/MessageInterface.h"

class Protocol : public QObject
{
    Q_OBJECT
    Q_ENUMS(CMD)
public:
    static Protocol* instance();
    ~Protocol();

    enum CMD
    {
       PROTOCOL_NULL,
       PROTOCOL_MSG_SENSOR,
       PROTOCOL_MSG_SYSTEM,
       PROCOCOL_MSG_COMMAND,
       PROTOCOL_MSG_PID,
       PROTOCOL_MSG_MOTOR
    };

    PortSettings getPortSettings();
    QString getPortName();
    
signals:
    void receiveMessage(MessageInterface msg);
    void serialOpened();
    void serialClosed();
    void serialError(QString,QString);
    void serialMessage(QString);

protected:
    explicit Protocol(QObject *parent = 0);

    QextSerialPort* serial;
    PortSettings portSettings;
    QString portName;

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
};

#endif // PROTOCOL_H
