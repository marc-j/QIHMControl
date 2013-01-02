#ifndef MESSAGESYSTEM_H
#define MESSAGESYSTEM_H

/*----------------------------------------------------
 # SYSTEM
 #--------------------------------------------------*/

#define PROTOCOL_MESSAGE_SYSTEM_LEN 8
typedef struct __protocol_message_system {
    uint16_t cpuLoad;
    uint16_t flightMode;
    uint16_t batteryVoltage;
    uint16_t mainLoopTime;
} protocol_message_system_t;

inline static void protocol_message_system_decode(const protocol_message_t* msg, protocol_message_system_t* system)
{
#ifdef PROCOTOL_BITSWAP
    system->cpuLoad = _PROTOCOL_RETURN_uint16_t(msg,0);
    system->flightMode = _PROTOCOL_RETURN_uint16_t(msg,2);
    system->batteryVoltage = _PROTOCOL_RETURN_uint16_t(msg,4);
    system->mainLoopTime = _PROTOCOL_RETURN_uint16_t(msg,6);
#else
    memcpy(system, _PROTOCOL_PAYLOAD(msg), PROTOCOL_MESSAGE_SYSTEM_LEN );
#endif
}

#endif // MESSAGESYSTEM_H
