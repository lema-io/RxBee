
#include <stdlib.h>

#include "Transaction.h"

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
        on_complete_device_handler(NULL), dest(NULL), 
        err(Error::NONE), chain(NULL), state(State::FREE),
        on_complete_context(NULL)
{
    
}
Transaction::Transaction(const Transaction& t)
{
    current_frame = t.current_frame;
    target_frame_id = t.target_frame_id;
    on_complete_handler = t.on_complete_handler;
    on_complete_device_handler = t.on_complete_device_handler;
    dest = t.dest;
    err = t.err;
    chain = t.chain;
    state = t.state;
    on_complete_context = t.on_complete_context;
}

Transaction::~Transaction()
{
    
}

Transaction& Transaction::operator=(Transaction& t)
{
    current_frame = t.current_frame;
    target_frame_id = t.target_frame_id;
    on_complete_handler = t.on_complete_handler;
    on_complete_device_handler = t.on_complete_device_handler;
    dest = t.dest;
    err = t.err;
    chain = t.chain;
    state = t.state;
    on_complete_context = t.on_complete_context;
}

void Transaction::Initialize(Device* destination)
{
    target_frame_id = 0;
    on_complete_handler = NULL;
    on_complete_context = NULL;
    dest = destination; 
    err = Error::NONE;
    chain = NULL;
    current_frame.Clear();
    state = State::INITIALIZED;
}
    
Frame* Transaction::GetFrame()
{
    return &current_frame;
}
    
Transaction::Error Transaction::GetError() const
{
    return err;
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
        current_frame = frame;
        
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
    current_frame.Clear();
}

void Transaction::Complete()
{
    state = State::COMPLETE;
    
    if (on_complete_device_handler != NULL)
    {
        on_complete_device_handler(this, dest);
    }
    
    if (on_complete_handler != NULL)
    {
        on_complete_handler(this, dest, on_complete_context);
    }
    
    state = State::FREE;
}


void Transaction::Chain(Transaction* t)
{
    chain = t;
    OnComplete(HandleChainComplete, on_complete_context);
}
    
void Transaction::Pend()
{
    state = State::PENDING;
}
    
void Transaction::OnCompleteDevice(Transaction::CompleteDeviceHandler handler)
{
    on_complete_device_handler = handler;
}

void Transaction::HandleChainComplete(Transaction* transaction,
                                      Device* dest_device,
                                      void* context)
{
    transaction->chain->SetError(transaction->GetError());
    transaction->chain->Pend();
}


} // namespace RXBee