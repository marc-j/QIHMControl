#ifndef MESSAGEMOTOR_H
#define MESSAGEMOTOR_H

/*----------------------------------------------------
 # Sensor
 #--------------------------------------------------*/

#define PROTOCOL_MESSAGE_MOTOR_LEN 8
typedef struct __protocol_message_motor {
    int16_t motorFrontLeft;
    int16_t motorFrontRight;
    int16_t motorRearLeft;
    int16_t motorRearRight;
} protocol_message_motor_t;

inline static void protocol_message_motor_decode(const protocol_message_t* msg, protocol_message_motor_t* motor)
{

#ifdef PROCOTOL_BITSWAP
    motor->motorFrontLeft = _PROTOCOL_RETURN_uint16_t(msg,0);
    motor->motorFrontRight = _PROTOCOL_RETURN_uint16_t(msg,2);
    motor->motorRearLeft = _PROTOCOL_RETURN_uint16_t(msg,4);
    motor->motorRearRight = _PROTOCOL_RETURN_uint16_t(msg,6);
#else
    memcpy(motor, _PROTOCOL_PAYLOAD(msg), PROTOCOL_MESSAGE_MOTOR_LEN);
#endif
}

#endif // MESSAGEMOTOR_H
