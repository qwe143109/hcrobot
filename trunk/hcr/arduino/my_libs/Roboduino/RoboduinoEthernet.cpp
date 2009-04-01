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

#include "RoboduinoEthernet.h"
#include <WProgram.h>

#include "enc28j60.h"
#include "ip_arp_udp_tcp.h"
#include "net.h"

//====================================================================
//====================================================================

RoboduinoEthernet RoboduinoEthernet::sm_ethernet;

RoboduinoEthernet& RoboduinoEthernet::instance()
{
    return sm_ethernet;
}

//====================================================================
//====================================================================

//constructor
RoboduinoEthernet::RoboduinoEthernet(){
}

//====================================================================
//====================================================================

void RoboduinoEthernet::begin(uint8_t *mac, uint8_t *ip)
{
    uint8_t gateway[4];
    gateway[0] = ip[0];
    gateway[1] = ip[1];
    gateway[2] = ip[2];
    gateway[3] = 1;
    begin(mac, ip, gateway);
}

void RoboduinoEthernet::begin(uint8_t *mac, uint8_t *ip, uint8_t *gateway)
{
    uint8_t subnet[] = { 255, 255, 255, 0 };
    begin(mac, ip, gateway, subnet);
}

void RoboduinoEthernet::begin(uint8_t *mac, uint8_t *ip, uint8_t *gateway, uint8_t *subnet)
{
#if 0
    iinchip_init();
    sysinit(0x55, 0x55);
    setSHAR(mac);
    setSIPR(ip);
    setGAR(gateway);
    setSUBR(subnet);
#endif

    /*initialize enc28j60*/
    E_enc28j60Init(mac);
    E_enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
    delay(10);
        
    /* Magjack leds configuration, see enc28j60 datasheet, page 11 */
    // LEDA=greed LEDB=yellow

    // 0x880 is PHLCON LEDB=on, LEDA=on
    // enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
    E_enc28j60PhyWrite(PHLCON,0x880);
    delay(500);
    
    // 0x990 is PHLCON LEDB=off, LEDA=off
    // enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
    E_enc28j60PhyWrite(PHLCON,0x990);
    delay(500);

    // 0x880 is PHLCON LEDB=on, LEDA=on
    // enc28j60PhyWrite(PHLCON,0b0000 1000 1000 00 00);
    E_enc28j60PhyWrite(PHLCON,0x880);
    delay(500);

    // 0x990 is PHLCON LEDB=off, LEDA=off
    // enc28j60PhyWrite(PHLCON,0b0000 1001 1001 00 00);
    E_enc28j60PhyWrite(PHLCON,0x990);
    delay(500);

    // 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
    // enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
    E_enc28j60PhyWrite(PHLCON,0x476);
    delay(100);
        
    //init the ethernet/ip layer:
    E_init_ip_arp_udp_tcp(mac, ip, 80);
}

//====================================================================
//====================================================================

uint16_t RoboduinoEthernet::E_fill_tcp_data_p(uint8_t *buf,uint16_t pos, const prog_char *progmem_s){
	return fill_tcp_data_p(buf, pos, progmem_s);
}

uint16_t RoboduinoEthernet::E_fill_tcp_data(uint8_t *buf,uint16_t pos, const char *s){
	return fill_tcp_data(buf,pos, s);
}


void RoboduinoEthernet::E_enc28j60Init(uint8_t* macaddr){
	enc28j60Init(macaddr);

}
void RoboduinoEthernet::E_enc28j60clkout(uint8_t clk){
	enc28j60clkout(clk);
}

void RoboduinoEthernet::E_enc28j60PhyWrite(uint8_t address, uint16_t data){
	enc28j60PhyWrite(address,  data);
}

uint16_t RoboduinoEthernet::E_enc28j60PacketReceive(uint16_t len, uint8_t* packet){
	return enc28j60PacketReceive(len, packet);
}


void RoboduinoEthernet::E_init_ip_arp_udp_tcp(uint8_t *mymac,uint8_t *myip,uint8_t wwwp){
	init_ip_arp_udp_tcp(mymac,myip,wwwp);
}

uint8_t RoboduinoEthernet::E_eth_type_is_arp_and_my_ip(uint8_t *buf,uint16_t len){
	return eth_type_is_arp_and_my_ip(buf,len);
}

void RoboduinoEthernet::E_make_arp_answer_from_request(uint8_t *buf){
	make_arp_answer_from_request(buf);
}

uint8_t RoboduinoEthernet::E_eth_type_is_ip_and_my_ip(uint8_t *buf,uint16_t len){
	return eth_type_is_ip_and_my_ip(buf, len);
}


void RoboduinoEthernet::E_make_echo_reply_from_request(uint8_t *buf,uint16_t len){
	make_echo_reply_from_request(buf,len);
}

void RoboduinoEthernet::E_make_tcp_synack_from_syn(uint8_t *buf){
	make_tcp_synack_from_syn(buf);
}	

void RoboduinoEthernet::E_init_len_info(uint8_t *buf){
	init_len_info(buf);
}

uint16_t RoboduinoEthernet::E_get_tcp_data_pointer(void){
	return get_tcp_data_pointer();
}

void RoboduinoEthernet::E_make_tcp_ack_from_any(uint8_t *buf){
	make_tcp_ack_from_any(buf);
}

void RoboduinoEthernet::E_make_tcp_ack_with_data(uint8_t *buf,uint16_t dlen){
	make_tcp_ack_with_data(buf,dlen);
}

//====================================================================
//====================================================================
