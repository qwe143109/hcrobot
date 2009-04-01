#include <RoboduinoMotor.h>

void setup()
{
    Serial.begin(9600);
    
    // 左右轮和电机的映射关系:
    //
    // 1. 左轮对应电机M2, 电机正转前进, 反转后退.
    // 2. 右轮对应电机M1, 电机正转后退, 反转前进.
    
    RoboduinoMotor.mapMotor(1, -1, true);
}
void loop()
{
    if(Serial.available() <= 0) return;
    
    int val = Serial.read();
    if(val == 'a')
    {
        // 左轮速度100
        // 右轮速度100
        // 前进
        
        RoboduinoMotor.start(100, 100);
    }
    else if(val == 'b')
    {
        // 左轮速度-100
        // 右轮速度-100
        // 后退
        
        RoboduinoMotor.start(-100, -100);
    }
    else
    {
        // 全部停止
        
        RoboduinoMotor.stop();
    }
    
    // 输出速度
    
    {
        uint8_t left = RoboduinoMotor.leftSpeed();
        uint8_t right = RoboduinoMotor.rightSpeed();
        
        Serial.print("left speed: ");
        Serial.println(left);
        Serial.print("right speed: ");
        Serial.println(right);
    }
}

