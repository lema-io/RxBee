

#ifndef RXBEE_PENDING_TRANSACTION_H
#define RXBEE_PENDING_TRANSACTION_H

#include <stdint.h>
#include <cstdint>
#include "Frame.h"

namespace RXBee
{
    
class Device;
class Network;

class Transaction
{
public:
    
    enum class Error
    {
        NONE,
        INVALID_DESTINATION,
        INVALID_TRANSACTION_STATE,
        AT_CMD_ERROR,
        AT_CMD_INVALID_COMMAND,
        AT_CMD_INVALID_PARAMETER,
        AT_CMD_TX_FAILURE
    };
    
    enum class Action
    {
        SET,
        SET_QUEUE,
        QUERY,
    };
    
    Transaction();
    Transaction(const Transaction& t);
    ~Transaction();
    
    void Initialize(Device* destination);
    
    Transaction& operator=(Transaction& t);
    
    typedef void (*CompleteHandler)(Transaction* transaction, 
                                    Device* dest_device,
                                    void* context);
    
    Device* GetDestinationDevice();
   
    
    void OnComplete(CompleteHandler handler, void* context);
    
    Frame* GetFrame();
    
    Error GetError() const;
    
protected:
    friend class Network;
    friend class Device;
    
    enum class State
    {
        FREE,
        INITIALIZED,
        FRAMED,
        PENDING,
        SENT,
        COMPLETE
    };
    
    State GetState() const;
    
    void Sent(uint16_t frame_id);
    
    bool TryComplete(const Frame& frame);
    
    void SetError(Error error);
    
    void SetFrame(const Frame& frame);
    
    void Complete();
    
    void Chain(Transaction* t);
    
    void Pend();
    
    
    
    typedef void (*CompleteDeviceHandler)(Transaction* transaction, 
                                          Device* dest_device);
    
    void OnCompleteDevice(CompleteDeviceHandler handler);
    
private:
    static void HandleChainComplete(Transaction* transaction, Device* dest_device, void* context);
    Frame current_frame;
    uint16_t target_frame_id;
    CompleteHandler on_complete_handler;
    CompleteDeviceHandler on_complete_device_handler;
    Device* dest;
    Error err;
    State state;
    Transaction* chain;
    void* on_complete_context;
};
    
} // namespace RXBee

#endif // RXBEE_PENDING_TRANSACTION_H