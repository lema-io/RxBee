
#ifndef RXBEE_SPECIFIC_DEVICE_H
#define RXBEE_SPECIFIC_DEVICE_H

#include <stdint.h>

#include "Command.h"

#include "Device.h"

namespace RXBee
{

template <class T>
class SpecificDevice : public Device
{
public:
T* SetRoutingMode(uint8_t mode) { return dynamic_cast<T*>(CreateATCmdTransaction(XBEE_CMD_CE, mode)); }

T* ReadRoutingMode() { return dynamic_cast<T*>(CreateATCmdTransaction(XBEE_CMD_CE)); }


T* SetIdentifier(const std::string& identifier) { return dynamic_cast<T*>(CreateATCmdTransaction(XBEE_CMD_NI, identifier)); } 
   
T* ReadIdentifier() { return dynamic_cast<T*>(CreateATCmdTransaction(XBEE_CMD_NI)); } 
    
const std::string& GetIdentifier() { return ident; } 

T* BeginCommandQueue()
{
    SetQueueCommands(true);
}

T* EndCommandQueue()
{
    return dynamic_cast<T*>(CreateATCmdTransaction(XBEE_CMD_AC));
}
    
private:
    std::string ident;
};

}
    
#endif // RXBEE_SPECIFIC_DEVICE_H