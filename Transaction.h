

#ifndef RXBEE_PENDING_TRANSACTION_H
#define RXBEE_PENDING_TRANSACTION_H

#include <stdint.h>
#include <cstdint>
#include "Frame.h"
#include "Types.h"

namespace RXBee
{

class XBeeNetwork;

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
    
    void Initialize(Address destination, XBeeNetwork* network);
    
    Transaction& operator=(Transaction& t);
    
    typedef void (*CompleteHandler)(Transaction* transaction,
                                    void* context);   
    
    void OnComplete(CompleteHandler handler, void* context);
    
    Frame* GetFrame();
    
    Error GetError() const;
    
    Address GetDestination() const;
    
    uint16_t GetFrameID() const;
    
    Transaction* WritePreambleID(uint8_t id);
    Transaction* ReadPreambleID();

    Transaction* WriteNetworkID(uint16_t id);
    Transaction* ReadNetworkID();

    Transaction* WriteRoutingMode(uint8_t mode);
    Transaction* ReadRoutingMode();

    Transaction* WriteIdentifier(const std::string& identifier);
    Transaction* ReadIdentifier();
    
    Transaction* ReadAddressUpper();
    Transaction* ReadAddressLower();
    
    Transaction* NetworkDiscover();
    
    Transaction* WriteApiMode(ApiMode mode);
    Transaction* ReadApiMode();

    Transaction* BeginCommandQueue();

    Transaction* EndCommandQueue();
    
    
    Transaction* Transmit(const uint8_t* buffer, uint16_t n);
    
    Transaction* Pend();
    
    Transaction* GetNext();
    
    bool HasTimeoutExpired(int32_t elapsed);
    
protected:
    friend class XBeeNetwork;
    
    enum class State
    {
        FREE,
        INITIALIZED,
        FRAMED,
        PENDING,
        CHAINED,
        SENT,
        TIMEOUT,
        COMPLETE
    };
    
    State GetState() const;
    
    void Sent(uint16_t frame_id);
    
    bool TryComplete(const Frame& frame);
    
    void SetError(Error error);
    
    void SetFrame(const Frame& frame);
    
    void Complete();
    
    void Chain(Transaction* t);
    
    XBeeNetwork* net;
    
    Transaction* GetNextTransaction();    
    Transaction* GetNextCmdTransaction();
    
private:
    static void HandleChainComplete(Transaction* transaction, void* context);
    Frame current_frame;
    uint16_t target_frame_id;
    CompleteHandler on_complete_handler;
    Address dest_addr;
    Error err;
    State state;
    void* on_complete_context;
    bool queue_cmds;
    Transaction* prev;
    Transaction* next;
    bool apply_timeout;
    int32_t timeout_remaining;
};
    
} // namespace RXBee

#endif // RXBEE_PENDING_TRANSACTION_H