
#include <stdlib.h>
#include <string.h>

#include "Network.h"
#include "Transaction.h"
#include "SerialDataSubject.h"
#include "NetworkObserver.h"

#define RXBEE_MAX_FRAME_COUNT  (0xFF) // Maximum frame id before rollover

namespace RXBee
{

char print_buffer[256];

XBeeNetwork::XBeeNetwork()
    : network_status(ModemStatus::UNKNOWN),
      rx_buff_head_index(0), rx_buff_tail_index(0),
      tx_buff_index(0), frame_count(0),
      frame_count_rollover(0), api_mode(ApiMode::ESCAPED),
      print_handler(NULL), max_packet_payload_bytes(0x3D)
{
    rx_frame.Initialize(api_mode);
}


XBeeNetwork::~XBeeNetwork()
{

}


void XBeeNetwork::Service(uint32_t milliseconds)
{
    uint16_t i = 0;    
    uint16_t buff_max = rx_buff_tail_index;
//    
//    sprintf(print_buffer, "RXBee milliseconds [%d]", milliseconds);
//    Print(print_buffer);
//    
    if (pending.size() >= RXBEE_MAX_TRANSACTIONS)
    {
        for (;i < pending.size(); ++i)
        {
            if (pending[i] != NULL)
            {
                pending[i]->CompleteWithError(Transaction::Error::TRANSACTION_OVERFLOW);            
                delete pending[i];
            }
        }
        
        pending.clear();
        Print("RXBee: Transactions cleared!");
    }
    
    
    // Set max buff index to RXBEE_RX_BUFFER_SIZE if the
    // head index is greater than the tail
    if (rx_buff_head_index > rx_buff_tail_index)
    {
        buff_max = RXBEE_RX_BUFFER_SIZE;
    }
    
    // Read from receive buffer until head index equals tail index
    while (rx_buff_head_index != rx_buff_tail_index)
    {
        uint16_t tmp_head_idx = rx_buff_head_index;
        // Read up to maximum from receive buffer
        if (rx_frame.Deserialize(rx_buff, buff_max, rx_buff_head_index))
        {
            // Complete frame received
         
            Response::ApiFrame api_frame (&rx_frame);
            if (api_frame.extracted == true)
            {  
                if (api_frame.api_id == ApiID::RECEIVE_PACKET)
                {
                    // Received serial data
                    std::vector<uint8_t> frame_data;
                    Response::ReceivePacket packet(api_frame, frame_data);
                    SerialDataReceived(packet.sender_addr, frame_data); // Notify observers
                }
                else if (api_frame.api_id == ApiID::MODEM_STATUS)
                {
                    Response::ModemStatusUpdate modem_status(api_frame);
                    StatusChanged(modem_status.status);
                }
                else
                {
                    uint16_t p = 0; 
                    for (;p < pending.size(); ++p)
                    {
                        // Complete the corresponding transaction
                        if ((pending[p]->GetState() == Transaction::State::SENT) &&
                            (pending[p]->TryComplete(rx_frame)))
                        { 
                            break;
                        }
                    }

                    // Handle any AT responses
                    if ((api_frame.api_id == ApiID::AT_COMMAND_RESPONSE) ||
                        (api_frame.api_id == ApiID::REMOTE_AT_COMMAND_RESPONSE))
                    {
                        Response::ATCommand::Response at_rsp(api_frame);
                        
                        
                        
                        // These are local properties, save them internally.
                        switch (at_rsp.command)
                        {
                            case XBeeATCommand::ND:
                            {
                                Response::ATCommand::ND_Rsp rsp = at_rsp.ND();

                                if (rsp.address != local_addr)
                                {
                                    std::string id(rsp.node_identifier);
                                    DeviceDiscovered(rsp.address, id);
                                }
                                break;
                            }
                            case XBeeATCommand::ID:
                            {
                                Response::ATCommand::ID_Rsp rsp = at_rsp.ID();
                                network_id = rsp.network_id;
                                break;
                            }
                            case  XBeeATCommand::HP:
                            {
                                Response::ATCommand::HP_Rsp rsp = at_rsp.HP();
                                preamble_id = rsp.preamble_id;
                                break;
                            }
                            case XBeeATCommand::SH:
                            {
                                Response::ATCommand::SH_Rsp rsp = at_rsp.SH();
                                local_addr |= rsp.address;
                                break;
                            }
                            case XBeeATCommand::SL:
                            {
                                Response::ATCommand::SL_Rsp rsp = at_rsp.SL();
                                local_addr |= rsp.address;
                                break;
                            }
                            case XBeeATCommand::NI:
                            {
                                Response::ATCommand::NI_Rsp rsp = at_rsp.NI();
                                std::strcpy(node_identifier, rsp.node_identifier); 
                                break;
                            }
                            case XBeeATCommand::NP:
                            {
                                Response::ATCommand::NP_Rsp rsp = at_rsp.NP();
                                max_packet_payload_bytes = rsp.max_rf_payload_bytes;
                                break; 
                            }
                        }
                    }
                }
            }
            
            // reset the receive frame
            rx_frame.Initialize(api_mode);
            break;
        }
        else if (tmp_head_idx == rx_buff_head_index)
        {
            // Did not deserialize any more data, wait for more
            break;
        }
        else
        {
            // Nothing
        }
        
        // Reset head index when it reaches the end of the buffer
        if (rx_buff_head_index >= RXBEE_RX_BUFFER_SIZE)
        {
            rx_buff_head_index = 0;         // Start at front of buffer
            buff_max = rx_buff_tail_index;  // Max is now the tail index
        }
    }
    
    i = 0;
    for (; i < pending.size(); ++i)
    {                
        if ((pending[i]->GetState() == Transaction::State::SENT) &&
            (pending[i]->HasTimeoutExpired(milliseconds)))
        {
            if (pending[i]->Retry())
            {
                sprintf(print_buffer, "RXBee: Transaction Timeout, Retrying : %d", i);
                Print(print_buffer);
                break;
            }
            else
            {
                pending[i]->CompleteWithError(Transaction::Error::TRANSACTION_TIMEOUT);
                sprintf(print_buffer, "RXBee: Transaction Timeout : %d", i);
                Print(print_buffer);
            }
        }
    }
    
    if (i >= pending.size())
    {
        i = 0;
        for (; i < pending.size(); ++i)
        { 
            if (pending[i]->GetState() == Transaction::State::CHAINED)
            {
                break;
            }
        }
    }
    
    if (i >= pending.size())
    {
        std::vector<Address> exclusion;
        bool exclude = false;
        // Send pending transactions
        i = 0;
        for (; i < pending.size(); ++i)
        {
            if (pending[i]->GetState() == Transaction::State::SENT)
            {
                exclusion.push_back(pending[i]->GetDestination());
            }
        }
        i = 0;
        for (; i < pending.size(); ++i)
        { 
            if (pending[i]->GetState() == Transaction::State::PENDING)
            {
                for (uint16_t e = 0; e < exclusion.size(); ++e)
                {
                    if (pending[i]->GetDestination() == exclusion[e])
                    {
                        exclude = true;
                        break;
                    }
                }
            
                if (!exclude)
                {
                    break;
                }
            }
        }
    }
    
    if (i < pending.size())
    {  
        frame_count++;

        Frame* f = pending[i]->GetFrame();

        f->SetFrameID(frame_count);

        // Write frame to transmit buffer 
        subject.Next(f->Serialize());

        // Transaction sent
        pending[i]->Sent(frame_count);
#if RXBEE_DEBUG
        uint64_t dest_addr = pending[i]->GetDestination();
        uint32_t frame_id = pending[i]->GetFrameID();
        sprintf(print_buffer, "RXBee: Transaction[%d] sent to %llx, id => %d", i, dest_addr, frame_id);
        Print(print_buffer);
#endif
        // Increment frame count
        if (frame_count == RXBEE_MAX_FRAME_COUNT)
        {
            // Handler overflow
            frame_count = 0;
            frame_count_rollover++;
        }
    }
}

uint64_t XBeeNetwork::GetTotalTransactions() const
{
    return frame_count_rollover * RXBEE_MAX_FRAME_COUNT + frame_count;
}
    
SerialDataSubject* XBeeNetwork::GetSerialDataSubject()
{
    return &subject;
}


Transaction* XBeeNetwork::DiscoverAsync()
{
    BeginTransaction()->ReadAddressUpper()->ReadAddressLower()->NetworkDiscover()->Pend();
}


void XBeeNetwork::OnStatusChanged(XBeeNetwork::Callback callback)
{
    status_changed_cb = callback;
}

ModemStatus XBeeNetwork::GetStatus()
{
    return network_status;
}

uint16_t XBeeNetwork::GetNetworkID() const
{
    return network_id;
}

uint8_t XBeeNetwork::GetPreambleID() const
{
    return preamble_id;
}

Address XBeeNetwork::GetLocalAddress() const
{
    return local_addr;
}

ApiMode XBeeNetwork::GetApiMode()
{
    return api_mode;
}

Transaction* XBeeNetwork::BeginTransaction(Address addr)
{
    uint16_t i = 0; 
    Transaction* t = NULL;
    
    for (;i < pending.size(); ++i)
    {
        if (pending[i]->GetState() == Transaction::State::FREE)
        {
            delete pending[i];
            t = new Transaction();
            pending[i] = t;
            break;
        }
    }
    
    if ((t == NULL))
    {
        t = new Transaction();
        
        pending.push_back(t);
    }
            
    if (t != NULL)
    {
        
#if RXBEE_DEBUG
        sprintf(print_buffer, "RXBee: Transaction created[%d]", i);
        Print(print_buffer);
#endif
        t->Initialize(addr, this);
    }
    
    return t;
}

Transaction* XBeeNetwork::BeginTransaction()
{
    return BeginTransaction(RXBEE_LOCAL_ADDRESS);
}

Transaction* XBeeNetwork::BeginBroadcastTransaction()
{
    return BeginTransaction(XBEE_BROADCAST_ADDRESS);
}

void XBeeNetwork::SerialDataReceived(const uint64_t source_addr, const std::vector<uint8_t>& data)
{
    for(uint16_t i = 0; i < subscribers.size(); ++i)
    {
        subscribers[i]->OnSerialDataReceived(source_addr, data);
    }
}

void XBeeNetwork::OnNext(const std::vector<uint8_t>& data)
{
    OnNext(&data[0], data.size());
}

void XBeeNetwork::OnNext(const uint8_t* data, const uint16_t len)
{
    for(uint16_t i = 0; i < len; ++i)
    {
        rx_buff[rx_buff_tail_index] = data[i];
        
        rx_buff_tail_index++;
        
        if (rx_buff_tail_index >= RXBEE_RX_BUFFER_SIZE)
        {
            rx_buff_tail_index = 0;
        }
        
        if (rx_buff_tail_index == rx_buff_head_index)
        {
            // Overrun, reset rx_frame and rx_buff
            rx_frame.Initialize(api_mode);
            Print("RXBee: RX overrun, buffer reset");
            rx_buff_tail_index = 0;
            rx_buff_head_index = 0;
            break;
        }
    }
}
    
void XBeeNetwork::OnComplete()
{
    
}

void XBeeNetwork::OnError(const int32_t error_code)
{
    
}


void XBeeNetwork::Subscribe(NetworkObserver* observer)
{
    subscribers.push_back(observer);
}
    
void XBeeNetwork::RegisterPrintHandler(XBeeNetwork::PrintCallback handler)
{
    print_handler = handler;
}

void XBeeNetwork::Print(const char* msg)
{
    if (print_handler != NULL) { print_handler(msg); }
}

uint16_t XBeeNetwork::GetMaxPacketPayloadBytes() const
{
    return max_packet_payload_bytes;
}

void XBeeNetwork::DeviceDiscovered(Address address, const std::string& node_id)
{
    for (uint16_t i = 0; i < subscribers.size(); ++i)
    {
        subscribers[i]->OnDeviceDiscovered(this, address, node_id);
    }
}

void XBeeNetwork::StatusChanged(ModemStatus status)
{
    ModemStatus prev = network_status;
    network_status = status;
    
    for (uint16_t i = 0; i < subscribers.size(); ++i)
    {
        subscribers[i]->OnStatusChanged(this, prev, network_status);
    }
}

} // namespace XBee