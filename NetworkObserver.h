

#ifndef RXBEE_NETWORK_OBSERVER_H    /* Guard against multiple inclusion */
#define RXBEE_NETOWRK_OBSERVER_H



#include <stdint.h>

#include "Network.h"
#include "Types.h"

namespace RXBee
{
    
class NetworkObserver
{
public:
    NetworkObserver() { };
    virtual ~NetworkObserver() { };
   
    virtual void OnSerialDataReceived(const Address source_addr, const std::vector<uint8_t>& data) = 0;
    
    virtual void OnDeviceDiscovered(XBeeNetwork* network, const Address address, const std::string& node_id) = 0;
    
    virtual void OnStatusChanged(XBeeNetwork* network, ModemStatus prev, ModemStatus current) = 0;
};

} // namespace RXBee

#endif /* RXBEE_NETWORK_OBSERVER_H */
