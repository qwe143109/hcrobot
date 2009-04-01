/*
  RoboduinoMotor.h - Library for Roboduino Motor.
  Created by ChaiShushan(chaishushan@gmail.com), March 28, 2009.
  Released into the public domain.
*/

#include "RoboduinoMotor.h"
#include <WProgram.h>

//====================================================================
//====================================================================

// ����ʵ��

RoboduinoMotorClass RoboduinoMotor;

//====================================================================
//====================================================================

RoboduinoMotorClass::RoboduinoMotorClass()
{
    m_bMapMotor = false;    // �Ƿ��޸���ӳ��
    m_bSwapM1M2 = false;    // �Ƿ񽻻����
    
    m_u8LCoff   = 1;        // ����ϵ��
    m_u8RCoff   = 1;        // ����ϵ��
    
    m_u8LSpeed  = 0;        // �����ٶ�
    m_u8RSpeed  = 0;        // �����ٶ�
    
    m_u8M1Speed = 0;        // M1����ٶ�
    m_u8M2Speed = 0;        // M2����ٶ�

    // ��������ģʽ
    
    pinMode(M1_Enable, OUTPUT);
    pinMode(M1_Ctrl  , OUTPUT);
    pinMode(M2_Enable, OUTPUT);
    pinMode(M2_Ctrl  , OUTPUT);
}

//====================================================================
//====================================================================

// ������ٶ�

int RoboduinoMotorClass::motorRead(int idx)const
{
    if(idx == 0)
    {
        return m_u8M1Speed;
    }
    else if(idx == 1)
    {
        return m_u8M2Speed;
    }
    else
    {
        return 0;
    }
}
    
// ���õ���ٶ�
// Ŀǰֻ֧��ȫ��ǰ��/ȫ�ٺ���/ֹͣ

void RoboduinoMotorClass::motorWrite(int idx, int val)
{
    if(idx != 0 && idx != 1) return;

    // ��Ӧ����
    
    int enPin = (idx==0)? M1_Enable: M2_Enable;
    int inPin = (idx==0)? M1_Ctrl  : M2_Ctrl;
    
    // �����ٶ�
    
    if(val > 0)
    {
        // ��ת
        
        digitalWrite(enPin, HIGH);
        digitalWrite(inPin, HIGH);
        
        // ��¼����ٶ�
        
        if(idx == 0)
        {
            m_u8M1Speed = 100;
        }
        else
        {
            m_u8M2Speed = 100;
        } 
    }
    else if(val < 0)
    {
        // ��ת
        
        digitalWrite(enPin, HIGH);
        digitalWrite(inPin, LOW);
        
        // ��¼����ٶ�
        
        if(idx == 0)
        {
            m_u8M1Speed = -100;
        }
        else
        {
            m_u8M2Speed = -100;
        }
    }
    else
    {
        // ֹͣ
        
        digitalWrite(enPin, LOW);
        
        // ��¼����ٶ�
        
        if(idx == 0)
        {
            m_u8M1Speed = 0;
        }
        else
        {
            m_u8M2Speed = 0;
        }
    }
}
    
//====================================================================
// �߼�����(���ڵͼ�������װ)
//====================================================================

// ӳ������������
    
void RoboduinoMotorClass::mapMotor(int lCoff, int rCoff, bool swapM1M2)
{
    m_bSwapM1M2 = swapM1M2;
    
    m_u8LCoff = lCoff;
    m_u8RCoff = rCoff;
    
    if(m_u8LCoff == 1 && m_u8RCoff == 1 && (!m_bSwapM1M2))
    {
        m_bMapMotor = false;
    }
    else
    {
        m_bMapMotor = true;
    }
}
    
// �������
    
void RoboduinoMotorClass::start(int lSpeed, int rSpeed)
{
    // �ٶȵ�����[-100, 100]
    
    if(lSpeed > SpeedMax) lSpeed = SpeedMax;
    if(lSpeed < (0-SpeedMax)) lSpeed = (0-SpeedMax);
    if(rSpeed > SpeedMax) rSpeed = SpeedMax;
    if(rSpeed < (0-SpeedMax)) rSpeed = (0-SpeedMax);
    
    // ��¼�����ٶ�
    
    m_u8LSpeed = lSpeed;
    m_u8RSpeed = rSpeed;
    
    // �������ٶ�
    
    if(m_bMapMotor)
    {
        if(m_bSwapM1M2)
        {
            m_u8M1Speed = rSpeed*m_u8RCoff;
            m_u8M2Speed = lSpeed*m_u8LCoff;
        }
        else
        {
            m_u8M1Speed = lSpeed*m_u8LCoff;
            m_u8M2Speed = rSpeed*m_u8RCoff;
        }
    }
    else
    {
        m_u8M1Speed = lSpeed;
        m_u8M2Speed = rSpeed;
    }
    
    // �������
    
    motorWrite(0, m_u8M1Speed);
    motorWrite(1, m_u8M2Speed);
}
    
// ֹͣ
    
void RoboduinoMotorClass::stop()
{
    m_u8LSpeed = 0;
    m_u8RSpeed = 0;
    
    motorWrite(0, 0);
    motorWrite(1, 0);
}
    
// �����ٶ�

int RoboduinoMotorClass::leftSpeed()const
{
    return m_u8LSpeed;
}

// �����ٶ�

int RoboduinoMotorClass::rightSpeed()const
{
    return m_u8RSpeed;
}

//====================================================================
//====================================================================

