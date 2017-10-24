
#include "LocalDevice.h"
#include "Command.h"
#include "Transaction.h"

namespace RXBee
{
    
LocalDevice::LocalDevice()    
    : api_mode(ApiMode::ESCAPED)
{
    
}


LocalDevice::~LocalDevice()
{

}
    
Device::Location LocalDevice::GetLocation() const
{
    return Device::Location::LOCAL;
}


LocalDevice* LocalDevice::NetworkDiscover() { GetNextCmdTransaction()->GetFrame()->AddField(XBEE_CMD_ND); return this; }

LocalDevice* LocalDevice::SetApiMode(ApiMode mode) { GetNextCmdTransaction()->GetFrame()->AddFields(XBEE_CMD_AP, static_cast<uint8_t>(mode)); return this; }
LocalDevice* LocalDevice::ReadApiMode() { GetNextCmdTransaction()->GetFrame()->AddFields(XBEE_CMD_AP); return this; }
ApiMode LocalDevice::GetApiMode() const { return api_mode; }
    
} // namespace RXBee
