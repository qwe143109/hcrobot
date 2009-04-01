#ifndef RoboduinoServer_H
#define RoboduinoServer_H

#include <inttypes.h>
#include "Print.h"

class RoboduinoClient;

class RoboduinoServer: public Print
{
public:
    RoboduinoServer(uint16_t port);
    
    void begin();
    virtual void write(uint8_t c);
    
    RoboduinoClient available();
    
private:
    void accept();
    
    uint16_t    m_port;
};

#endif  // RoboduinoServer_H

