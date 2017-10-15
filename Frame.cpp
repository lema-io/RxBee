#include <sstream>

#include "Frame.h"

namespace RXBee
{

// Frame
// Constructor
Frame::Frame()
    : mode(ApiMode::TRANSPARENT)
{
}

Frame::~Frame()
{
}

Frame::Frame(const Frame& other)
{
    mode = other.mode;
    data = other.data;
}

Frame& Frame::operator=(const Frame& l)
{
    mode = l.mode;
    data = l.data;
}

void Frame::Initialize(const ApiID id, const ApiMode api_mode)
{
    mode = api_mode;
    if (mode != ApiMode::TRANSPARENT)
    {
        data.push_back(XBEE_PACKET_START);
        data.push_back(0);  // Length MSB
        data.push_back(0);  // Length LSB
        data.push_back(static_cast<uint8_t>(id)); // Frame Type
        data.push_back(0);  // Frame ID
    }
    else
    {
        Clear();
    }
}



void Frame::AddData(const uint8_t* bytes, const uint16_t byte_cnt)
{
    uint16_t i = 0; 
    for(; i < byte_cnt; ++i)
    {
        data.push_back(bytes[i]);
    }
    AddSize(i);
}

void Frame::AddData(const std::vector<uint8_t>& bytes)
{
     uint16_t i = 0; 
    for(; i < bytes.size(); ++i)
    {
        data.push_back(bytes[i]);
    }
    AddSize(i);
    
}




void Frame::AddField(const char* const field)
{
    uint16_t i = 0;
    for(; field[i] != '\0'; ++i)
    {
        data.push_back(field[i]);
    }

    data.push_back('\0');
    AddSize(i + 1);
}

void Frame::AddField(const std::string& field)
{
    AddField(field.c_str());
}

bool Frame::GetFields() const
{
    return true;
}



bool Frame::GetField(const uint16_t index, char* field, uint16_t& length,
              const  uint16_t max_length) const
{
    bool success = false;
    if (data.size() > XBEE_FRAME_API_CONTENT_INDEX + index)
    {
        for (length = 0; (length < GetSize()) && (length < max_length) ; ++length)
        {
            field[length] = data[XBEE_FRAME_API_CONTENT_INDEX + index + length];
        }
        success = true;
    }
    
    return success;
}

bool Frame::GetField(const uint16_t index, std::string& field, uint16_t& length) const
{
    std::stringstream ss;
    bool success = false;
    if (data.size() > XBEE_FRAME_API_CONTENT_INDEX + index)
    {
        for (length = 0; length < GetSize(); ++length)
        {
            ss << data[XBEE_FRAME_API_CONTENT_INDEX + index + length];
        }
        field.assign(ss.str());
        success = true;
    }
    return true;
}

bool Frame::GetData(const uint16_t index, uint8_t* bytes, uint16_t& length,
             uint16_t max_length) const
{
    bool success = false;
    if (data.size() > XBEE_FRAME_API_CONTENT_INDEX + index)
    {
        for (length = 0; (length < GetSize()) && (length < max_length) ; ++length)
        {
            bytes[length] = data[XBEE_FRAME_API_CONTENT_INDEX + index + length];
        }
        success = true;
    }
    
    return success;
}

bool Frame::GetData(const uint16_t index, std::vector<uint8_t>& bytes) const
{
    bool success = false;
    if (data.size() > XBEE_FRAME_API_CONTENT_INDEX + index)
    {
        for (uint16_t i = 0; (i  < GetSize()) ; ++i )
        {
            bytes.push_back(data[XBEE_FRAME_API_CONTENT_INDEX + index + i ]);
        }
        success = true;
    }
    
    return success;
}

uint16_t Frame::GetSize() const
{
    uint16_t size = 0;
    
    if ((data.size() > XBEE_FRAME_API_LENGTH_LSB) &&
        (mode != ApiMode::TRANSPARENT))
    {
        size += (data[XBEE_FRAME_API_LENGTH_MSB] << 8);
        size += data[XBEE_FRAME_API_LENGTH_LSB];
    }
    else if (mode == ApiMode::TRANSPARENT)
    {
        size = data.size();
    }
    else
    {
        // nothing
    }
    
    return size;
}

void Frame::Clear()
{
    data.clear();
}

ApiID Frame::GetApiID() const
{
    ApiID id = ApiID::UNKOWN;
    
    if ((data.size() > XBEE_FRAME_API_ID_INDEX) &&
        (mode != ApiMode::TRANSPARENT))
    {
        uint8_t n = data[XBEE_FRAME_API_ID_INDEX];
        
        if ((n == static_cast<uint8_t>(ApiID::AT_COMMAND)) ||
            (n == static_cast<uint8_t>(ApiID::AT_QUEUE_COMMAND)) || 
            (n == static_cast<uint8_t>(ApiID::TRANSMIT_REQUEST)) || 
            (n == static_cast<uint8_t>(ApiID::EXPLICIT_ADDRESSING_COMMAND)) || 
            (n == static_cast<uint8_t>(ApiID::REMOTE_AT_COMMAND)) || 
            (n == static_cast<uint8_t>(ApiID::AT_COMMAND_RESPONSE)) || 
            (n == static_cast<uint8_t>(ApiID::MODEM_STATUS)) || 
            (n == static_cast<uint8_t>(ApiID::TRANSMIT_STATUS)) || 
            (n == static_cast<uint8_t>(ApiID::ROUTE_INFO_PACKET)) || 
            (n == static_cast<uint8_t>(ApiID::AGGREGATE_ADDRESSING_UPDATE)) || 
            (n == static_cast<uint8_t>(ApiID::RECEIVE_PACKET)) || 
            (n == static_cast<uint8_t>(ApiID::EXPLICIT_RX_INDICATOR)) || 
            (n == static_cast<uint8_t>(ApiID::IO_DATA_SAMPLE_RX_INDICATOR)) || 
            (n == static_cast<uint8_t>(ApiID::NODE_ID_INDICATOR)) || 
            (n == static_cast<uint8_t>(ApiID::REMOTE_AT_COMMAND_RESPONSE)))
        {
                id = static_cast<ApiID>(n);
        }
        else
        {
                id = ApiID::INVALID;
        }
    }
    
    return id;
}

uint16_t Frame::GetID() const
{
    uint16_t id = 0;
    if (data.size() > XBEE_FRAME_API_FRAME_ID_INDEX)
    {
        id = data[XBEE_FRAME_API_FRAME_ID_INDEX];
    }
    return id;
}

void Frame::SetID(uint16_t id)
{
    if (data.size() > XBEE_FRAME_API_FRAME_ID_INDEX)
    {
        data[XBEE_FRAME_API_FRAME_ID_INDEX] = id;
    }
}

uint8_t Frame::Checksum()
{
	uint16_t total = 0;
    uint8_t checksum = XBEE_VALID_CHECKSUM;
    
    // Only calculate checksum if the there is enough data
    if (data.size() >= XBEE_FRAME_API_ID_INDEX + 1)
    {
        
        // Checksum is the total of all bytes starting with the frame ID.
        for (uint16_t i = XBEE_FRAME_API_ID_INDEX; i < data.size(); ++i)
        {
            total += data[i];
        }
    
        // Checksum is 0xFF minus the LSB of the total
        checksum -= (total & 0xFF);
        
        if (mode != ApiMode::TRANSPARENT)
        {
            data.push_back(checksum);
        }
    }
    else
    {
        checksum = 0;
    }

	return checksum;
}

bool Frame::Validate() const
{
    bool valid = false;
    
    if (mode != ApiMode::TRANSPARENT)
    {
        uint16_t total = 0;

        // Only calculate checksum if the there is enough data
        if (data.size() >= XBEE_FRAME_API_ID_INDEX + 1)
        {
            // Checksum is the total of all bytes starting with the frame ID.
            for (uint16_t i = XBEE_FRAME_API_ID_INDEX; i < data.size(); ++i)
            {
                total += data[i];
            }

            // Checksum is valid if the LSB of the total equals 0xFF
            valid = ((total & 0xFF) == XBEE_VALID_CHECKSUM);        
        }
    }
    else
    {
        valid = true;
    }
    
    return valid;
}


std::vector<uint8_t> Frame::Serialize() const
{
    std::vector<uint8_t> serial_data;
    
    for(uint16_t i = 0; i < data.size(); ++i)
    {
        if ((mode == ApiMode::ESCAPED) &&
            (i > 0) &&
            ((data[i] == XBEE_PACKET_START) ||
             (data[i] == XBEE_ESCAPE_BYTE) ||
             (data[i] == XBEE_XON) ||
             (data[i] == XBEE_XOFF)))
        {
            serial_data.push_back(XBEE_ESCAPE_BYTE);
            serial_data.push_back(data[i] ^ XBEE_ESCAPE_MASK);
        }
        else
        {
            serial_data.push_back(data[i]);
        }
    }
    
    return serial_data;
}

bool Frame::Deserialize(const std::vector<uint8_t>& serial_data, uint16_t& index)
{
    bool complete = false;  // Set to true when entire frame is deserialized
    
    Deserialize(&serial_data[0], serial_data.size(), index);
    
    return complete;
}


    
bool Frame::Deserialize(const uint8_t* buff, const uint16_t buff_size, uint16_t& index)
{
    bool complete = false;  // Set to true when entire frame is deserialized
    
    for(; index < buff_size; ++index)
    {
        if ((mode == ApiMode::ESCAPED) &&
            (data.size() > 0) &&
            (buff[index]) == XBEE_ESCAPE_BYTE)
        {
            index++;
            data.push_back(buff[index] ^ XBEE_ESCAPE_MASK);
        }
        else
        {
            data.push_back(buff[index]);
        }
        
        
        if (mode != ApiMode::TRANSPARENT)
        {
            // Entire frame is deserialized when the length of data is
            // the length specified in the packet plus the number
            // of framing bytes (start, length, checksum).
            if ((data.size() > XBEE_FRAME_API_LENGTH_LSB) &&
                (data.size() == (GetSize() + XBEE_FRAMING_SIZE)))
            {
                complete = true;
                break;
            }
        }
    }
    
    if (mode == ApiMode::TRANSPARENT)
    {
        complete = true;
    }
    
    return complete;
}


void Frame::AddSize(uint16_t size)
{
    if (mode != ApiMode::TRANSPARENT)
    {
        // Add size to frame size bytes
        size += (data[XBEE_FRAME_API_LENGTH_MSB] << 8);
        size += data[XBEE_FRAME_API_LENGTH_LSB];

        data[XBEE_FRAME_API_LENGTH_MSB] = ((size >> 8) & 0xFF);
        data[XBEE_FRAME_API_LENGTH_LSB] = (size & 0xFF);
    }
}


} // namespace XBee