/*
  Ethernet library for Arduino ethernet shield

  Copyright (c) 2008 DFRobot.  All right reserved.
  http://www.DFRobot.com
   This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT EthernetANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef RoboduinoEthernet_H
#define RoboduinoEthernet_H

#include <inttypes.h>
#include <avr/pgmspace.h>
#include <Ethernet.h>

#include "utility/enc28j60.h"
#include "utility/net.h"

#include "RoboduinoClient.h"
#include "RoboduinoServer.h"

class RoboduinoEthernet
{
    friend class RoboduinoClient;
    friend class RoboduinoServer;
  
    RoboduinoEthernet(const RoboduinoEthernet&);
    RoboduinoEthernet& operator=(const RoboduinoEthernet&);
    
public:
    RoboduinoEthernet();

    static RoboduinoEthernet& instance();
    
    void begin(uint8_t *mac, uint8_t *ip);
    void begin(uint8_t *mac, uint8_t *ip, uint8_t *gateway);
    void begin(uint8_t *mac, uint8_t *ip, uint8_t *gateway, uint8_t *subnet);
    
    uint16_t E_fill_tcp_data_p(uint8_t *buf,uint16_t pos, const prog_char *progmem_s);
    uint16_t E_fill_tcp_data(uint8_t *buf,uint16_t pos, const char *s);
    
    uint16_t E_enc28j60PacketReceive(uint16_t len, uint8_t* packet);
    uint8_t E_eth_type_is_arp_and_my_ip(uint8_t *buf,uint16_t len);
    void E_make_arp_answer_from_request(uint8_t *buf);
    uint8_t E_eth_type_is_ip_and_my_ip(uint8_t *buf,uint16_t len);
    void E_make_echo_reply_from_request(uint8_t *buf,uint16_t len);
    void E_make_tcp_synack_from_syn(uint8_t *buf);
    void E_init_len_info(uint8_t *buf);
    uint16_t E_get_tcp_data_pointer(void);
    void E_make_tcp_ack_from_any(uint8_t *buf);
    void E_make_tcp_ack_with_data(uint8_t *buf,uint16_t dlen);
    
private:
    void E_enc28j60Init(uint8_t* macaddr);
    void E_enc28j60clkout(uint8_t clk);
    void E_enc28j60PhyWrite(uint8_t address, uint16_t data);
    void E_init_ip_arp_udp_tcp(uint8_t *mymac,uint8_t *myip,uint8_t wwwp);
    
    
    static uint8_t              sm_state[MAX_SOCK_NUM];
    static uint16_t             sm_server_port[MAX_SOCK_NUM];
    static RoboduinoEthernet    sm_ethernet;       // µ¥¼þ
};

#endif  // RoboduinoEthernet_H

