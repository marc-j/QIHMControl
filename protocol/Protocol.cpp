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

    serial = new QextSerialPort("COM1");
}

Protocol::~Protocol()
{

}
