/*
  RoboduinoEthernet.h - Library for Roboduino Ethernet.
  Created by ChaiShushan(chaishushan@gmail.com), March 28, 2009.
  Released into the public domain.
*/

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

// 操作实体

RoboduinoEthernetClass RoboduinoEthernet;

//====================================================================
//====================================================================

//constructor
RoboduinoEthernetClass::RoboduinoEthernetClass()
{
	m_wwwport = 80;
}

//====================================================================
//====================================================================

void RoboduinoEthernetClass::begin(uint8_t *mac, uint8_t *ip)
{
    uint8_t gateway[4];
    gateway[0] = ip[0];
    gateway[1] = ip[1];
    gateway[2] = ip[2];
    gateway[3] = 1;
    begin(mac, ip, gateway);
}

void RoboduinoEthernetClass::begin(uint8_t *mac, uint8_t *ip, uint8_t *gateway)
{
    uint8_t subnet[] = { 255, 255, 255, 0 };
    begin(mac, ip, gateway, subnet);
}

void RoboduinoEthernetClass::begin(uint8_t *mac, uint8_t *ip, uint8_t *gateway, uint8_t *subnet)
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

#define BUFFER_SIZE 500
#define STR_BUFFER_SIZE 22

// 可读的数据

int RoboduinoEthernetClass::available()
{
	uint16_t plen, dat_p;
    int8_t cmd;
    
    // plen will ne unequal to zero if there is a valid packet 
    
    plen = E_enc28j60PacketReceive(BUFFER_SIZE, sm_buf);
    if(plen == 0) return 0;
    
    // arp is broadcast if unknown but a host may also verify 
    // the mac address by sending it to a unicast address.
	
    if(E_eth_type_is_arp_and_my_ip(sm_buf, plen))
	{
        E_make_arp_answer_from_request(sm_buf);
        return 0;
    }
    
    // check if ip packets are for us:
	
    if(RoboduinoEthernet.E_eth_type_is_ip_and_my_ip(sm_buf, plen)==0)
	{
        return 0;
    }
    
    if(sm_buf[IP_PROTO_P] == IP_PROTO_ICMP_V 
		&& sm_buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V)
	{
        E_make_echo_reply_from_request(sm_buf,plen);
        return 0;
    }
    
    // tcp port www start, compare only the lower byte
	
    if (sm_buf[IP_PROTO_P]==IP_PROTO_TCP_V
        && sm_buf[TCP_DST_PORT_H_P]== 0
        && sm_buf[TCP_DST_PORT_L_P]== m_wwwport)
    {
        if (sm_buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V)
		{
			// make_tcp_synack_from_syn does already send the syn,ack
			
			E_make_tcp_synack_from_syn(sm_buf); 
			return 0;     
        }
        if (sm_buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)
		{
			E_init_len_info(sm_buf); // init some data structures
			dat_p = E_get_tcp_data_pointer();
			
			// we can possibly have no data, just ack:
			
			if(dat_p==0)
			{ 
				if (sm_buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V)
				{
					E_make_tcp_ack_from_any(sm_buf);
				}
				return 0;
			}
			
			if (strncmp("GET ",(char *)&(sm_buf[dat_p]),4)!=0)
			{
				// head, post and other methods for possible status codes 
				// http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html

				const char *httphead = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 OK</h1>";
				
				E_fill_tcp_data_p(sm_buf, 0, httphead);
				goto SENDTCP;
			}
			if (strncmp("/ ",(char *)&(sm_buf[dat_p+4]),2)==0)
			{
				// 打印页面
				
				//plen = print_webpage(sm_buf);
				goto SENDTCP;
			}
			
			// 分析
			
			//cmd = analyse_cmd((char *)&(buf[dat_p+5]));
			if (cmd==1)
			{
				//	plen=print_webpage(buf);
			}
SENDTCP:
			// send ack for http get
			E_make_tcp_ack_from_any(sm_buf); 
			// send data
			E_make_tcp_ack_with_data(sm_buf,plen); 
        }
    }
	return 0;
}

// 发送数据

int RoboduinoEthernetClass::send(void *data, int8_t len)
{
	return 0;
}

// 发送数据

int RoboduinoEthernetClass::recv(void *vuf, int8_t len)
{
	return 0;
}

//====================================================================
//====================================================================

uint16_t RoboduinoEthernetClass::E_fill_tcp_data_p(uint8_t *buf,uint16_t pos, const prog_char *progmem_s)
{
	return fill_tcp_data_p(buf, pos, progmem_s);
}

uint16_t RoboduinoEthernetClass::E_fill_tcp_data(uint8_t *buf,uint16_t pos, const char *s)
{
	return fill_tcp_data(buf,pos, s);
}


void RoboduinoEthernetClass::E_enc28j60Init(uint8_t* macaddr){
	enc28j60Init(macaddr);

}
void RoboduinoEthernetClass::E_enc28j60clkout(uint8_t clk){
	enc28j60clkout(clk);
}

void RoboduinoEthernetClass::E_enc28j60PhyWrite(uint8_t address, uint16_t data){
	enc28j60PhyWrite(address,  data);
}

uint16_t RoboduinoEthernetClass::E_enc28j60PacketReceive(uint16_t len, uint8_t* packet){
	return enc28j60PacketReceive(len, packet);
}


void RoboduinoEthernetClass::E_init_ip_arp_udp_tcp(uint8_t *mymac,uint8_t *myip,uint8_t wwwp){
	init_ip_arp_udp_tcp(mymac,myip,wwwp);
}

uint8_t RoboduinoEthernetClass::E_eth_type_is_arp_and_my_ip(uint8_t *buf,uint16_t len){
	return eth_type_is_arp_and_my_ip(buf,len);
}

void RoboduinoEthernetClass::E_make_arp_answer_from_request(uint8_t *buf){
	make_arp_answer_from_request(buf);
}

uint8_t RoboduinoEthernetClass::E_eth_type_is_ip_and_my_ip(uint8_t *buf,uint16_t len){
	return eth_type_is_ip_and_my_ip(buf, len);
}


void RoboduinoEthernetClass::E_make_echo_reply_from_request(uint8_t *buf,uint16_t len){
	make_echo_reply_from_request(buf,len);
}

void RoboduinoEthernetClass::E_make_tcp_synack_from_syn(uint8_t *buf){
	make_tcp_synack_from_syn(buf);
}	

void RoboduinoEthernetClass::E_init_len_info(uint8_t *buf){
	init_len_info(buf);
}

uint16_t RoboduinoEthernetClass::E_get_tcp_data_pointer(void){
	return get_tcp_data_pointer();
}

void RoboduinoEthernetClass::E_make_tcp_ack_from_any(uint8_t *buf){
	make_tcp_ack_from_any(buf);
}

void RoboduinoEthernetClass::E_make_tcp_ack_with_data(uint8_t *buf,uint16_t dlen){
	make_tcp_ack_with_data(buf,dlen);
}

//====================================================================
//====================================================================
