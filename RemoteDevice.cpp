
#include "RemoteDevice.h"
#include "Transaction.h"
#include "Network.h"

#define XBEE_DEFAULT_MAX_PAYLOAD (0x100)

namespace RXBee
{
    
RemoteDevice::RemoteDevice()
    : max_payload(XBEE_DEFAULT_MAX_PAYLOAD)
{

}

RemoteDevice::RemoteDevice(Network* n, uint64_t address, const char* const identifier)
    : max_payload(XBEE_DEFAULT_MAX_PAYLOAD)
{
    SetAddress(address);
    SetNetwork(n);
    RecordNodeIdentifier(identifier);
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

    Frame* f = t->GetFrame();
    f->Initialize(ApiID::TRANSMIT_REQUEST, GetNetwork()->GetApiMode());
    
    f->AddFields(GetAddress(),
                 static_cast<uint16_t>(0xFFFE), // Reserved
                 static_cast<uint8_t>(0),       // Max hops on broadcast
                 static_cast<uint8_t>(0xC0));   // Delivery method = DigiMesh
    
    if (len > max_payload)
    {
        // Transmit up to max payload bytes
        f->AddData(buffer, max_payload);
        
        // Chain next section of data
        
        Transmit(&buffer[max_payload], len - max_payload);
    }
    else
    {
        // Transmit entire data
        f->AddData(buffer, len);
    }
    
    return this;
}

} // namespace RXBee