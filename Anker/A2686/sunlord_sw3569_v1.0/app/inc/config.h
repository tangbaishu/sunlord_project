/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file config.h
 * @brief The config for customization
 * @author SD1 & SW team
 ****************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H

#include "config_types.h"
#include "system.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off
typedef struct
{
    const volatile uint8_t *ufcsSrcCap;                         //!< ufcs src cap data
    const volatile uint32_t* pdSrcCap;                          //!< pd src cap data
    uint32_t xid : 32;                                          //!< XID
    uint16_t vid : 16;                                          //!< Vendor ID
    uint16_t pid : 16;                                          //!< PID
    uint16_t bcdDevice : 16;                                    //!< bcd device;
                        
    uint8_t pdMaxPower : 8;                                     //!< pd max power, unit: W
    uint8_t tfcpMaxPower : 8;                                   //!< tfcp max power, unit : W
               
    uint8_t voocMaxPower : 8;                                   //!< VOOC1.0 VOOC4.0 Max Power, unit : W
    uint8_t superVoocMaxPower : 8;                              //!< SuperVOOC Max Power, unit : W
    uint8_t sprPdoNumber : 8;                                   //!< spr pdo number
    uint8_t sprFixPdoNumber : 8;                                //!< fix pdo number
    uint8_t eprPdoNumber : 8;                                   //!< epr pdo number
    uint8_t eprFixPdoNumber : 8;                                //!< fix pdo number
    uint16_t scpMaxCurrent : 16;                                //!< scp max current, uint: 100mA, value only can be set in range 1-6, 10~63, 100~630
    uint8_t scpMaxPower : 8;                                    //!< scp max power, unit: W, 1-127
    uint16_t scpMaxVoltage : 16;                                //!< scp max voltage, unit: 100mV, value only can be set in range 1-6, 10~63, 100~630
    uint8_t ufcsSrcCapNum: 8;                                   //!< ufcs src cap num
    uint8_t scpAdapterType: 8;                                  //!< scp class b adaptor type
               
    uint8_t voocPathImpedanceGenuineCableTyp150 : 8;            //!< vooc path impedance genuine cable type 150
    uint8_t voocPathImpedanceGenuineCableTyp180 : 8;            //!< vooc path impedance genuine cable type 180
    uint8_t voocPathImpedanceGenuineCableTyp250 : 8;            //!< vooc path impedance genuine cable type 250
    uint8_t voocPathImpedanceCommonCableTyp150 : 8;             //!< vooc path impedance common cable type 150
    uint8_t voocPathImpedanceCommonCableTyp180 : 8;             //!< vooc path impedance common cable type 180
    uint8_t voocPathImpedanceCommonCableTyp250 : 8;             //!< vooc path impedance common cable type 250
                   
    uint8_t dpdmFixPower : 6;                                   //!< afc/fcp/sfcp/pe/qc2.0 power, unit : W   

    port_mode_e portMode:3;                                     //!< port mode, see port_mode_e
    typec_cc_term_set_e typecCcTerm : 2;                        //!< typec cc term set
    fix_voltage_offset_e fixVoltageOffset : 2;                  //!< fix voltage offset
    fix_current_offset_e fixCurrentOffset : 2;                  //!< fix current offset
    qc2p0_max_voltage_level_e qc2p0MaxVoltage : 2;              //!< qc2.0 max voltage
    qc3p0_max_voltage_level_e qc3p0MaxVoltage : 2;              //!< qc3.0 max voltage
    pe_max_voltage_level_e peMaxVoltage : 2;                    //!< pe max voltage
    qc3_max_power_level_e qc3MaxPower : 2;                      //!< qc3.0 & qc3.0+ max power
    port_current_limit_e p1CurrentLimit: 2;                     //!< port 1 current limit
    port_current_limit_e p2CurrentLimit: 2;                     //!< port 2 current limit
    cable_compensave_value_e cableCompensationValue : 1;        //!< cable compensave value
    afc_max_voltage_level_e afcMaxVoltage : 1;                  //!< afc max voltage
    sfcp_max_voltage_level_e sfcpMaxVoltage : 1;                //!< sfcp max voltage
    fcp_max_voltage_level_e fcpMaxVoltage : 1;                  //!< fcp max voltage

    bool isCableCompensaved:1;                                  //!< whether to enable cable compensave
    bool isTypecVconnEnabled : 1;                               //!< whether typec vconn enable
    bool isVconnSwapEnabled : 1;                                //!< whether to support vconn swap
    bool isResponseDiscoveryId : 1;                             //!< whether to response discover id

    bool isTfcpEnabled : 1;                                     //!< whether to enable tfcp
    bool isVooc1p0Enabled : 1;                                  //!< whether to enable vooc 1.0
    bool isVooc4p0Enabled : 1;                                  //!< whether to enable vooc 4.0
    bool isSuperVooc2p0Enabled : 1;                             //!< whether to enable vooc supper 2.0
    bool isPeEnabled : 1;                                       //!< whether to enable pe
    bool isQc2p0Enabled : 1;                                    //!< whether to enable qc2.0
    bool isQc3p0Enabled : 1;                                    //!< whether to enable qc3.0
    bool isQc3plusEnabled : 1;                                  //!< whether to enable qc3+
    bool isAfcEnabled : 1;                                      //!< whether to enable afc
    bool isFcpEnabled : 1;                                      //!< whether to enable FCP
    bool isHvScpEnabled : 1;                                    //!< whether to enable high voltage scp
    bool isLvScpEnabled : 1;                                    //!< whether to enable low voltage scp
    bool isSfcpEnabled : 1;                                     //!< whether to enable sfcp
    bool isAppleEnabled : 1;                                    //!< whether to enable apple 2.7v mode
    bool isSamsungEnabled: 1;                                   //!< whether to enable samsung 1.2v mode 
    bool isUfcsEnabled : 1;                                     //!< whether to enable ufcs
 
    bool isPdGatingVoocEnabled : 1;                             //!< whether to suppress vooc while PD is working
    bool isPdGatingScpEnabled : 1;                              //!< whether to suppress SCP while PD is working
 
    bool isVoocCableStricMode : 1;                              //!< whether to enable VOOC Cable strict mode
    bool isEmarkEnabled : 1;                                    //!< whether to read emark
    bool isPdEnabled : 1;                                       //!< whether to enable PD
 
    bool isVconnLowPowerModeEnabled : 1;                        //!< whether to close vconn after cable reading
    bool isInvalidPpsEnabled : 1;                               //!< whether to send hardreset and disable pps while receive a invalid pps request 
    bool isPdo5v2aEnabled : 1;                                  //!< whether to re-brocast 5v2a src cap while within 8s
    bool isPortCNoLoad: 1;                                      //!< whether to do port c noload detection
    bool isTypecCertEnabled: 1;                                 //!< whether to open typec certification mode, the difference is disconnect time 
                                                                //!< 0: disconnect time is 200ms, 1: disconnect time is 12ms
    bool isPdCertEnabled: 1;                                    //!< whether to open pd certification mode, when it is true, dpdm short, dpdm protocol will not run;
    bool isPdAlertEnabled: 1;                                   //!< whether to send alert while cc and cv switching   
    bool isAbnLpsEnabled: 1;                                    //!< whether to enable lps abn   
    
    uint8_t pdVersionMinor:4;                                   //!< the version of pd revision
    uint8_t typecConnectTime;                                   //!< the connect time of typec,this time just typec time, really vbus_on time will > this, 1ms/step
    
    bool isUfcsCertEnabled: 1;                                  //!< whether to open ufcs certification mode, when it is true, dp < 1.05v apple->samsung; 

    uint8_t pdVdmVer:4;                                         //!< the version of pd vdm, pdVdmVer = 4 -> vdm version 2.0 pdVdmVer = 5 -> vdm version 2.1
    
    uint8_t ufcsDevTemp;                                        //!< the device temperatrue of ufcs src_info message, unit: degree
    uint8_t ufcsUsbTemp;                                        //!< the usb port temperatrue of ufcs src_info message, unit: degree

    bool isIgnoreEprCable: 1;                                   //!< whether ignore epr cable,true,no care cable,send 140w epr src cap
	
	bool isPdCertWithApple: 1;                                  //!< whether pd cert with apple
	
	uint16_t vPpsShutDownInMv;                                //!< vppsshutdown threshold,1mv/step
} config_data_t;


#ifdef __cplusplus
}
#endif

#endif        // end of CONFIG_H
