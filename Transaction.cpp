
#include <stdlib.h>

#include "Transaction.h"
#include "Network.h"
#include "Command.h"

namespace RXBee
{

void Transaction::OnComplete(Transaction::CompleteHandler handler, void* context)
{
    on_complete_handler = handler;
    on_complete_context = context;
    
    if (state == State::COMPLETE)
    {
        Complete();
    }
}

Transaction::Transaction()
    : target_frame_id(0), on_complete_handler(NULL), 
        dest_addr(RXBEE_LOCAL_ADDRESS),
        err(Error::NONE), state(State::FREE),
        on_complete_context(NULL), prev(NULL), next(NULL), queue_cmds(false),
        apply_timeout(true)
{
    
}

Transaction::Transaction(const Transaction& t)
{
    current_frame = t.current_frame;
    target_frame_id = t.target_frame_id;
    on_complete_handler = t.on_complete_handler;
    err = t.err;
    state = t.state;
    on_complete_context = t.on_complete_context;
    dest_addr = t.dest_addr;
    prev = t.prev;
    next = t.next;
    queue_cmds = t.queue_cmds;
    apply_timeout = t.apply_timeout;
    timeout_remaining = t.timeout_remaining;
}

Transaction::~Transaction()
{
    
}

Transaction& Transaction::operator=(Transaction& t)
{
    current_frame = t.current_frame;
    target_frame_id = t.target_frame_id;
    on_complete_handler = t.on_complete_handler;
    err = t.err;
    state = t.state;
    on_complete_context = t.on_complete_context;
    dest_addr = t.dest_addr;
    prev = t.prev;
    next = t.next;
    queue_cmds = t.queue_cmds;
    apply_timeout = t.apply_timeout;
    timeout_remaining = t.timeout_remaining;
}

void Transaction::Initialize(Address destination, XBeeNetwork* network)
{
    target_frame_id = 0;
    on_complete_handler = NULL;
    on_complete_context = NULL;
    dest_addr = destination;
    err = Error::NONE;
    current_frame.Clear();
    state = State::INITIALIZED;
    net = network;
    prev = NULL;
    next = NULL;
    apply_timeout = true;
    timeout_remaining = 300;
}
    
Frame* Transaction::GetFrame()
{
    return &current_frame;
}

uint16_t Transaction::GetFrameID() const
{
    return target_frame_id;
}
    
Transaction::Error Transaction::GetError() const
{
    return err;
}

Address Transaction::GetDestination() const
{
    return dest_addr;
}
    
Transaction::State Transaction::GetState() const
{
    return state;
}
    
bool Transaction::TryComplete(const Frame& frame)
{
    bool completed = false;
    
    if (frame.GetFrameID() == target_frame_id)
    {
        current_frame.Clear();
        current_frame = frame;
        
        char buffer[30];
        sprintf(buffer, "Transaction completed, id => %d", target_frame_id);
        net->Print(buffer);
        
        Complete();
    }
    
    return completed;
}
    
void Transaction::SetError(Transaction::Error error)
{
    err = error;
}

void Transaction::SetFrame(const Frame& frame)
{
    state = State::FRAMED;
    current_frame = frame;
}


void Transaction::Sent(uint16_t frame_id)
{
    target_frame_id = frame_id;
    state = State::SENT;
}

void Transaction::CompleteWithError(Transaction::Error error)
{
    err = error;
    state = State::ERROR;
    
    if (on_complete_handler != NULL)
    {
        on_complete_handler(this, on_complete_context);
    }
    
    state = State::FREE;
}

void Transaction::Complete()
{
    state = State::COMPLETE;
    
    if (on_complete_handler != NULL)
    {
        on_complete_handler(this, on_complete_context);
    }
    
    state = State::FREE;
}


void Transaction::Chain(Transaction* chain)
{
    Transaction* t = this;
    while (t->next != NULL)
    { 
        t = t->next; 
    }
    t->next = chain;
    
    if (chain != NULL)
    {
        chain->prev = t;
    }
    t->OnComplete(HandleChainComplete, on_complete_context);
}
    
Transaction* Transaction::Pend()
{
    Transaction* t = this;
    while ((t->prev != NULL) && (t->prev->state == State::FRAMED))
    { 
        t = t->prev; 
    }
    
    t->state = State::PENDING;
    if ((t->next != NULL) && (t->next->state == State::CHAINED))
    {
        t->next->state = State::FRAMED;
    }
    t->timeout_remaining = 300;
    
    return this;
}


Transaction* Transaction::GetNext()
{
    return next;
}


    
bool Transaction::HasTimeoutExpired(int32_t elapsed)
{
    if (apply_timeout)
    {
        if (elapsed < timeout_remaining)
        {
            timeout_remaining -= elapsed;
        }
        else
        {
            timeout_remaining = 0;
            state = State::TIMEOUT;
        }
    }
    
    return apply_timeout && (state == State::TIMEOUT);
}

void Transaction::HandleChainComplete(Transaction* transaction,
                                      void* context)
{
    if ((transaction != NULL) && (transaction->next != NULL))
    {
        if (transaction->state == State::ERROR)
        {
            transaction->next->CompleteWithError(transaction->err);
        }
        else
        {
            transaction->next->SetError(transaction->err);
            transaction->next->state = State::CHAINED;
        }
    }
}

Transaction* Transaction::WritePreambleID(uint8_t id) 
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddFields(XBEE_CMD_HP, id); 
    return t; 
}

Transaction* Transaction::ReadPreambleID()
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddFields(XBEE_CMD_HP); 
    return t; 
}

Transaction* Transaction::WriteNetworkID(uint16_t id)
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddFields(XBEE_CMD_ID, id); 
    return t; 
}

Transaction* Transaction::ReadNetworkID()
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddField(XBEE_CMD_ID);  
    return t; 
}

Transaction* Transaction::WriteRoutingMode(uint8_t mode)
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddFields(XBEE_CMD_CE, mode);  
    return t; 
}

Transaction* Transaction::ReadRoutingMode()
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddField(XBEE_CMD_CE);  
    return t; 
}

Transaction* Transaction::WriteIdentifier(const std::string& identifier)
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddFields(XBEE_CMD_NI, identifier);  
    return t; 
} 

Transaction* Transaction::ReadIdentifier()
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddField(XBEE_CMD_NI);  
    return t; 
} 

Transaction* Transaction::ReadAddressUpper()
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddField(XBEE_CMD_SH);  
    return t; 
} 

Transaction* Transaction::ReadAddressLower()
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddField(XBEE_CMD_SL);  
    return t; 
} 
    
Transaction* Transaction::NetworkDiscover()
{
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddField(XBEE_CMD_ND);  
    t->apply_timeout = false;
    return t;   
}

Transaction* Transaction::WriteApiMode(ApiMode mode)
{
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddFields(XBEE_CMD_AP, static_cast<uint8_t>(mode));  
    return t;   
}

Transaction* Transaction::ReadApiMode()
{
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddField(XBEE_CMD_AP);  
    return t;   
}

Transaction* Transaction::BeginCommandQueue() 
{ 
    queue_cmds = true; 
    return this;
}

Transaction* Transaction::EndCommandQueue()
{ 
    Transaction* t = GetNextCmdTransaction();
    t->GetFrame()->AddField(XBEE_CMD_AC); 
    return t; 
} 


Transaction* Transaction::GetNextTransaction()
{
    Transaction* t = NULL;
    
    if (state == Transaction::State::INITIALIZED)
    {
        t = this;
    }
    else if (net != NULL)
    {
        t = net->BeginTransaction(dest_addr);
        Chain(t);       
    }
    
    t->state = State::FRAMED;
    
    return t;
}


Transaction* Transaction::GetNextCmdTransaction()
{
    Transaction* t = GetNextTransaction();
    
    if (t != NULL)
    {
        ApiID frame_id = ApiID::AT_COMMAND;

        if (dest_addr != 0)
        {
            frame_id = ApiID::REMOTE_AT_COMMAND;

        }
        else if (queue_cmds)
        {
            frame_id = ApiID::AT_QUEUE_COMMAND;
        }

        Frame* f = t->GetFrame();
        f->Initialize(frame_id, net->GetApiMode());

        if (dest_addr != 0)
        {
            f->AddFields(dest_addr,
                         static_cast<uint16_t>(0xFFFE),
                         static_cast<uint8_t>(0));
        }
    }
    
    return t;
}

Transaction* Transaction::Transmit(const uint8_t* buffer, uint16_t n)
{
    Transaction* t = GetNextTransaction(); 

    if (t != NULL)
    {
        Frame* f = t->GetFrame();
        f->Initialize(ApiID::TRANSMIT_REQUEST, net->GetApiMode());

        f->AddFields(dest_addr,
                     static_cast<uint16_t>(0xFFFE), // Reserved
                     static_cast<uint8_t>(0),       // Max hops on broadcast
                     static_cast<uint8_t>(0xC0));   // Delivery method = DigiMesh

        if (n > 0x3D)
        {
            // Transmit up to max payload bytes
            f->AddData(buffer, 0x3D);

            // Chain next section of data

            Transmit(&buffer[0x3D], n - 0x3D);
        }
        else
        {
            // Transmit entire data
            f->AddData(buffer, n);

            Pend();
        }
    }
    
    return t;
}


} // namespace RXBee