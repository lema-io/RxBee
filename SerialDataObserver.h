

#ifndef RXBEE_SERIAL_DATA_OBSERVER_H
#define RXBEE_SERIAL_DATA_OBSERVER_H

#include <stdint.h>

namespace RXBee
{
    
class SerialDataObserver
{
public:
    SerialDataObserver() {}
    virtual ~SerialDataObserver() {}
    
    virtual void OnNext(const std::vector<uint8_t>& data) = 0;
    virtual void OnNext(const uint8_t* data, const uint16_t len) = 0;
    virtual void OnComplete() = 0;
    virtual void OnError(const int32_t error_code) = 0;
};
    
}


#endif // RXBEE_SERIAL_DATA_OBSERVER_H