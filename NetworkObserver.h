

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
    
    virtual void OnDiscoveryComplete(Network* network) = 0;
    
    virtual void OnStatusChanged(Network* network, Network::ModemStatus status) = 0;
    
    virtual void OnDeviceCommandComplete(Network* network, Device* device) = 0;
    
    virtual void OnDeviceTransactionComplete(Network* network, Device* device, Transaction* transaction) = 0;
};

} // namespace RXBee

#endif /* RXBEE_NETWORK_OBSERVER_H */
