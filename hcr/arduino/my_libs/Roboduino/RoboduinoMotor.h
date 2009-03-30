/*
  RoboduinoMotor.h - Library for Roboduino Motor.
  Created by ChaiShushan(chaishushan@gmail.com), March 28, 2009.
  Released into the public domain.
*/

#ifndef RoboduinoMotor_H
#define RoboduinoMotor_H

#include <inttypes.h>

/**
\brief Roboduino�������
\author ����ɼ(chaishushan@gmail.com)
\date 2009-03-29 ������

Roboduino Motor��չ��˵��

\image html RoboduinoMotor.jpg

����L293Bר�õ������оƬ, 1A�����H�������������, ��������ģ�����ʹ��,
ռ�����ֶ˿�PIN6/PIN7/PIN8/PIN9.

 -# �������ֹ�������Io: <= 1A
 -# ������ʽ: ˫·����H������
 -# ������ѹ: 7V < Vin < 20V

�Ե�����ƽ����˻�����װ, ʹ�ø��Ӽ�. ͬʱ, ͨ����M1��M2���
ӳ��Ϊ��������, �ṩ�˻��������ֵĸ��߼��Ŀ��ƺ���.

���ڵ�����÷�����:

\code
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
        uint8_t m2 = RoboduinoMotor::instance().motorRead(1);
        
        Serial.print("M1: ");
        Serial.println(m1);
        Serial.print("M2: ");
        Serial.println(m2);
    }
}
\endcode

�����������ӵĿ����÷�:

\code
#include <RoboduinoMotor.h>

void setup()
{
    Serial.begin(9600);
    
    // �����ֺ͵����ӳ���ϵ:
    //
    // 1. ���ֶ�Ӧ���M2, �����תǰ��, ��ת����.
    // 2. ���ֶ�Ӧ���M1, �����ת����, ��תǰ��.
    
    RoboduinoMotor::instance().mapMotor(1, -1, true);
}
void loop()
{
    if(Serial.available() <= 0) return;
    
    int val = Serial.read();
    if(val == 'a')
    {
        // �����ٶ�100
        // �����ٶ�-100
        
        RoboduinoMotor::instance().start(100, -100);
    }
    else if(val == 'b')
    {
        // �����ٶ�-100
        // ����ֹͣ
        
        RoboduinoMotor::instance().start(-100, 0);
    }
    else
    {
        // ȫ��ֹͣ
        
        RoboduinoMotor::instance().stop();
    }
    
    // ����ٶ�
    
    {
        uint8_t left = RoboduinoMotor::instance().leftSpeed();
        uint8_t right = RoboduinoMotor::instance().rightSpeed();
        
        Serial.print("left speed: ");
        Serial.println(left);
        Serial.print("right speed: ");
        Serial.println(right);
    }
}
\endcode

\todo �Ƿ�������Ӷ�ʱֹͣ����? 
\todo ���ٶȱ��������.
*/

class RoboduinoMotor
{
    RoboduinoMotor();
    RoboduinoMotor(const RoboduinoMotor&);
    RoboduinoMotor& operator=(const RoboduinoMotor&);
    
    //================================================================
    //================================================================
    
public:

    /// �������

    enum MotorPin
    {
        M1_Enable   = 6,    ///< M1���ʹ��
        M1_Ctrl     = 7,    ///< M1�����ѹ
        M2_Enable   = 9,    ///< M2���ʹ��
        M2_Ctrl     = 8     ///< M2�����ѹ
    };
    
    /// �ٶȷ�Χ
    
    enum MotorSpeed
    {
        SpeedMax    = 100   ///< �ٶȷ�Χ[-100,100]
    };

    /// ��ȡ�����������
    
    static RoboduinoMotor& instance();
    
    //================================================================
    // �ͼ�����(������ĺ���)
    //================================================================
    
    /**
     *\brief ������ٶ�
     *
     * Ŀǰ���ֻ��ȫ��״̬100, ǰ����Ӧ����, ���˶�Ӧ����.
     * ���Ϊ0��ʾ���ֹͣ.
     *
     *\param idx �������(��0��ʼ)
     */
    uint8_t motorRead(uint8_t idx)const;
    
    /**
     *\brief ���õ���ٶ�
     *
     * Ŀǰ���ֻ��ȫ��״̬100, ǰ����Ӧ����, ���˶�Ӧ����.
     * ���Ϊ0��ʾ���ֹͣ.
     *
     *\param idx �������(��0��ʼ)
     *\param val ����ٶ�, ������ʾ����
     */
    void motorWrite(uint8_t idx, uint8_t val);
    
    //================================================================
    // �߼�����(���ڵͼ�������װ)
    //================================================================
    
    /**
     *\brief ӳ������������
     *
     * �����ӳ�䵽��ʵ��������, �Լ����õ����������֮���ϵ��.
     * ���ϵ��Ϊ����, ���ʾ�����������ӵ���ʵ�����෴.
     *
     * Ĭ�������, M1��Ӧ����, M2��Ӧ����. ���������ӳ���෴,
     * ����ͨ������<c>swapM1M2</c>ʵ��.
     *
     *\param lCoff    ����ӳ�䵽�����ϵ��
     *\param rCoff    ����ӳ�䵽�����ϵ��
     *\param swapM1M2 �����ֵ�ӳ���ϵ.
     *                true��ʾM1��Ӧ����, M2��Ӧ����.
     *                false��ʾM1��Ӧ����, M2��Ӧ����(����Ҫ����).
     */
    
    void mapMotor(uint8_t lCoff, uint8_t rCoff, bool swapM1M2);
    
    /**
     *\brief �������
     *
     * ͨ�������������ٶ�. �ٶ����ո��� \ref RoboduinoMotor#mapMotor
     * ָ���Ĳ���ӳ�䵽����ĵ��.
     *
     * ӳ���ϵ:
     *
     *\code
        if(swapM1M2)
        {
            m1Speed = rSpeed*rCoff;
            m2Speed = lSpeed*lCoff;
        }
        else
        {
            m1Speed = lSpeed*lCoff;
            m2Speed = rSpeed*rCoff;
        }
     *\endcode
     *
     *\param lSpeed �����ٶ�
     *\param rSpeed �����ٶ�
     */
    
    void start(uint8_t lSpeed, uint8_t rSpeed);
    
    /// ֹͣ
    
    void stop();
    
    /**
     *\brief �����ٶ�
     *
     * ���ﷵ�ص���ͨ�� \ref RoboduinoMotor#start �������õ��ٶ�.
     * ����û���ͨ�� \ref RoboduinoMotor#motorWrite ���õ���ٶ�, ��ô
     * Ӧ���� \ref RoboduinoMotor#motorRead ��ѯ. 
     */
    uint8_t leftSpeed()const;
    /**
     *\brief �����ٶ�
     *
     * ���ﷵ�ص���ͨ�� \ref RoboduinoMotor#start �������õ��ٶ�.
     * ����û���ͨ�� \ref RoboduinoMotor#motorWrite ���õ���ٶ�, ��ô
     * Ӧ���� \ref RoboduinoMotor#motorRead ��ѯ. 
     */
    uint8_t rightSpeed()const;
    
    //================================================================
    //================================================================
    
private:

    // ���ùܽ�ģʽ
    
    static void initPinMode();
    
    //================================================================
    //================================================================
	
private:

    bool        m_bMapMotor;    // �Ƿ��޸���ӳ��
    
    //================================================================
    
    bool        m_bSwapM1M2;    // �Ƿ񽻻����
    
    uint8_t     m_u8LCoff;      // ����ϵ��
    uint8_t     m_u8RCoff;      // ����ϵ��
    
    uint8_t     m_u8LSpeed;     // �����ٶ�
    uint8_t     m_u8RSpeed;     // �����ٶ�
    
    //================================================================
    
    uint8_t     m_u8M1Speed;    // M1����ٶ�
    uint8_t     m_u8M2Speed;    // M2����ٶ�
    
    //================================================================
    
    static bool             sm_bPinMode;    // �ܽ�ģʽ
    static RoboduinoMotor   sm_motor;       // ����
    
    //================================================================
    //================================================================
};

#endif	// RoboduinoMotor_H
