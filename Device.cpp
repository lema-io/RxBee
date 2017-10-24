
#include <stdlib.h>

#include "Device.h"
#include "Transaction.h"
#include "Network.h"

namespace RXBee
{

Device::Device()
    : head(NULL), tail(NULL), network(NULL),
      trans_comp_cb(NULL)
{
    node_id = "";
}

Device::Device(const Device& d)
{
    head = d.head;
    tail = d.tail;
    queue_commands = d.queue_commands;
    dev_addr = d.dev_addr;
    network = d.network;
    node_id = d.node_id;
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
    node_id = d.node_id;
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
    
const std::string& Device::GetNodeIdentifier() const
{
    return node_id;
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
    
    if ((tail != NULL) && 
        (tail->GetState() > Transaction::State::FREE) &&
        (tail->GetState() < Transaction::State::SENT))
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


Transaction* Device::GetNextCmdTransaction()
{
    Transaction* t = GetNextTransaction();
    
    ApiID frame_id = ApiID::AT_COMMAND;

    if (GetLocation() == Location::REMOTE)
    {
        frame_id = ApiID::REMOTE_AT_COMMAND;

    }
    else if (queue_commands)
    {
        frame_id = ApiID::AT_QUEUE_COMMAND;
    }

    Frame* f = t->GetFrame();
    f->Initialize(frame_id, network->GetApiMode());
    
    if (GetLocation() == Location::REMOTE)
    {
        f->AddFields(dev_addr,
                     static_cast<uint16_t>(0xFFFE),
                     static_cast<uint8_t>(0));
    }
    
    t->OnCompleteDevice(HandleTransactionComplete);
    
    return t;
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
        Frame* f = t->GetFrame();
        if (f->GetApiID() == ApiID::AT_COMMAND_RESPONSE)
        {
        }
    }
    
    if (t->chain != NULL)
    {
        d->head = t->chain;
    }
    else if (d->tail != NULL)
    {
        d->head = d->tail;
    }
    else
    {
        d->head = NULL;
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

Network* Device::GetNetwork()
{
    return network;
}

void Device::RecordNodeIdentifier(const std::string& identifier)
{
    node_id = identifier;
}
    
void Device::OnTransactionComplete(Device::TranscationCompleteCallback callback)
{
    trans_comp_cb = callback;
}


} // namespace RXBee
