
#ifndef RXBEE_DEVICE_H
#define RXBEE_DEVICE_H

#include <stdint.h>

#include "Frame.h"

namespace RXBee
{    

class Transaction;
class Network;

class Device
{
public:
    enum class Location
    {
        LOCAL,
        REMOTE
    };
    
    enum class RoutingMode
    {
        DEFAULT = 0,
        INDIRECT_MSG_COORDINATOR = 1,
        DISABLED = 2,
        INDIRECT_MSG_POLLER = 4
    };


    struct VersionInformation
    {
        uint16_t firmware_version;
        uint16_t hardware_version;
        uint16_t hardware_series;
        uint32_t device_type_id;
        uint16_t max_packet_payload;
        uint16_t configration_crc;
    };


    struct SerialInterface
    {
        uint8_t baud_rate ;
        uint8_t parity;
        uint8_t stop_bits;
        uint8_t packetization_timeout;
        uint16_t flow_control_threshold;
        uint8_t api_mode;
        uint8_t api_options;
    };


    struct IO
    {
        uint8_t io_config[14];
        uint16_t pull_dir;
        uint16_t pull_enable;
        uint16_t pwm_0_duty_cycle;
        uint16_t pwm_1_duty_cycle;
        uint8_t associate_blink_time;
        uint8_t rssi_pwm_timer;
    };

    Device();
    Device(const Device& d);
    virtual ~Device();
    
    Device& operator=(const Device& d);

    virtual Location GetLocation() const =  0;
    uint64_t GetAddress();    
    
    Transaction* Pend();

    void ReadVersionInfo();

    VersionInformation GetVersionInfo();
    
    void SetNetwork(Network* n);
    
protected:
    
    friend class Network;
    
    void SetAddress(uint64_t addr);
    
    typedef void (*TranscationCompleteCallback)(Device* d, Transaction* t);
    
    void InitFrame(Frame& f);
    
    void InitTransaction(Transaction* t, Frame& f);
    
    template<typename ...Ts> 
    Device* CreateATCmdTransaction(const char* const at_cmd, Ts... fields)
    {
        Transaction* t = GetNextTransaction();  
        Frame f;
       
        InitFrame(f);
        
        f.AddField(at_cmd);
        f.AddFields(fields...);

        InitTransaction(t, f);

        return this;
    }
    
    Transaction* GetNextTransaction();
    
    void SetTransactionFrame(Transaction* t, const Frame& f) const;
    
    void SetQueueCommands(bool value);
    
    void OnTransactionComplete(TranscationCompleteCallback callback);

private:
    
    static void HandleTransactionComplete(Transaction* t, Device* d);

    void SetVersionInfo();
    
    bool queue_commands;
    
    uint64_t dev_addr;
    
    Network* network;

    struct Configuration
    {
        VersionInformation version_info;
    };

    Configuration config;
    
    
    Transaction* head;
    Transaction* tail;
    
    TranscationCompleteCallback trans_comp_cb;
};

} // namespace XBee

#endif // RXBEE_DEVICE_H