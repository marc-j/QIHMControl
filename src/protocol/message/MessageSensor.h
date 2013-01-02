#ifndef MESSAGESENSOR_H
#define MESSAGESENSOR_H

/*----------------------------------------------------
 # Sensor
 #--------------------------------------------------*/

#define PROTOCOL_MESSAGE_SENSOR_LEN 18
typedef struct __protocol_message_sensor {
    int16_t accX;
    int16_t accY;
    int16_t accZ;
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
} protocol_message_sensor_t;

inline static void protocol_message_sensor_decode(const protocol_message_t* msg, protocol_message_sensor_t* sensor)
{

#ifdef PROCOTOL_BITSWAP
    sensor->accX = _PROTOCOL_RETURN_uint16_t(msg,0);
    sensor->accY = _PROTOCOL_RETURN_uint16_t(msg,2);
    sensor->accZ = _PROTOCOL_RETURN_uint16_t(msg,4);
    sensor->gyroX = _PROTOCOL_RETURN_uint16_t(msg,6);
    sensor->gyroY = _PROTOCOL_RETURN_uint16_t(msg,8);
    sensor->gyroZ = _PROTOCOL_RETURN_uint16_t(msg,10);
    sensor->roll = _PROTOCOL_RETURN_uint16_t(msg,12);
    sensor->pitch = _PROTOCOL_RETURN_uint16_t(msg,14);
    sensor->yaw = _PROTOCOL_RETURN_uint16_t(msg,16);
#else
    memcpy(sensor, _PROTOCOL_PAYLOAD(msg), PROTOCOL_MESSAGE_SENSOR_LEN);
#endif
}

#endif // MESSAGESENSOR_H
