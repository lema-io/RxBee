
#ifndef RXBEE_TYPES_H
#define RXBEE_TYPES_H

#include "RXBee_Config.h"

namespace RXBee
{
    
#define XBEE_NETWORK_ID_DEFAULT (0x7FFF)
#define XBEE_NETWORK_ID_OEM     (0xFFFF)
#define XBEE_PREAMBLE_ID_DEFAULT (0x00)

#define XBEE_BROADCAST_ADDRESS  (0x000000000000FFFF)
#define RXBEE_LOCAL_ADDRESS     (0x0000000000000000) 

typedef uint64_t Address;
   
enum class ApiID : uint8_t 
{
    UNKOWN = 0x00,
    INVALID = 0x01,

    AT_COMMAND = 0x08,
    AT_QUEUE_COMMAND = 0x09,
    TRANSMIT_REQUEST = 0x10,
    EXPLICIT_ADDRESSING_COMMAND = 0x11,
    REMOTE_AT_COMMAND = 0x17,

    AT_COMMAND_RESPONSE = 0x88,
    MODEM_STATUS = 0x8A,
    TRANSMIT_STATUS = 0x8B,
    ROUTE_INFO_PACKET = 0x8D,
    AGGREGATE_ADDRESSING_UPDATE = 0x8E,
    RECEIVE_PACKET = 0x90,
    EXPLICIT_RX_INDICATOR = 0x91,
    IO_DATA_SAMPLE_RX_INDICATOR = 0x92,
    NODE_ID_INDICATOR = 0x95,
    REMOTE_AT_COMMAND_RESPONSE = 0x97
};

enum class ApiMode : uint8_t
{
    TRANSPARENT = 0x00,
    UNESCAPED = 0x01,
    ESCAPED = 0x02
};

enum class TxPowerLevel : uint8_t
{
    SEVEN_DBM = 0,          // +7dBm (5mW)
    FIFTEEN_DBM = 1,        // +15dBm (32mW)
    EIGHTEEN_DBM = 2,       // +18dBm (63mW)
    TWENTY_ONE_DBM = 3,     // +21dBm (125mW)
    TWENTY_FOUR_DBM = 4,     // +24dBm (250mW)
    INVALID = 0xFF
};

enum class ModemStatus
{
    HW_RESET                = 0x00,
    WATCHDOG_TIMER_RESET    = 0x01,
    NETWORK_WOKE_UP         = 0x0B,
    NETWORK_WENT_TO_SLEEP   = 0x0C,
    UNKNOWN                 = 0xFF
};

} // namespace RXBee
#endif // XBEE_TYPES_H