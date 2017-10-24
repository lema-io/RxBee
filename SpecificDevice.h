
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
 

T* SetPreambleID(uint8_t id) { GetNextCmdTransaction()->GetFrame()->AddFields(XBEE_CMD_HP, id); return dynamic_cast<T*>(this); }
T* ReadPreambleID() {  GetNextCmdTransaction()->GetFrame()->AddFields(XBEE_CMD_HP); return dynamic_cast<T*>(this); } 

T* SetNetworkID(uint16_t id) { GetNextCmdTransaction()->GetFrame()->AddFields(XBEE_CMD_ID, id); return dynamic_cast<T*>(this); }
T* ReadNetworkID() { GetNextCmdTransaction()->GetFrame()->AddField(XBEE_CMD_ID); return dynamic_cast<T*>(this); }
    
T* SetRoutingMode(uint8_t mode) { GetNextCmdTransaction()->GetFrame()->AddFields(XBEE_CMD_CE, mode); return dynamic_cast<T*>(this); }
T* ReadRoutingMode() { GetNextCmdTransaction()->GetFrame()->AddField(XBEE_CMD_CE); return dynamic_cast<T*>(this); }


T* SetIdentifier(const std::string& identifier) { GetNextCmdTransaction()->GetFrame()->AddFields(XBEE_CMD_NI, identifier); return dynamic_cast<T*>(this); } 
T* ReadIdentifier() { GetNextCmdTransaction()->GetFrame()->AddField(XBEE_CMD_NI); return dynamic_cast<T*>(this); } 
    
const std::string& GetIdentifier() { return ident; } 

T* BeginCommandQueue() { SetQueueCommands(true); }

T* EndCommandQueue() { GetNextCmdTransaction()->GetFrame()->AddField(XBEE_CMD_AC); return dynamic_cast<T*>(this); }
    
private:
    std::string ident;
};

}

#endif // RXBEE_SPECIFIC_DEVICE_H