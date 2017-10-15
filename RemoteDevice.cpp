
#include "RemoteDevice.h"

#define XBEE_DEFAULT_MAX_PAYLOAD (0x100)

namespace RXBee
{
    
RemoteDevice::RemoteDevice()
    : max_payload(XBEE_DEFAULT_MAX_PAYLOAD)
{

}

RemoteDevice::~RemoteDevice()
{
    
}


    
Device::Location RemoteDevice::GetLocation() const
{
    return Device::Location::REMOTE;
}

RemoteDevice* RemoteDevice::Transmit(const uint8_t* buffer, const uint16_t len)
{
    Transaction* t = GetNextTransaction();  
    Frame f;
    ApiID frame_id = ApiID::TRANSMIT_REQUEST;
    
    frame_id = ApiID::REMOTE_AT_COMMAND;
    f.AddField(GetAddress());
    f.AddField(0xFFFE);     // Reserved
    f.AddField((uint8_t)0); // Max hops on broadcast
    f.AddField(0xC0);       // Delivery method = DigiMesh
    
    if (len > max_payload)
    {
        // Transmit up to max payload bytes
        f.AddData(buffer, max_payload);
        SetTransactionFrame(t, f);
        
        // Chain next section of data
        
        Transmit(&buffer[max_payload], len - max_payload);
    }
    else
    {
        // Transmit entire data
        f.AddData(buffer, len);
        SetTransactionFrame(t, f);
    }
    
    return this;
}

} // namespace RXBee