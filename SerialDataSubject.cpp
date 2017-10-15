
#include <vector>

#include "SerialDataSubject.h"
#include "SerialDataObserver.h"

namespace RXBee
{
    
SerialDataSubject::SerialDataSubject()
{
    
}

SerialDataSubject::~SerialDataSubject()
{
    
}
    
void SerialDataSubject::Subscribe(SerialDataObserver* observer)
{
    observers.push_back(observer);
}

void SerialDataSubject::Unsubscribe(SerialDataObserver* observer)
{
    for (std::vector<SerialDataObserver*>::iterator itr = observers.begin();
         itr != observers.end(); ++itr)
    {
        if (*itr == observer)
        {
            observers.erase(itr);
            break;
        }
    }
}

void SerialDataSubject::Next(const uint64_t source_addr, 
                             const std::vector<uint8_t>& data)
{
    for (uint16_t i = 0; i < observers.size(); ++i)
    {
        observers[i]->OnNext(source_addr, data);
    }
}


void SerialDataSubject::Next(const std::vector<uint8_t>& data)
{
    for (uint16_t i = 0; i < observers.size(); ++i)
    {
        observers[i]->OnNext(data);
    }  
}

void SerialDataSubject::Complete()
{
    for (uint16_t i = 0; i < observers.size(); ++i)
    {
        observers[i]->OnComplete();
    }
}

void SerialDataSubject::Error(const int32_t error_code)
{
    for (uint16_t i = 0; i < observers.size(); ++i)
    {
        observers[i]->OnError(error_code);
    }
}
    
} // namespace RXBee