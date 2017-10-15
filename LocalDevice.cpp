
#include "LocalDevice.h"
#include "Command.h"

namespace RXBee
{
    
LocalDevice::LocalDevice()    
    : api_mode(ApiMode::TRANSPARENT)
{
    
}


LocalDevice::~LocalDevice()
{

}
    
Device::Location LocalDevice::GetLocation() const
{
    return Device::Location::LOCAL;
}

LocalDevice* LocalDevice::SetPreambleID(uint8_t id) { return dynamic_cast<LocalDevice*>(CreateATCmdTransaction(XBEE_CMD_HP, id)); }
LocalDevice* LocalDevice::ReadPreambleID() { return dynamic_cast<LocalDevice*>(CreateATCmdTransaction(XBEE_CMD_HP)); } 

LocalDevice* LocalDevice::SetNetworkID(uint16_t id) { return dynamic_cast<LocalDevice*>(CreateATCmdTransaction(XBEE_CMD_ID, id)); }
LocalDevice* LocalDevice::ReadNetworkID() { return dynamic_cast<LocalDevice*>(CreateATCmdTransaction(XBEE_CMD_ID)); }

LocalDevice* LocalDevice::NetworkDiscover() { return dynamic_cast<LocalDevice*>(CreateATCmdTransaction(XBEE_CMD_ND)); }

LocalDevice* LocalDevice::SetApiMode(ApiMode mode) { return dynamic_cast<LocalDevice*>(CreateATCmdTransaction(XBEE_CMD_AP, static_cast<uint8_t>(mode))); }
LocalDevice* LocalDevice::ReadApiMode() { return dynamic_cast<LocalDevice*>(CreateATCmdTransaction(XBEE_CMD_AP)); }
ApiMode LocalDevice::GetApiMode() const { return api_mode; }
    
} // namespace RXBee
