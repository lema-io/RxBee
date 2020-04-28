#ifndef RXBEE_COMMAND_H
#define RXBEE_COMMAND_H

#include <string>
#include <stdint.h>

#define XBEE_CMD_AC "AC"
#define XBEE_CMD_FR "FR"
#define XBEE_CMD_RE "RE"
#define XBEE_CMD_WR "WR"

#define XBEE_CMD_AF "AF"
#define XBEE_CMD_CM "CM"
#define XBEE_CMD_MF "MF"
#define XBEE_CMD_HP "HP"    // Preabmle ID
#define XBEE_CMD_ID "ID"    // Network ID
#define XBEE_CMD_MT "MT"
#define XBEE_CMD_PL "PL"
#define XBEE_CMD_RR "RR"
#define XBEE_CMD_ED "ED"

#define XBEE_CMD_BC "BC"
#define XBEE_CMD_DB "DB"
#define XBEE_CMD_ER "ER"
#define XBEE_CMD_GD "GD"
#define XBEE_CMD_EA "EA"
#define XBEE_CMD_TR "TR"
#define XBEE_CMD_UA "UA"


#define XBEE_CMD_CE "CE"
#define XBEE_CMD_BH "BH"
#define XBEE_CMD_NH "NH"
#define XBEE_CMD_NN "NN"
#define XBEE_CMD_MR "MR"

#define XBEE_CMD_SH "SH"
#define XBEE_CMD_SL "SL"
#define XBEE_CMD_DH "DH"
#define XBEE_CMD_DL "DL"
#define XBEE_CMD_TO "TO"
#define XBEE_CMD_NI "NI"
#define XBEE_CMD_NT "NT"
#define XBEE_CMD_NO "NO"
#define XBEE_CMD_CI "CI"
#define XBEE_CMD_DE "DE"
#define XBEE_CMD_SE "SE"

#define XBEE_CMD_AG "AG"
#define XBEE_CMD_DN "DN"
#define XBEE_CMD_ND "ND"    // Network Discover
#define XBEE_CMD_FN "FN"

#define XBEE_CMD_EE "EE"
#define XBEE_CMD_KY "KY"

#define XBEE_CMD_BD "BD"
#define XBEE_CMD_NB "NB"
#define XBEE_CMD_SB "SB"
#define XBEE_CMD_RO "RO"
#define XBEE_CMD_FT "FT"
#define XBEE_CMD_AP "AP"
#define XBEE_CMD_AO "AO"

#define XBEE_CMD_CB "CB"
#define XBEE_CMD_D0 "D0"
#define XBEE_CMD_D1 "D1"
#define XBEE_CMD_D2 "D2"
#define XBEE_CMD_D3 "D3"
#define XBEE_CMD_D4 "D4"
#define XBEE_CMD_D5 "D5"
#define XBEE_CMD_D6 "D6"
#define XBEE_CMD_D7 "D7"
#define XBEE_CMD_D8 "D8"
#define XBEE_CMD_D9 "D9"
#define XBEE_CMD_P0 "P0"
#define XBEE_CMD_P1 "P1"
#define XBEE_CMD_P2 "P2"
#define XBEE_CMD_P3 "P3"
#define XBEE_CMD_P4 "P4"
#define XBEE_CMD_PD "PD"
#define XBEE_CMD_PR "PR"
#define XBEE_CMD_M0 "M0"
#define XBEE_CMD_M1 "M1"
#define XBEE_CMD_LT "LT"
#define XBEE_CMD_RP "RP"

#define XBEE_CMD_AV "AV"
#define XBEE_CMD_IC "IC"
#define XBEE_CMD_IF "IF"
#define XBEE_CMD_IR "IR"
#define XBEE_CMD_IS "IS"
#define XBEE_CMD_TP "TP"

#define XBEE_CMD_SM "SM"
#define XBEE_CMD_SO "SO"
#define XBEE_CMD_SN "SN"
#define XBEE_CMD_SP "SP"
#define XBEE_CMD_ST "ST"
#define XBEE_CMD_WH "WH"

#define XBEE_CMD_SS "SS"
#define XBEE_CMD_OS "OS"
#define XBEE_CMD_OW "OW"
#define XBEE_CMD_MS "MS"
#define XBEE_CMD_SQ "SQ"

#define XBEE_CMD_CC "CC"
#define XBEE_CMD_CN "CN"
#define XBEE_CMD_CT "CT"
#define XBEE_CMD_GT "GT"

#define XBEE_CMD_VL "VL"
#define XBEE_CMD_VR "VR"
#define XBEE_CMD_HV "HV"
#define XBEE_CMD_HS "HS"
#define XBEE_CMD_DD "DD"
#define XBEE_CMD_NP "NP"
#define XBEE_CMD_CK "CK"

#define XBEE_CMD(ID) XBEE_CMD_ ## ID

static const char XBEE_AT_CMD[][3] = {
    XBEE_CMD_AC,
    XBEE_CMD_AF,
    XBEE_CMD_AG,
    XBEE_CMD_AO,
    XBEE_CMD_AP,
    XBEE_CMD_AV,
    XBEE_CMD_BC,
    XBEE_CMD_BD,
    XBEE_CMD_BH,
    XBEE_CMD_CB,
    XBEE_CMD_CC,
    XBEE_CMD_CE,
    XBEE_CMD_CI,
    XBEE_CMD_CK,
    XBEE_CMD_CM,
    XBEE_CMD_CN,
    XBEE_CMD_CT,
    XBEE_CMD_D0,
    XBEE_CMD_D1,
    XBEE_CMD_D2,
    XBEE_CMD_D3,
    XBEE_CMD_D4,
    XBEE_CMD_D5,
    XBEE_CMD_D6,
    XBEE_CMD_D7,
    XBEE_CMD_D8,
    XBEE_CMD_D9,
    XBEE_CMD_DB,
    XBEE_CMD_DD,
    XBEE_CMD_DE,
    XBEE_CMD_DH,
    XBEE_CMD_DL,
    XBEE_CMD_DN,
    XBEE_CMD_EA,
    XBEE_CMD_ED,
    XBEE_CMD_EE,
    XBEE_CMD_ER,
    XBEE_CMD_FN,
    XBEE_CMD_FR,
    XBEE_CMD_FT,
    XBEE_CMD_GD,
    XBEE_CMD_GT,
    XBEE_CMD_HP,
    XBEE_CMD_HS,
    XBEE_CMD_HV,
    XBEE_CMD_IC,
    XBEE_CMD_ID,
    XBEE_CMD_IF,
    XBEE_CMD_IR,
    XBEE_CMD_IS,
    XBEE_CMD_KY,
    XBEE_CMD_LT,
    XBEE_CMD_M0,
    XBEE_CMD_M1,
    XBEE_CMD_MF,
    XBEE_CMD_MR,
    XBEE_CMD_MS,
    XBEE_CMD_MT,
    XBEE_CMD_NB,
    XBEE_CMD_ND,
    XBEE_CMD_NH,
    XBEE_CMD_NI,
    XBEE_CMD_NN,
    XBEE_CMD_NO,
    XBEE_CMD_NP,
    XBEE_CMD_NT,
    XBEE_CMD_OS,
    XBEE_CMD_OW,
    XBEE_CMD_P0,
    XBEE_CMD_P1,
    XBEE_CMD_P2,
    XBEE_CMD_P3,
    XBEE_CMD_P4,
    XBEE_CMD_PD,
    XBEE_CMD_PL,
    XBEE_CMD_PR,
    XBEE_CMD_RE,
    XBEE_CMD_RO,
    XBEE_CMD_RP,
    XBEE_CMD_RR,
    XBEE_CMD_SB,
    XBEE_CMD_SE,
    XBEE_CMD_SH,
    XBEE_CMD_SL,
    XBEE_CMD_SM,
    XBEE_CMD_SN,
    XBEE_CMD_SO,
    XBEE_CMD_SP,
    XBEE_CMD_SQ,
    XBEE_CMD_SS,
    XBEE_CMD_ST,
    XBEE_CMD_TO,
    XBEE_CMD_TP,
    XBEE_CMD_TR,
    XBEE_CMD_UA,
    XBEE_CMD_VL,
    XBEE_CMD_VR,
    XBEE_CMD_WH,
    XBEE_CMD_WR
};

enum class XBeeATCommand {
    AC, AF, AG, AO, AP, AV, 
    BC, BD, BH,
    CB, CC, CE, CI, CK, CM, CN, CT,
    D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, DB, DD, DE, DH, DL, DN,
    EA, ED, EE, ER,
    FN, FR, FT,
    GD, GT,
    HP, HS, HV,
    IC, ID, IF, IR, IS,
    KY,
    LT,
    M0, M1, MF, MR, MS, MT,
    NB, ND, NH, NI, NN, NO, NP, NT, 
    OS, OW,
    P0, P1, P2, P3, P4, PD, PL, PR,
    RE, RO, RP, RR,
    SB, SE, SH, SL, SM, SN, SO, SP, SQ, SS, ST,
    TO, TP, TR,
    UA,  
    VL, VR,
    WH, WR,
    INVALID
};


#endif // RXBEE_COMMAND_H
