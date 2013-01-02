#ifndef PROTOCOL_MESSAGE_H
#define PROTOCOL_MESSAGE_H
#include <stdint.h>

#include <QDebug>

//#define PROCOTOL_BITSWAP 1

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
    uint8_t datas[255];
} protocol_message_t;

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

#include "MessageSystem.h"
#include "MessageSensor.h"
#include "MessageMotor.h"

#endif // PROTOCOL_MESSAGE_H
