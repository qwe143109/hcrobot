#include <RoboduinoMotor.h>

void setup()
{
    Serial.begin(9600);
    
    // �����ֺ͵����ӳ���ϵ:
    //
    // 1. ���ֶ�Ӧ���M2, �����תǰ��, ��ת����.
    // 2. ���ֶ�Ӧ���M1, �����ת����, ��תǰ��.
    
    RoboduinoMotor.mapMotor(1, -1, true);
}
void loop()
{
    if(Serial.available() <= 0) return;
    
    int val = Serial.read();
    if(val == 'a')
    {
        // �����ٶ�100
        // �����ٶ�100
        // ǰ��
        
        RoboduinoMotor.start(100, 100);
    }
    else if(val == 'b')
    {
        // �����ٶ�-100
        // �����ٶ�-100
        // ����
        
        RoboduinoMotor.start(-100, -100);
    }
    else
    {
        // ȫ��ֹͣ
        
        RoboduinoMotor.stop();
    }
    
    // ����ٶ�
    
    {
        uint8_t left = RoboduinoMotor.leftSpeed();
        uint8_t right = RoboduinoMotor.rightSpeed();
        
        Serial.print("left speed: ");
        Serial.println(left);
        Serial.print("right speed: ");
        Serial.println(right);
    }
}

