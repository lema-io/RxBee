
#ifndef RXBEE_SPECIFIC_RESPONSES_H
#define RXBEE_SPECIFIC_RESPONSES_H

#include "Frame.h"
#include "Command.h"

#ifndef XBEE_RESP_MAX_PACKET_SIZE
#define XBEE_RESP_MAX_PACKET_SIZE       (0x3D)
#endif

#define XBEE_RESP_AT_CMD_INDEX          (5)
#define XBEE_RESP_RMTAT_CMD_INDEX       (15)
#define XBEE_RESP_AT_STATUS_OFFSET      (2)
#define XBEE_RESP_AT_DATA_OFFSET        (3)

#define XBEE_RESP_TRNSMIT_STATUS_INDEX  (5)
#define XBEE_RESP_MODEM_STATUS_INDEX    (4)
#define XBEE_RESP_RTE_INFO_INDEX        (4)
#define XBEE_RESP_AGG_ADD_INDEX         (5)
#define XBEE_RESP_RX_INDEX              (4)
#define XBEE_RESP_RX_DATA_INDEX         (15)
#define XBEE_RESP_EXP_RX_INDEX          (4)
#define XBEE_RESP_EXP_RX_DATA_INDEX     (21)
#define XBEE_RESP_NODE_ID_INDEX         (4)
#define XBEE_RESP_NODE_ID_STR_INDEX     (25)

#define XBEE_AT_AF_BITFIELD_LEN (9)
#define XBEE_AT_ED_CHANNEL_CNT  (72)
#define XBEE_AT_NI_IDENT_LEN    (20)
#define XBEE_AT_VL_VER_LEN      (64)

namespace RXBee {
namespace Response {
    
struct ApiFrame
{
    ApiFrame(Frame* f) : frame(f), api_id(ApiID::UNKOWN), extracted(false)
    {
        if (f != NULL)
        {
            api_id = frame->GetApiID();
            extracted = true;
        }
    }
    
    Frame* frame;
    ApiID api_id;
    bool extracted;
};



namespace ATCommand
{

enum class Status
{
    OK = 0,
    ERROR = 1,
    INVALID_COMMAND = 2,
    INVALID_PARAMETER = 3,
    TX_FAILURE = 4,
    
    INVALID_STATUS = 0xFF
};


struct AF_Rsp
{ uint8_t bitfield[XBEE_AT_AF_BITFIELD_LEN]; };


struct AO_Rsp
{ uint16_t api_options; };


struct AP_Rsp
{ uint8_t api_mode; };


struct AV_Rsp
{ uint8_t ana_v_ref; };


struct BC_Rsp
{ uint16_t bytes_transmitted; };


struct BD_Rsp
{ uint32_t baud_rate; };


struct BH_Rsp
{ uint8_t maximum_broadcast_hops; };


struct CC_Rsp
{ uint16_t command_character; };


struct CE_Rsp
{ uint8_t node_messaging_options; };


struct CI_Rsp
{ uint16_t app_cluster_id; };


struct CK_Rsp
{ uint16_t config_crc; };


struct CM_Rsp
{ uint8_t mask[XBEE_AT_AF_BITFIELD_LEN]; };


struct CT_Rsp
{ uint16_t cmd_mode_timeout; };


struct D0_Rsp
{ uint8_t config; };


struct D1_Rsp
{ uint8_t config; };


struct D2_Rsp
{ uint8_t config; };


struct D3_Rsp
{ uint8_t config; };


struct D4_Rsp
{ uint8_t config; };


struct D5_Rsp
{ uint8_t config; };


struct D6_Rsp
{ uint8_t config; };


struct D7_Rsp
{ uint8_t config; };


struct D8_Rsp
{ uint8_t config; };


struct D9_Rsp
{ uint8_t config; };


struct DB_Rsp
{ uint8_t last_packet_rssi; };


struct DD_Rsp
{ uint32_t digi_device_type; };


struct DE_Rsp
{ uint8_t app_dest_id; };


struct DH_Rsp
{ uint64_t address; };


struct DL_Rsp
{ uint64_t address; };


struct DN_Rsp
{ uint64_t node_address; };


struct EA_Rsp
{ uint16_t mac_ack_failure_count; };


struct ED_Rsp
{ uint8_t channel_energy[XBEE_AT_ED_CHANNEL_CNT]; };


struct EE_Rsp
{ uint8_t security_enable; };


struct ER_Rsp
{ uint16_t rx_error_count; };


struct FN_Rsp
{
    uint64_t address;
    char node_identifier[XBEE_AT_NI_IDENT_LEN];
    uint8_t device_type;
    uint8_t status;
    uint16_t profile_id;
    uint16_t mfr_id;
    uint32_t digi_device_type;
    uint8_t last_hop_rssi;    
};


struct FT_Rsp
{ uint8_t flow_control_threshold; };


struct GD_Rsp
{ uint16_t good_packet_bytes; };


struct GT_Rsp
{ uint16_t cmd_mode_silence; };


struct HP_Rsp
{ uint8_t preamble_id; };


struct HS_Rsp
{ uint16_t hardware_series_number; };


struct HV_Rsp
{ uint16_t hardware_version; };


struct IC_Rsp
{ uint16_t dio_change_detect_bits; };


struct ID_Rsp
{ uint16_t network_id; };


struct IF_Rsp
{ uint16_t sleep_sample_rate; };


struct IR_Rsp
{ uint16_t sample_rate; };


struct LT_Rsp
{ uint8_t associate_led_blink_time; };


struct M0_Rsp
{ uint16_t pwm0_duty_cycle; };


struct M1_Rsp
{ uint16_t pwm1_duty_cycle; };


struct MF_Rsp
{ uint8_t minimum_region_channels; };


struct MR_Rsp
{ uint8_t mesh_unicast_retries; };


struct MS_Rsp
{ uint16_t missed_sync_messages; };


struct MT_Rsp
{ uint8_t additional_broadcast_transmissions; };


struct NB_Rsp
{ uint8_t parity; };


struct ND_Rsp
{ 
    uint64_t address;
    char node_identifier[XBEE_AT_NI_IDENT_LEN];
    uint8_t device_type;
    uint8_t status;
    uint16_t profile_id;
    uint16_t mfr_id;
    uint32_t digi_device_type;
    uint8_t last_hop_rssi;    
};


struct NH_Rsp
{ uint16_t maximum_network_hops; };


struct NI_Rsp
{ char node_identifier[XBEE_AT_NI_IDENT_LEN]; };


struct NN_Rsp
{ uint8_t rebroadcast_delay_slots; };


struct NO_Rsp
{ uint8_t node_discovery_options; };


struct NP_Rsp
{ uint16_t max_rf_payload_bytes; };


struct NT_Rsp
{ uint16_t node_discover_time; };


struct OS_Rsp
{ uint16_t operating_sleep_time; };


struct OW_Rsp
{ uint32_t operating_wake_time; };


struct P0_Rsp
{ uint8_t config; };


struct P1_Rsp
{ uint8_t config; };


struct P2_Rsp
{ uint8_t config; };


struct P3_Rsp
{ uint8_t config; };


struct P4_Rsp
{ uint8_t config; };


struct PD_Rsp
{ uint16_t pull_up_down_dir_bits; };


struct PL_Rsp
{ TxPowerLevel tx_power_level; };


struct PR_Rsp
{ uint16_t pull_up_down_enable_bits; };


struct RO_Rsp
{ uint8_t transparent_inter_char_silence; };


struct RP_Rsp
{ uint16_t pwm_expiration; };


struct RR_Rsp
{ uint8_t max_unicast_delivery_attempts; };


struct SB_Rsp
{ uint8_t stop_bits; };


struct SE_Rsp
{ uint16_t app_source_ep; };


struct SH_Rsp
{ uint64_t address; };


struct SL_Rsp
{ uint64_t address; };


struct SM_Rsp
{ uint8_t sleep_mode; };


struct SN_Rsp
{ uint16_t sleep_periods; };


struct SO_Rsp
{ uint8_t sleep_options; };


struct SP_Rsp
{ uint32_t sleep_period; };


struct SQ_Rsp
{ uint16_t missed_sleep_sync_count; };


struct SS_Rsp
{ uint16_t sleep_status; };


struct ST_Rsp
{ uint32_t wake_time; };


struct TO_Rsp
{ uint8_t transmit_options; };


struct TP_Rsp
{ uint16_t temperature; };


struct TR_Rsp
{ uint16_t tx_failure_count; };


struct UA_Rsp
{ uint16_t unicast_tx_count; };


struct VL_Rsp
{ char version_info[XBEE_AT_VL_VER_LEN]; };


struct VR_Rsp
{ uint16_t firmware_version; };


struct WH_Rsp
{ uint16_t wake_host_delay; };


struct Response
{
    Response(ApiFrame& rsp);
    
    Frame* frame;
    XBeeATCommand command;
    Status status;
    bool remote;
    uint64_t source_addr;
    uint16_t data_offset;
    bool extracted;
    
    AF_Rsp AF() const;
    AO_Rsp AO() const;
    AP_Rsp AP() const;
    AV_Rsp AV() const;
    BC_Rsp BC() const;
    BD_Rsp BD() const;
    BH_Rsp BH() const;
    CC_Rsp CC() const;
    CE_Rsp CE() const;
    CI_Rsp CI() const;
    CK_Rsp CK() const;
    CM_Rsp CM() const;
    CT_Rsp CT() const;
    D0_Rsp D0() const;
    D1_Rsp D1() const;
    D2_Rsp D2() const;
    D3_Rsp D3() const;
    D4_Rsp D4() const;
    D5_Rsp D5() const;
    D6_Rsp D6() const;
    D7_Rsp D7() const;
    D8_Rsp D8() const;
    D9_Rsp D9() const;
    DB_Rsp DB() const;
    DD_Rsp DD() const;
    DE_Rsp DE() const;
    DH_Rsp DH() const;
    DL_Rsp DL() const;
    DN_Rsp DN() const;
    EA_Rsp EA() const;
    ED_Rsp ED() const;
    EE_Rsp EE() const;
    ER_Rsp ER() const;
    FN_Rsp FN() const;
    FT_Rsp FT() const;
    GD_Rsp GD() const;
    GT_Rsp GT() const;
    HP_Rsp HP() const;
    HS_Rsp HS() const;
    HV_Rsp HV() const;
    IC_Rsp IC() const;
    ID_Rsp ID() const;
    IF_Rsp IF() const;
    IR_Rsp IR() const;
    LT_Rsp LT() const;
    M0_Rsp M0() const;
    M1_Rsp M1() const;
    MF_Rsp MF() const;
    MR_Rsp MR() const;
    MS_Rsp MS() const;
    MT_Rsp MT() const;
    NB_Rsp NB() const;
    ND_Rsp ND() const;
    NH_Rsp NH() const;
    NI_Rsp NI() const;
    NN_Rsp NN() const;
    NO_Rsp NO() const;
    NP_Rsp NP() const;
    NT_Rsp NT() const;
    OS_Rsp OS() const;
    OW_Rsp OW() const;
    P0_Rsp P0() const;
    P1_Rsp P1() const;
    P2_Rsp P2() const;
    P3_Rsp P3() const;
    P4_Rsp P4() const;
    PD_Rsp PD() const;
    PL_Rsp PL() const;
    PR_Rsp PR() const;
    RO_Rsp RO() const;
    RP_Rsp RP() const;
    RR_Rsp RR() const;
    SB_Rsp SB() const;
    SE_Rsp SE() const;
    SH_Rsp SH() const;
    SL_Rsp SL() const;
    SM_Rsp SM() const;
    SN_Rsp SN() const;
    SO_Rsp SO() const;
    SP_Rsp SP() const;
    SQ_Rsp SQ() const;
    SS_Rsp SS() const;
    ST_Rsp ST() const;
    TO_Rsp TO() const;
    TP_Rsp TP() const;
    TR_Rsp TR() const;
    UA_Rsp UA() const;
    VL_Rsp VL() const;
    VR_Rsp VR() const;
    WH_Rsp WH() const;
};

} // namespace RXBee.Response.ATCommand

enum class TransmitDeliveryStatus
{
    SUCCESS = 0X00,
    MAC_ACK_FAILURE = 0X01,
    COLLISION_AVOIDANCE_FAILURE = 0X02,
    NETWORK_ACK_FAILURE = 0X21,
    ROUTE_NOT_FOUND = 0X25,
    INTERNAL_RESOURCE_ERROR = 0X31,
    INTERNAL_ERROR = 0X32,
    PAYLOAD_TOO_LARGE = 0X74,
    INDIRECT_MESSAGE_REQUESTED = 0X75,
    
    INVALID = 0xFF
};

enum class TransmitDiscoveryStatus
{
    NO_DISCOVERY_OVERHEAD = 0X00,
    ROUTE_DISCOVERY = 0x02,
    
    INVALID = 0xFF
};

struct TransmitStatus
{
    TransmitStatus(ApiFrame& rsp);
    Frame* frame;
    bool extracted;
    uint16_t destination_network_address;
    uint8_t tx_retry_count;
    TransmitDeliveryStatus delivery_status;
    TransmitDiscoveryStatus discovery_status;
};

struct ModemStatusUpdate
{
    ModemStatusUpdate(ApiFrame& rsp);

    Frame* frame;
    bool extracted;
    ModemStatus status;
};

        
struct RouteInformation
{
    RouteInformation(ApiFrame& rsp);
    Frame* frame;
    bool extracted;
    bool trace;
    uint32_t timestamp;
    uint8_t ack_timeout_count;
    uint8_t tx_blocked_count;
    uint64_t destination_addr;
    uint64_t source_addr;
    uint64_t responder_addr;
    uint64_t receiver_addr;
};


struct AggregateAddressingUpdate
{
    AggregateAddressingUpdate(ApiFrame& rsp);
    Frame* frame;
    bool extracted;
    uint64_t new_addr;
    uint64_t old_addr;
};

enum class ReceiveOptions
{   
    PACKET_ACK = 0x01,
    PACKET_BROADCAST = 0x02,
    POINT_TO_MULTIPOINT = 0x40,
    REPEATER_MODE = 0x80,
    DIGI_MESH = 0xC0
};

struct ReceivePacket
{
    ReceivePacket(ApiFrame& rsp, std::vector<uint8_t>& buffer);
    Frame* frame;
    bool extracted;
    uint64_t sender_addr;
    uint8_t options;
};

struct ExplicitReceivePacket
{
    ExplicitReceivePacket(ApiFrame& rsp);
    Frame* frame;
    bool extracted;
    uint64_t sender_addr;
    uint8_t source_endpoint;
    uint8_t destination_endpoint;
    uint16_t cluster_id;
    uint16_t profile_id;
    uint8_t options;
    uint8_t data[XBEE_RESP_MAX_PACKET_SIZE];
    uint16_t data_length;
};

struct ExplicitIOSample
{
    ExplicitIOSample(ApiFrame& rsp);
    Frame* frame;
    bool extracted;
    uint64_t source_addr;
    uint16_t source_nework_addr;
    uint8_t options;
    uint8_t number_of_samples;
    uint16_t digital_channel_mask;
    uint8_t analog_channel_mask;
    uint16_t digital_samples;
    uint16_t analog_samples[4];
};

enum class DeviceType
{
    DEVICE_TYPE_COORDINATOR = 0,
    DEVICE_TYPE_NORMAL = 1,
    DEVICE_TYPE_END = 2,
    
    INVALID = 0xFF
};

struct NodeID
{
    NodeID(ApiFrame& rsp);
    Frame* frame;
    bool extracted;
    uint64_t source_addr;
    uint8_t options;
    uint64_t remote_addr;
    char identifier[XBEE_AT_NI_IDENT_LEN];
    uint16_t identifier_len;
    DeviceType type;
    bool pushbutton;
    uint16_t digi_profile_id;
    uint16_t digi_mfr_id;
    bool digi_dd_value_included;
    uint16_t digi_dd_value;
    bool rssi_included;
    int8_t rssi;
};

} } // namespace RXBee.Response


#endif