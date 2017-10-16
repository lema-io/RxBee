#ifndef RXBEE_SERIAL_DATA_SUBJECT_H
#define RXBEE_SERIAL_DATA_SUBJECT_H

#include <stdint.h>
#include <vector>

namespace RXBee
{

class SerialDataObserver;

class SerialDataSubject
{
public:
    SerialDataSubject();
    ~SerialDataSubject();
    
    void Subscribe(SerialDataObserver* observer);
    void Unsubscribe(SerialDataObserver* observer);
    
    void Next(const std::vector<uint8_t>& data);
    void Next(const uint8_t* data, const uint16_t len);
    void Complete();
    void Error(const int32_t error_code);

private:
    std::vector<SerialDataObserver*> observers;
};
    
} // namespace RXBee

#endif // RXBEE_SERIAL_DATA_SUBJECT_H