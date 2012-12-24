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
    
signals:
    void receiveMessage(MessageInterface msg);

protected:
    explicit Protocol(QObject *parent = 0);

    QextSerialPort* serial;

public slots:
    
};

#endif // PROTOCOL_H
