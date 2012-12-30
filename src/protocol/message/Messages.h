#ifndef PROTOCOL_MESSAGE_H
#define PROTOCOL_MESSAGE_H
#include <stdint.h>

#include <QDebug>

enum PROTOCOL_CMD
{
   PROTOCOL_UNKNOW,
   PROTOCOL_MSG_SENSOR,
   PROTOCOL_MSG_SYSTEM,
   PROCOCOL_MSG_COMMAND,
   PROTOCOL_MSG_PID,
   PROTOCOL_MSG_MOTOR
};

typedef struct __protocol_message {
    uint8_t cmd;
    uint8_t len;
    uint16_t datas[255];
} protocol_message_t;

#define PROTOCOL_MESSAGE_SENSOR_LEN 18
typedef struct __protocol_message_sensor {
    uint16_t accX;
    uint16_t accY;
    uint16_t accZ;
    uint16_t gyroX;
    uint16_t gyroY;
    uint16_t gyroZ;
    uint16_t roll;
    uint16_t pitch;
    uint16_t yaw;
} protocol_message_sensor_t;


#define _PROTOCOL_PAYLOAD(msg) ((const char *)(&((msg)->datas[0])))
#define _PROTOCOL_RETURN_uint8_t(msg, offset) (uint8_t)_PROTOCOL_PAYLOAD(msg)[offset];
/*#define _PROTOCOL_RETURN_TYPE(TYPE, SIZE) \
    static inline TYPE _PROTOCOL_RETURN_## TYPE(const protocol_message_t* msg, uint8_t offset) \
    { TYPE r; byte_copy_## SIZE((char*)&r, &_PROTOCOL_PAYLOAD(msg)[offset]); return r; }

_PROTOCOL_RETURN_TYPE(uint16_t, 2)
_PROTOCOL_RETURN_TYPE(int16_t, 2)*/

static inline uint16_t _PROTOCOL_RETURN_uint16_t(const protocol_message_t* msg, uint8_t offset)
{
    return (msg->datas[offset] << 8) + (msg->datas[offset+1] & 0xFF);
}

inline static void protocol_message_sensor_decode(const protocol_message_t* msg, protocol_message_sensor_t* sensor)
{
    memcpy(sensor, _PROTOCOL_PAYLOAD(msg), PROTOCOL_MESSAGE_SENSOR_LEN);
}

#define PROTOCOL_MESSAGE_SYSTEM_LEN 8
typedef struct __protocol_message_system {
    uint16_t cpuLoad;
    uint16_t flightMode;
    uint16_t batteryVoltage;
    uint16_t mainLoopTime;
} protocol_message_system_t;

inline static void protocol_message_system_decode(const protocol_message_t* msg, protocol_message_system_t* system)
{
    system->cpuLoad = _PROTOCOL_RETURN_uint16_t(msg,0);
    system->flightMode = _PROTOCOL_RETURN_uint16_t(msg,2);
    system->batteryVoltage = _PROTOCOL_RETURN_uint16_t(msg,4);
    system->mainLoopTime = _PROTOCOL_RETURN_uint16_t(msg,6);
}

#endif // PROTOCOL_MESSAGE_H
