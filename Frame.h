#ifndef RXBEE_FRAME_H
#define RXBEE_FRAME_H

#include <stdint.h>
#include <string>
#include <vector>

#include "Types.h"

#define XBEE_PACKET_START   (0x7e)
#define XBEE_FRAME_MAXSIZE  (0x1ff)
#define XBEE_FRAMING_SIZE   (4)
#define XBEE_VALID_CHECKSUM (0xff)
#define XBEE_ESCAPE_BYTE    (0x7d)
#define XBEE_ESCAPE_MASK    (0x20)

#define XBEE_XON            (0x11)
#define XBEE_XOFF           (0x13)


#define XBEE_FRAME_API_LENGTH_MSB   (1)
#define XBEE_FRAME_API_LENGTH_LSB   (XBEE_FRAME_API_LENGTH_MSB + 1)
#define XBEE_FRAME_API_ID_INDEX     (XBEE_FRAME_API_LENGTH_LSB + 1)
#define XBEE_FRAME_API_FRAME_ID_INDEX (XBEE_FRAME_API_ID_INDEX + 1)
#define XBEE_FRAME_API_CONTENT_INDEX (XBEE_FRAME_API_FRAME_ID_INDEX + 1)

namespace RXBee
{
    
class Frame
{
public:
    
    Frame();
    virtual ~Frame();

    Frame(const Frame& other);

    Frame& operator=(const Frame& l);

    void Initialize(const ApiID id, const ApiMode api_mode);
    
    template<typename T, typename ...Ts> 
    void AddFields(T field, Ts... fields)
    {
        AddField(field);
        AddFields(fields...);
    }
    void AddFields() { }

    template<typename T>
    void AddField(const T field)
    {
        for (uint16_t i = 0; i < sizeof(T); ++i)
        {
            data.push_back((field >> (8 * i)) & 0xFF);
        }

        AddSize(sizeof(T));
    }
    
    void AddField(const char* const field);

    void AddField(const std::string& field);

    void AddData(const uint8_t* bytes, const uint16_t byte_cnt);

    void AddData(const std::vector<uint8_t>& bytes);
    
    template<typename T, typename ...Ts>
    bool GetFields(uint16_t index, T& field, Ts&... fields) const
    {
        bool success = GetField(index, field);
        if (success)
        {
            index += sizeof(T);
            success = GetFields(index, fields...);
        }
        return success;
    }
    
    bool GetFields() const;    
    
    template<typename T>
    bool GetField(const uint16_t index, T& field) const
    {
        bool success = false;
        if (data.size() > XBEE_FRAME_API_CONTENT_INDEX + index + sizeof(T))
        {
            for (uint16_t i = 0; i < sizeof(T); ++i)
            {
                field = (data[XBEE_FRAME_API_CONTENT_INDEX + index + i] << (8 * i));
            }
            success = true;
        }
        return success;
    }
    
    bool GetField(const uint16_t index, char* field, uint16_t& length,
                  const  uint16_t max_length) const;
    bool GetField(const uint16_t index, std::string& field, uint16_t& length) const;
    
    bool GetData(const uint16_t index, uint8_t* bytes, uint16_t& length,
                 const uint16_t max_length) const;
    
    bool GetData(const uint16_t index, std::vector<uint8_t>& bytes) const;
    
    uint16_t GetSize() const;
    
    void Clear();

    ApiID GetApiID() const;
    
    uint16_t GetID() const;
    
    void SetID(uint16_t id);
    
    uint8_t Checksum();
    
    bool Validate() const;
    
    std::vector<uint8_t> Serialize() const;
    
    bool Deserialize(const std::vector<uint8_t>& serial_data, uint16_t& index);
    
    bool Deserialize(const uint8_t* buff, const uint16_t buff_size, uint16_t& index);
    
private:
    void AddSize(uint16_t size);
    std::vector<uint8_t> data;
    ApiMode mode;
};

} // namespace XBee

#endif // RXBEE_FRAME_H
