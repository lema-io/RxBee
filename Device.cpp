
#include <stdlib.h>

#include "Device.h"
#include "Transaction.h"
#include "Network.h"

namespace RXBee
{

Device::Device()
    : head(NULL), tail(NULL)
{

}

Device::Device(const Device& d)
{
    head = d.head;
    tail = d.tail;
    queue_commands = d.queue_commands;
    dev_addr = d.dev_addr;
    network = d.network;
    trans_comp_cb = d.trans_comp_cb;
}

Device::~Device()
{
    
}
    
Device& Device::operator=(const Device& d)
{
    head = d.head;
    tail = d.tail;
    queue_commands = d.queue_commands;
    dev_addr = d.dev_addr;
    network = d.network;
    trans_comp_cb = d.trans_comp_cb;    
}
    
void Device::SetNetwork(Network* n)
{
    network = n;
}

uint64_t Device::GetAddress()
{
    return dev_addr;
}

void Device::SetAddress(uint64_t addr)
{
    dev_addr = addr;
}

Transaction* Device::Pend()
{
    if ((head != NULL) && (head->GetState() == Transaction::State::INITIALIZED))
    {
        head->Pend();
    }
    
    return tail;
}

Transaction* Device::GetNextTransaction()
{
    Transaction* t = network->BeginTransaction(this);
    
    if ((tail != NULL) && (tail->GetState() == Transaction::State::INITIALIZED))
    {
        tail->Chain(t);       
    }
    
    if (head == NULL)
    {
        head = t;
    }
    
    tail = t;
    
    return t;
}
    
void Device::InitFrame(Frame& f)
{
    ApiID frame_id = ApiID::AT_COMMAND;

    if (GetLocation() == Location::REMOTE)
    {
        frame_id = ApiID::REMOTE_AT_COMMAND;

    }
    else if (queue_commands)
    {
        frame_id = ApiID::AT_QUEUE_COMMAND;
    }

    f.Initialize(frame_id, network->GetApiMode());
    
    if (GetLocation() == Location::REMOTE)
    {
        f.AddField(dev_addr);
        f.AddField(0xFFFE);
        f.AddField((uint8_t)0);
    }
}

void Device::InitTransaction(Transaction* t, Frame& f)
{
    t->SetFrame(f);
    t->OnCompleteDevice(HandleTransactionComplete);
}


void Device::SetTransactionFrame(Transaction* t, const Frame& f) const
{
    t->SetFrame(f);
}

void Device::HandleTransactionComplete(Transaction* t, Device* d)
{
    Transaction::Error err = t->GetError();
    
    if (err == Transaction::Error::NONE)
    {
        Frame f = t->GetFrame();
        if (f.GetID() == static_cast<uint8_t>(ApiID::AT_COMMAND_RESPONSE))
        {

        }
    }
    
    if (d->trans_comp_cb != NULL)
    {
        d->trans_comp_cb(d, t);
    }
}
    
void Device::SetQueueCommands(bool value)
{
    queue_commands = value;
}
    
void Device::OnTransactionComplete(Device::TranscationCompleteCallback callback)
{
    trans_comp_cb = callback;
}



} // namespace RXBee
