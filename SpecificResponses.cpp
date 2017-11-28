
#include "SpecificResponses.h"

namespace RXBee {
namespace Response {
    
namespace ATCommand
{

Response::Response(ApiFrame& rsp) : frame(NULL), command(XBeeATCommand::INVALID), 
        status(Status::INVALID_STATUS), remote(false), source_addr(0),
        data_offset(0), extracted(false)
{
    if (rsp.extracted)
    {
        if ((rsp.api_id == ApiID::AT_COMMAND_RESPONSE) ||
            (rsp.api_id == ApiID::REMOTE_AT_COMMAND_RESPONSE))
        {
            frame = rsp.frame;
            remote = (rsp.api_id == ApiID::REMOTE_AT_COMMAND_RESPONSE);

            char cmd_str[3];
            uint16_t str_len;

            uint16_t offset = XBEE_RESP_AT_CMD_INDEX;

            if (remote)  
            { 
                rsp.frame->GetField(offset, source_addr); 
                offset += 10; // 8 byte address and 2 byte reserved
            }

            extracted = rsp.frame->GetField(offset, cmd_str, str_len, 2);
            cmd_str[2] = '\0';
            offset += 2;

            if (extracted)
            {
                command = ToXbeeATCmd(cmd_str);
                uint8_t status_i;
                extracted = rsp.frame->GetField(offset, status_i);

                if (extracted)
                {
                    offset += 1;

                    if ((status_i == static_cast<uint8_t>(Status::OK)) ||
                        (status_i == static_cast<uint8_t>(Status::ERROR)) || 
                        (status_i == static_cast<uint8_t>(Status::INVALID_COMMAND)) || 
                        (status_i == static_cast<uint8_t>(Status::INVALID_PARAMETER)) || 
                        (status_i == static_cast<uint8_t>(Status::TX_FAILURE)))
                    {
                        status = static_cast<Status>(status_i);
                    }
                    else
                    {
                        status = Status::INVALID_STATUS;
                    }
                }

                data_offset = offset;
            }
        }
    }
}


AF_Rsp Response::AF() const
{
    AF_Rsp rsp;
    if (extracted)
    {
        uint16_t len;
        frame->GetData(data_offset, rsp.bitfield, len, XBEE_AT_AF_BITFIELD_LEN);
    }
    return rsp;
}

AO_Rsp Response::AO() const
{
    AO_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.api_options); }
    else { rsp.api_options = 0; }
    return rsp;
}



AP_Rsp Response::AP() const
{
    AP_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.api_mode); }
    else { rsp.api_mode = 0; }
    return rsp;
}



AV_Rsp Response::AV() const
{
    AV_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.ana_v_ref); }
    else { rsp.ana_v_ref = 0; }
    return rsp;
}



BC_Rsp Response::BC() const
{
    BC_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.bytes_transmitted); }
    else { rsp.bytes_transmitted = 0; }
    return rsp;
}



BD_Rsp Response::BD() const
{
    BD_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.baud_rate); }
    else { rsp.baud_rate = 0; }
    return rsp;
}



BH_Rsp Response::BH() const
{
    BH_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.maximum_broadcast_hops); }
    else { rsp.maximum_broadcast_hops = 0; }
    return rsp;
}

CC_Rsp Response::CC() const
{
    CC_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.command_character); }
    else { rsp.command_character = 0; }
    return rsp;
}



CE_Rsp Response::CE() const
{
    CE_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.node_messaging_options); }
    else { rsp.node_messaging_options = 0; }
    return rsp;
}



CI_Rsp Response::CI() const
{
    CI_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.app_cluster_id); }
    else { rsp.app_cluster_id = 0; }
    return rsp;
}



CK_Rsp Response::CK() const
{
    CK_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config_crc); }
    else { rsp.config_crc = 0; }
    return rsp;
}



CM_Rsp Response::CM() const
{
    CM_Rsp rsp;
    if (extracted)
    {
        uint16_t len;
        frame->GetData(data_offset, rsp.mask, len, XBEE_AT_AF_BITFIELD_LEN);
    }
    return rsp;
}


CT_Rsp Response::CT() const
{
    CT_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.cmd_mode_timeout); }
    else { rsp.cmd_mode_timeout = 0; }
    return rsp;
}



D0_Rsp Response::D0() const
{
    D0_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



D1_Rsp Response::D1() const
{
    D1_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



D2_Rsp Response::D2() const
{
    D2_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



D3_Rsp Response::D3() const
{
    D3_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



D4_Rsp Response::D4() const
{
    D4_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



D5_Rsp Response::D5() const
{
    D5_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



D6_Rsp Response::D6() const
{
    D6_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



D7_Rsp Response::D7() const
{
    D7_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



D8_Rsp Response::D8() const
{
    D8_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



D9_Rsp Response::D9() const
{
    D9_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



DB_Rsp Response::DB() const
{
    DB_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.last_packet_rssi); }
    else { rsp.last_packet_rssi = 0; }
    return rsp;
}



DD_Rsp Response::DD() const
{
    DD_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.digi_device_type); }
    else { rsp.digi_device_type = 0; }
    return rsp;
}



DE_Rsp Response::DE() const
{
    DE_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.app_dest_id); }
    else { rsp.app_dest_id = 0; }
    return rsp;
}



DH_Rsp Response::DH() const
{
    DH_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.address); }
    else { rsp.address = 0; }
    return rsp;
}



DL_Rsp Response::DL() const
{
    DL_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.address); }
    else { rsp.address = 0; }
    return rsp;
}



DN_Rsp Response::DN() const
{
    DN_Rsp rsp;
    if(extracted) { frame->GetField(data_offset + 2, rsp.node_address); }
    else { rsp.node_address = 0; }
    return rsp;
}



EA_Rsp Response::EA() const
{
    EA_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.mac_ack_failure_count); }
    else { rsp.mac_ack_failure_count = 0; }
    return rsp;
}



ED_Rsp Response::ED() const
{
    ED_Rsp rsp;
    for (uint16_t i = 0; i < XBEE_AT_ED_CHANNEL_CNT; ++i) { rsp.channel_energy[i] = 0; }
    if(extracted)
    {    
        uint16_t len;
        frame->GetData(data_offset, rsp.channel_energy, len, XBEE_AT_ED_CHANNEL_CNT);
    }
    return rsp;
}



EE_Rsp Response::EE() const
{
    EE_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.security_enable); }
    else { rsp.security_enable = 0; }
    return rsp;
}



ER_Rsp Response::ER() const
{
    ER_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.rx_error_count); }
    else { rsp.rx_error_count = 0; }
    return rsp;
}



FN_Rsp Response::FN() const
{
    FN_Rsp rsp;
    for (uint16_t i = 0; i < XBEE_AT_NI_IDENT_LEN; ++i) { rsp.node_identifier[i] = '\0'; }
    if(extracted)
    { 
        uint16_t offset = data_offset + 2;
        frame->GetField(offset, rsp.address);
        offset += 8;
        uint16_t len;
        frame->GetField(offset, rsp.node_identifier, len, XBEE_AT_NI_IDENT_LEN); 
        offset += len + 2;
        frame->GetFields(offset, rsp.device_type, rsp.status, rsp.profile_id, rsp.mfr_id);
        offset += 6;
        if (frame->GetField(offset, rsp.digi_device_type))
        { 
            offset += 4; 
        }
        frame->GetField(offset, rsp.last_hop_rssi);
    }
    else 
    {
        rsp.address = 0;
        rsp.device_type = 0;
        rsp.digi_device_type = 0;
        rsp.last_hop_rssi = 0;
        rsp.mfr_id = 0;
        rsp.profile_id = 0;
        rsp.status = 0;
    }
    return rsp;
}


FT_Rsp Response::FT() const
{
    FT_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.flow_control_threshold); }
    else { rsp.flow_control_threshold = 0; }
    return rsp;
}



GD_Rsp Response::GD() const
{
    GD_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.good_packet_bytes); }
    else { rsp.good_packet_bytes = 0; }
    return rsp;
}



GT_Rsp Response::GT() const
{
    GT_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.cmd_mode_silence); }
    else { rsp.cmd_mode_silence = 0; }
    return rsp;
}



HP_Rsp Response::HP() const
{
    HP_Rsp rsp;
    if (extracted) { frame->GetField(data_offset, rsp.preamble_id); }
    return rsp;
}



HS_Rsp Response::HS() const
{
    HS_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.hardware_series_number); }
    else { rsp.hardware_series_number = 0; }
    return rsp;
}



HV_Rsp Response::HV() const
{
    HV_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.hardware_version); }
    else { rsp.hardware_version = 0; }
    return rsp;
}



IC_Rsp Response::IC() const
{
    IC_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.dio_change_detect_bits); }
    else { rsp.dio_change_detect_bits = 0; }
    return rsp;
}



ID_Rsp Response::ID() const
{
    ID_Rsp rsp;
    if (extracted) { frame->GetField(data_offset, rsp.network_id); }
    return rsp;
}



IF_Rsp Response::IF() const
{
    IF_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.sleep_sample_rate); }
    else { rsp.sleep_sample_rate = 0; }
    return rsp;
}



IR_Rsp Response::IR() const
{
    IR_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.sample_rate); }
    else { rsp.sample_rate = 0; }
    return rsp;
}


LT_Rsp Response::LT() const
{
    LT_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.associate_led_blink_time); }
    else { rsp.associate_led_blink_time = 0; }
    return rsp;
}



M0_Rsp Response::M0() const
{
    M0_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.pwm0_duty_cycle); }
    else { rsp.pwm0_duty_cycle = 0; }
    return rsp;
}



M1_Rsp Response::M1() const
{
    M1_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.pwm1_duty_cycle); }
    else { rsp.pwm1_duty_cycle = 0; }
    return rsp;
}



MF_Rsp Response::MF() const
{
    MF_Rsp rsp;
    if (extracted) { frame->GetField(data_offset, rsp.minimum_region_channels); }
    else { rsp.minimum_region_channels = 0; }
    return rsp;
}



MR_Rsp Response::MR() const
{
    MR_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.mesh_unicast_retries); }
    else { rsp.mesh_unicast_retries = 0; }
    return rsp;
}



MS_Rsp Response::MS() const
{
    MS_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.missed_sync_messages); }
    else { rsp.missed_sync_messages = 0; }
    return rsp;
}



MT_Rsp Response::MT() const
{
    MT_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.additional_broadcast_transmissions); }
    else { rsp.additional_broadcast_transmissions = 0; }
    return rsp;
}



NB_Rsp Response::NB() const
{
    NB_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.parity); }
    else { rsp.parity = 0; }
    return rsp;
}



ND_Rsp Response::ND() const
{
    ND_Rsp rsp;
    for (uint16_t i = 0; i < XBEE_AT_NI_IDENT_LEN; ++i) { rsp.node_identifier[i] = '\0'; }
    if(extracted)
    { 
        uint16_t offset = data_offset + 2;
        frame->GetField(offset, rsp.address);
        offset += 8;
        uint16_t len;
        frame->GetField(offset, rsp.node_identifier, len, XBEE_AT_NI_IDENT_LEN); 
        offset += len + 2;
        frame->GetFields(offset, rsp.device_type, rsp.status, rsp.profile_id, rsp.mfr_id);
        offset += 6;
        if (frame->GetField(offset, rsp.digi_device_type))
        { 
            offset += 4; 
        }
        frame->GetField(offset, rsp.last_hop_rssi);
    }
    else 
    {
        rsp.address = 0;
        rsp.device_type = 0;
        rsp.digi_device_type = 0;
        rsp.last_hop_rssi = 0;
        rsp.mfr_id = 0;
        rsp.profile_id = 0;
        rsp.status = 0;
    }
    return rsp;
}



NH_Rsp Response::NH() const
{
    NH_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.maximum_network_hops); }
    else { rsp.maximum_network_hops = 0; }
    return rsp;
}



NI_Rsp Response::NI() const
{
    NI_Rsp rsp;
    for (uint16_t i = 0; i < XBEE_AT_NI_IDENT_LEN; ++i) { rsp.node_identifier[i] = '\0'; }
    if(extracted) 
    { 
        uint16_t len;
        frame->GetField(data_offset, rsp.node_identifier, len, XBEE_AT_NI_IDENT_LEN); 
    }
    return rsp;
}



NN_Rsp Response::NN() const
{
    NN_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.rebroadcast_delay_slots); }
    else { rsp.rebroadcast_delay_slots = 0; }
    return rsp;
}



NO_Rsp Response::NO() const
{
    NO_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.node_discovery_options); }
    else { rsp.node_discovery_options = 0; }
    return rsp;
}



NP_Rsp Response::NP() const
{
    NP_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.max_rf_payload_bytes); }
    else { rsp.max_rf_payload_bytes = 0; }
    return rsp;
}



NT_Rsp Response::NT() const
{
    NT_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.node_discover_time); }
    else { rsp.node_discover_time = 0; }
    return rsp;
}



OS_Rsp Response::OS() const
{
    OS_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.operating_sleep_time); }
    else { rsp.operating_sleep_time = 0; }
    return rsp;
}



OW_Rsp Response::OW() const
{
    OW_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.operating_wake_time); }
    else { rsp.operating_wake_time = 0; }
    return rsp;
}



P0_Rsp Response::P0() const
{
    P0_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



P1_Rsp Response::P1() const
{
    P1_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



P2_Rsp Response::P2() const
{
    P2_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



P3_Rsp Response::P3() const
{
    P3_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



P4_Rsp Response::P4() const
{
    P4_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.config); }
    else { rsp.config = 0; }
    return rsp;
}



PD_Rsp Response::PD() const
{
    PD_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.pull_up_down_dir_bits); }
    else { rsp.pull_up_down_dir_bits = 0; }
    return rsp;
}



PL_Rsp Response::PL() const
{
    PL_Rsp rsp;
    if(extracted) 
    {
        uint8_t pl_i = 0;
        frame->GetField(data_offset, pl_i);

        if ((pl_i == static_cast<uint8_t>(TxPowerLevel::SEVEN_DBM)) ||
            (pl_i == static_cast<uint8_t>(TxPowerLevel::EIGHTEEN_DBM)) ||
            (pl_i == static_cast<uint8_t>(TxPowerLevel::TWENTY_ONE_DBM)) ||
            (pl_i == static_cast<uint8_t>(TxPowerLevel::TWENTY_FOUR_DBM)))
        {
            rsp.tx_power_level = static_cast<TxPowerLevel>(pl_i);
        }
    }
    else
    { rsp.tx_power_level = TxPowerLevel::INVALID; }
    return rsp;
}



PR_Rsp Response::PR() const
{
    PR_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.pull_up_down_enable_bits); }
    else { rsp.pull_up_down_enable_bits = 0; }
    return rsp;
}


RO_Rsp Response::RO() const
{
    RO_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.transparent_inter_char_silence); }
    else { rsp.transparent_inter_char_silence = 0; }
    return rsp;
}



RP_Rsp Response::RP() const
{
    RP_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.pwm_expiration); }
    else { rsp.pwm_expiration = 0; }
    return rsp;
}



RR_Rsp Response::RR() const
{
    RR_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.max_unicast_delivery_attempts); }
    else { rsp.max_unicast_delivery_attempts = 0; }
    return rsp;
}



SB_Rsp Response::SB() const
{
    SB_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.stop_bits); }
    else { rsp.stop_bits = 0; }
    return rsp;
}



SE_Rsp Response::SE() const
{
    SE_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.app_source_ep); }
    else { rsp.app_source_ep = 0; }
    return rsp;
}



SH_Rsp Response::SH() const
{
    SH_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.address); }
    else { rsp.address = 0; }
    return rsp;
}



SL_Rsp Response::SL() const
{
    SL_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.address); }
    else { rsp.address = 0; }
    return rsp;
}



SM_Rsp Response::SM() const
{
    SM_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.sleep_mode); }
    else { rsp.sleep_mode = 0; }
    return rsp;
}



SN_Rsp Response::SN() const
{
    SN_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.sleep_periods); }
    else { rsp.sleep_periods = 0; }
    return rsp;
}



SO_Rsp Response::SO() const
{
    SO_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.sleep_options); }
    else { rsp.sleep_options = 0; }
    return rsp;
}



SP_Rsp Response::SP() const
{
    SP_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.sleep_period); }
    else { rsp.sleep_period = 0; }
    return rsp;
}



SQ_Rsp Response::SQ() const
{
    SQ_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.missed_sleep_sync_count); }
    else { rsp.missed_sleep_sync_count = 0; }
    return rsp;
}



SS_Rsp Response::SS() const
{
    SS_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.sleep_status); }
    else { rsp.sleep_status = 0; }
    return rsp;
}



ST_Rsp Response::ST() const
{
    ST_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.wake_time); }
    else { rsp.wake_time = 0; }
    return rsp;
}



TO_Rsp Response::TO() const
{
    TO_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.transmit_options); }
    else { rsp.transmit_options = 0; }
    return rsp;
}



TP_Rsp Response::TP() const
{
    TP_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.temperature); }
    else { rsp.temperature = 0; }
    return rsp;
}



TR_Rsp Response::TR() const
{
    TR_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.tx_failure_count); }
    else { rsp.tx_failure_count = 0; }
    return rsp;
}



UA_Rsp Response::UA() const
{
    UA_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.unicast_tx_count); }
    else { rsp.unicast_tx_count = 0; }
    return rsp;
}



VL_Rsp Response::VL() const
{
    VL_Rsp rsp;
    for (uint16_t i = 0; i < XBEE_AT_VL_VER_LEN; ++i) { rsp.version_info[i] = '\0'; }
    if(extracted) 
    { 
        uint16_t len;
        frame->GetField(data_offset, rsp.version_info, len, XBEE_AT_VL_VER_LEN); 
    }
    return rsp;
}



VR_Rsp Response::VR() const
{
    VR_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.firmware_version); }
    else { rsp.firmware_version = 0; }
    return rsp;
}



WH_Rsp Response::WH() const
{
    WH_Rsp rsp;
    if(extracted) { frame->GetField(data_offset, rsp.wake_host_delay); }
    else { rsp.wake_host_delay = 0; }
    return rsp;
}

} // namespace RXBee::Response::ATCommand 

TransmitStatus::TransmitStatus(ApiFrame& rsp)
{
    if (rsp.extracted && (rsp.api_id == ApiID::TRANSMIT_STATUS))
    {
        frame = rsp.frame;
        delivery_status = TransmitDeliveryStatus::INVALID;
        discovery_status = TransmitDiscoveryStatus::INVALID;
        uint8_t del_status_i;
        uint8_t dis_status_i;
        extracted = frame->GetFields(XBEE_RESP_TRNSMIT_STATUS_INDEX,
                destination_network_address, tx_retry_count,
                del_status_i, dis_status_i);
        
        if (extracted)
        {
            if ((del_status_i == static_cast<uint8_t>(TransmitDeliveryStatus::SUCCESS)) ||
                (del_status_i == static_cast<uint8_t>(TransmitDeliveryStatus::MAC_ACK_FAILURE)) ||
                (del_status_i == static_cast<uint8_t>(TransmitDeliveryStatus::COLLISION_AVOIDANCE_FAILURE)) ||
                (del_status_i == static_cast<uint8_t>(TransmitDeliveryStatus::NETWORK_ACK_FAILURE)) ||
                (del_status_i == static_cast<uint8_t>(TransmitDeliveryStatus::ROUTE_NOT_FOUND)) ||
                (del_status_i == static_cast<uint8_t>(TransmitDeliveryStatus::INTERNAL_RESOURCE_ERROR)) ||
                (del_status_i == static_cast<uint8_t>(TransmitDeliveryStatus::INTERNAL_ERROR)) ||
                (del_status_i == static_cast<uint8_t>(TransmitDeliveryStatus::PAYLOAD_TOO_LARGE)) ||
                (del_status_i == static_cast<uint8_t>(TransmitDeliveryStatus::INDIRECT_MESSAGE_REQUESTED)))
            {
                delivery_status = static_cast<TransmitDeliveryStatus>(del_status_i);
            }

            if ((dis_status_i == static_cast<uint8_t>(TransmitDiscoveryStatus::NO_DISCOVERY_OVERHEAD)) ||
                (dis_status_i == static_cast<uint8_t>(TransmitDiscoveryStatus::ROUTE_DISCOVERY)))
            {
                discovery_status = static_cast<TransmitDiscoveryStatus>(dis_status_i);
            }
        }
    }        
}


ModemStatusUpdate::ModemStatusUpdate(ApiFrame& rsp)
{
    if (rsp.extracted && (rsp.api_id == ApiID::MODEM_STATUS))
    {
        frame = rsp.frame;
        status = ModemStatus::UNKNOWN;
        uint8_t status_i;
        extracted = frame->GetFields(XBEE_RESP_MODEM_STATUS_INDEX, status_i);
        
        if (extracted)
        {
            if ((status_i == static_cast<uint8_t>(ModemStatus::HW_RESET)) ||
                (status_i == static_cast<uint8_t>(ModemStatus::WATCHDOG_TIMER_RESET)) ||
                (status_i == static_cast<uint8_t>(ModemStatus::NETWORK_WOKE_UP)) ||
                (status_i == static_cast<uint8_t>(ModemStatus::NETWORK_WENT_TO_SLEEP)))
            {
                status = static_cast<ModemStatus>(status_i);
            }
        }
    }        
}


RouteInformation::RouteInformation(ApiFrame& rsp)
{
    if (rsp.extracted && (rsp.api_id == ApiID::MODEM_STATUS))
    {
        frame = rsp.frame;
        uint8_t source;
        uint8_t len;
        extracted = frame->GetFields(XBEE_RESP_RTE_INFO_INDEX, source, len,
                timestamp, ack_timeout_count, tx_blocked_count, 
                destination_addr, source_addr, responder_addr,
                receiver_addr);
        
        if (extracted)
        {
            trace = (source == 0x12);
        }
    }        
}


AggregateAddressingUpdate::AggregateAddressingUpdate(ApiFrame& rsp)
{
    if (rsp.extracted && (rsp.api_id == ApiID::AGGREGATE_ADDRESSING_UPDATE))
    {
        frame = rsp.frame;
        extracted = frame->GetFields(XBEE_RESP_AGG_ADD_INDEX, new_addr, old_addr);
    }        
}

ReceivePacket::ReceivePacket(ApiFrame& rsp, std::vector<uint8_t>& buffer)
{
    if (rsp.extracted && (rsp.api_id == ApiID::RECEIVE_PACKET))
    {
        frame = rsp.frame;
        uint16_t reserved;
        extracted = frame->GetFields(XBEE_RESP_RX_INDEX, sender_addr,
                reserved, options);
        if (extracted)
        {
            frame->GetData(XBEE_RESP_RX_DATA_INDEX, buffer);
        }  
    }        
}

ExplicitReceivePacket::ExplicitReceivePacket(ApiFrame& rsp) : data_length(0)
{
    if (rsp.extracted && (rsp.api_id == ApiID::RECEIVE_PACKET))
    {
        frame = rsp.frame;
        uint16_t reserved;
        extracted = frame->GetFields(XBEE_RESP_EXP_RX_INDEX, sender_addr,
                reserved, source_endpoint, destination_endpoint, cluster_id,
                profile_id, options);
        if (extracted)
        {
            frame->GetData(XBEE_RESP_EXP_RX_DATA_INDEX, data, data_length, XBEE_RESP_MAX_PACKET_SIZE);
        }
    }        
}

ExplicitIOSample::ExplicitIOSample(ApiFrame& rsp)
{
    if (rsp.extracted && (rsp.api_id == ApiID::RECEIVE_PACKET))
    {
        frame = rsp.frame;
        extracted = frame->GetFields(XBEE_RESP_EXP_RX_INDEX, source_addr,
                source_nework_addr, options, number_of_samples,
                digital_channel_mask, analog_channel_mask);
        
        // TODO: Are digital sample bytes always in the message?
        // TODO: Are analog sample bytes always in the message?
    }        
}

NodeID::NodeID(ApiFrame& rsp)
{
    if (rsp.extracted && (rsp.api_id == ApiID::MODEM_STATUS))
    {
        frame = rsp.frame;
        type = DeviceType::INVALID;
        uint8_t type_i;
        uint8_t pushbutton_i;
        uint16_t reserved;
        extracted = frame->GetFields(XBEE_RESP_NODE_ID_INDEX, source_addr,
                reserved, options, reserved, remote_addr);
        
        if (extracted)
        {
            extracted = frame->GetField(XBEE_RESP_NODE_ID_STR_INDEX, identifier, 
                    identifier_len, XBEE_AT_NI_IDENT_LEN);
        }
        if (extracted)
        {
            extracted = frame->GetFields(XBEE_RESP_NODE_ID_STR_INDEX + identifier_len,
                    type_i, pushbutton_i, digi_profile_id, digi_mfr_id);
            
            // TODO:: Optional fields
        }
        
        if (extracted)
        {
            if ((type_i == static_cast<uint8_t>(DeviceType::DEVICE_TYPE_COORDINATOR)) ||
                (type_i == static_cast<uint8_t>(DeviceType::DEVICE_TYPE_NORMAL)) ||
                (type_i == static_cast<uint8_t>(DeviceType::DEVICE_TYPE_END)))
            {
                type = static_cast<DeviceType>(type_i);
            }
        }
    }        
}

} }

