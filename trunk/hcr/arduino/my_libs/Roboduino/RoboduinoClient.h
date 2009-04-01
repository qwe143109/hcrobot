#ifndef RoboduinoClient_H
#define RoboduinoClient_H

#include <inttypes.h>
#include "Print.h"

class RoboduinoClient: public Print
{
    friend class RoboduinoServer;
  
public:
    RoboduinoClient(uint8_t sock);
    RoboduinoClient(uint8_t *ip, uint16_t port);
  
    uint8_t connect();
    uint8_t connected();
    uint8_t status();
    
    int available();
    
    int read();
    virtual void write(uint8_t c);
    void flush();
    void stop();
    
    uint8_t operator==(int p);
    uint8_t operator!=(int p);
    operator bool();
  
private:
    uint8_t             m_sock;
    uint8_t*            m_ip;
    uint16_t            m_port;
    
    static uint16_t     sm_srcport;
};

#endif  // RoboduinoClient_H

