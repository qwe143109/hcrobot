/*
  RoboduinoClient.h - Library for Roboduino Ethernet.
  Created by ChaiShushan(chaishushan@gmail.com), March 28, 2009.
  Released into the public domain.
*/

#include "RoboduinoEthernet.h"
#include "RoboduinoClient.h"
#include "RoboduinoServer.h"

//====================================================================
//====================================================================

RoboduinoClient::RoboduinoClient(uint8_t sock)
{

}
RoboduinoClient::RoboduinoClient(uint8_t *ip, uint16_t port)
{

}

//====================================================================
//====================================================================

uint8_t RoboduinoClient::connect()
{
    return 0;
}
uint8_t RoboduinoClient::connected()
{
    return 0;
}
uint8_t RoboduinoClient::status()
{
    return 0;
}

//====================================================================
//====================================================================
int RoboduinoClient::available()
{
    return 0;
}
    
int RoboduinoClient::read()
{
    return 0;
}
void RoboduinoClient::write(uint8_t c)
{
}
void RoboduinoClient::flush()
{
}
void RoboduinoClient::stop()
{
}

//====================================================================
//====================================================================

uint8_t RoboduinoClient::operator==(int p)
{
    return 0;
}
uint8_t RoboduinoClient::operator!=(int p)
{
    return 0;
}
RoboduinoClient::operator bool()
{
    return 0;
}

//====================================================================
//====================================================================


#if 0

extern "C" {
  #include "types.h"
  #include "w5100.h"
  #include "socket.h"
}

#include "Ethernet.h"
#include "Client.h"
#include "Server.h"

uint16_t Client::_srcport = 0;

Client::Client(uint8_t sock) {
  _sock = sock;
}

Client::Client(uint8_t *ip, uint16_t port) {
  _ip = ip;
  _port = port;  
}

uint8_t Client::connect() {
  _sock = 255;
  
  for (int i = 0; i < MAX_SOCK_NUM; i++) {
    if (getSn_SR(i) == SOCK_CLOSED) {
      _sock = i;
    }
  }
  
  if (_sock == 255)
    return 0;
    
  _srcport++;
    
  // XXX: what port should we connect from?
  socket(_sock, Sn_MR_TCP, _port, 1024 + _srcport);
  
  if (!::connect(_sock, _ip, _port))
    return 0;
    
  while (status() != SOCK_ESTABLISHED) {
    if (status() == SOCK_CLOSED)
      return 0;
  }
  
  return 1;
}

void Client::write(uint8_t b) {
  send(_sock, &b, 1);
}

int Client::available() {
  return getSn_RX_RSR(_sock);
}

int Client::read() {
  uint8_t b;
  if (!available())
    return -1;
  recv(_sock, &b, 1);
  return b;
}

void Client::flush() {
  while (available())
    read();
}

void Client::stop() {
  close(_sock);
  disconnect(_sock);
  EthernetClass::_server_port[_sock] = 0;
}

uint8_t Client::connected() {
  uint8_t s = status();
  return !(s == SOCK_LISTEN || s == SOCK_CLOSED || (s == SOCK_CLOSE_WAIT && !available()));
}

uint8_t Client::status() {
  return getSn_SR(_sock);
}

// the next three functions are a hack so we can compare the client returned
// by Server::available() to null, or use it as the condition in an
// if-statement.  this lets us stay compatible with the Processing network
// library.

uint8_t Client::operator==(int p) {
  return _sock == 255;
}

uint8_t Client::operator!=(int p) {
  return _sock != 255;
}

Client::operator bool() {
  return _sock != 255;
}

#endif

