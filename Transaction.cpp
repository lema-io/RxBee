
#include <stdlib.h>

#include "Transaction.h"

namespace RXBee
{

void Transaction::OnComplete(Transaction::CompleteHandler handler)
{
    on_complete_handler = handler;
    
    if (state == State::COMPLETE)
    {
        Complete();
    }
}

Transaction::Transaction()
    : target_frame_id(0), on_complete_handler(NULL), dest(NULL), 
        err(Error::NONE), chain(NULL), state(State::FREE)
{
    
}
Transaction::Transaction(const Transaction& t)
{
    current_frame = t.current_frame;
    target_frame_id = t.target_frame_id;
    on_complete_handler = t.on_complete_handler;
    dest = t.dest;
    err = t.err;
    chain = t.chain;
    state = t.state;
}

Transaction::~Transaction()
{
    
}

Transaction& Transaction::operator=(Transaction& t)
{
    current_frame = t.current_frame;
    target_frame_id = t.target_frame_id;
    on_complete_handler = t.on_complete_handler;
    dest = t.dest;
    err = t.err;
    chain = t.chain;
    state = t.state;
}

void Transaction::Initialize(Device* destination)
{
    target_frame_id = 0;
    on_complete_handler = NULL;
    dest = destination; 
    err = Error::NONE;
    chain = NULL;
    current_frame.Clear();
    state = State::INITIALIZED;
}
    
const Frame& Transaction::GetFrame() const
{
    return current_frame;
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
    
    if (frame.GetID() == target_frame_id)
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
    current_frame.SetID(frame_id);
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
        on_complete_handler(this, dest);
    }
    
    state = State::FREE;
}


void Transaction::Chain(Transaction* t)
{
    chain = t;
    OnComplete(HandleChainComplete);
}
    
void Transaction::Pend()
{
    state = State::PENDING;
}
    
void Transaction::OnCompleteDevice(Transaction::CompleteHandler handler)
{
    on_complete_device_handler = handler;
}

void Transaction::HandleChainComplete(Transaction* transaction,
                                      Device* dest_device)
{
    transaction->chain->SetError(transaction->GetError());
    transaction->chain->Pend();
}


} // namespace RXBee