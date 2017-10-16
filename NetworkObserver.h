

#ifndef RXBEE_NETWORK_OBSERVER_H    /* Guard against multiple inclusion */
#define RXBEE_NETOWRK_OBSERVER_H



#include <stdint.h>

#include "Network.h"

namespace RXBee
{
    
class NetworkObserver
{
public:
    NetworkObserver() { };
    virtual ~NetworkObserver() { };
   
    virtual void OnSerialDataReceived(const uint64_t source_addr, const std::vector<uint8_t>& data) = 0;
    
    virtual void OnDeviceDiscovered(Network* network, RemoteDevice* Device) = 0;
    
    virtual void OnStatusChanged(Network* network, Network::ModemStatus prev, Network::ModemStatus current) = 0;
    
    virtual void OnDeviceTransactionComplete(Network* network, Device* device, Transaction* transaction) = 0;
};

} // namespace RXBee

#endif /* RXBEE_NETWORK_OBSERVER_H */
