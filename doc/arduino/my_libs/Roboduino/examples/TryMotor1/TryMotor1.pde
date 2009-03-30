#include <RoboduinoMotor.h>

void setup()
{
    Serial.begin(9600);
}
void loop()
{
    if(Serial.available() <= 0) return;
    
    int val = Serial.read();
    if(val == 'a')
    {
        // M1/M2��ת
        
        RoboduinoMotor::instance().motorWrite(0, 100);
        RoboduinoMotor::instance().motorWrite(1, 100);
    }
    else if(val == 'b')
    {
        // M1/M2��ת
        
        RoboduinoMotor::instance().motorWrite(0, -100);
        RoboduinoMotor::instance().motorWrite(1, -100);
    }
    else
    {
        // ֹͣ
        
        RoboduinoMotor::instance().motorWrite(0, 0);
        RoboduinoMotor::instance().motorWrite(1, 0);
    }
    
    // ����ٶ�
    
    {
        uint8_t m1 = RoboduinoMotor::instance().motorRead(0);
        uint8_t m2 = RoboduinoMotor::instance().motorRead(0);
        
        Serial.print("M1: ");
        Serial.println(m1);
        Serial.print("M2: ");
        Serial.println(m2);
    }
}