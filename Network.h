
#ifndef RXBEE_NETOWRK_H
#define RXBEE_NETOWRK_H

#include <stdint.h>
#include <vector>
#include <string>

#include "Frame.h"
#include "RemoteDevice.h"
#include "LocalDevice.h"
#include "Command.h"
#include "SerialDataObserver.h"
#include "SerialDataSubject.h"
#include "Transaction.h"
#include "Types.h"

#define XBEE_NETWORK_ID_DEFAULT (0x7FFF)
#define XBEE_NETWORK_ID_OEM     (0xFFFF)

#define RXBEE_RX_BUFFER_SIZE   (512)
#define RXBEE_TX_BUFFER_SIZE   (512)

#define RXBEE_MAX_FRAME_COUNT  (0xFF)


namespace RXBee
{
    
class NetworkObserver;

class Network : public SerialDataObserver
{
public:
    typedef void (*Callback)(Network* source);

    enum class ModemStatus
    {
        HW_RESET                = 0x00,
        WATCHDOG_TIMER_RESET    = 0x01,
        NETWORK_WOKE_UP         = 0x0B,
        NETWORK_WENT_TO_SLEEP   = 0x0C,
        UNKNOWN                 = 0xFF
    };
    
    enum class DeliveryStatus
    {
        SUCCESS = 0x00,
        MAC_ACK_FAIL = 0x01,
        COLLISION_AVOIDANCE_FAIL = 0x02,
        NETWORK_ACK_FAIL = 0x21,
        ROUTE_NOT_FOUND = 0x25,
        INTERNAL_RESOURCE_ERROR = 0x31,
        INTERNAL_ERROR = 0x32,
        PAYLOAD_TOO_LARGE = 0x74,
        INDIRECT_MSG_REQUESTED = 0x75
    };
    
    enum class CommandStatus
    {
        OK = 0x00,
        ERROR = 0x01,
        INVALID_COMMAND = 0x02,
        INVALID_PARAMETER = 0x03,
        TX_FAILURE = 0x04
    };

    Network();
    virtual ~Network();
    
    void Initialize(uint16_t id);

    void Service();

    void DiscoverAsync();

    void OnStatusChanged(Callback callback);
    
    ModemStatus GetStatus();

    bool FindRemoteDevice(const std::string& identifier, RemoteDevice* device); 
    bool FindRemoteDevice(const uint64_t address, RemoteDevice* device);
    
    
    LocalDevice* GetLocalDevice();
    
    ApiMode GetApiMode();
    
    Transaction* BeginTransaction(Device* device);
    Transaction* BeginTransaction();
    
    uint64_t GetTotalTransactions() const;
    
    SerialDataSubject* GetSerialDataSubject();
       
    void OnNext(const std::vector<uint8_t>& data);
    void OnNext(const uint8_t* data, const uint16_t len);
    void OnComplete();
    void OnError(const int32_t error_code);
    
    void Subscribe(NetworkObserver* observer);

protected:
    
    virtual void DeviceDiscovered(RemoteDevice* device);
    
    virtual void StatusChanged(ModemStatus status);
    
    virtual void SerialDataReceived(const uint64_t source_addr, const std::vector<uint8_t>& data);
    
private:
    
    static void DeviceTransactionComplete(Device* device, Transaction* transaction);
    
    uint16_t network_id;

    ModemStatus network_status;

    LocalDevice local_device;
    std::vector<RemoteDevice> remote_devices;
    
    Frame rx_frame;
    
    uint8_t frame_count;
    uint32_t frame_count_rollover;
    
    std::vector<Transaction> pending;
    
    std::vector<NetworkObserver*> subscribers;
    
    uint8_t rx_buff[RXBEE_RX_BUFFER_SIZE];
    uint8_t tx_buff[RXBEE_TX_BUFFER_SIZE];
    
    uint16_t rx_buff_head_index;
    uint16_t rx_buff_tail_index;
    uint16_t tx_buff_index;
    
    SerialDataSubject subject;
    
    Callback disc_comp_cb;
    Callback status_changed_cb;
};

} // namespace XBee

#endif // RXBEE_NETWORK_H