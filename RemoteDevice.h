

#ifndef RXBEE_REMOTE_DEVICE_H
#define RXBEE_REMOTE_DEVICE_H

#include "SpecificDevice.h"

namespace RXBee
{
  
   
class RemoteDevice : public SpecificDevice<RemoteDevice>
{
public:
    RemoteDevice();
    RemoteDevice(Network* n, uint64_t address, const char* const identifier);
    virtual ~RemoteDevice();
    
    virtual Device::Location GetLocation() const;
    
    RemoteDevice* Transmit(const uint8_t* buffer, const uint16_t len);
    RemoteDevice* ReadMaxTransmitPayload();
    
private:
    uint16_t max_payload;
};
    
} // namespace RXBee


#endif // RXBEE_REMOTED_DEVICE_G