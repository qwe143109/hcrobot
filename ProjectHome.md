Encoder Connection Diagram

http://www.dfrobot.com/wiki/images/thumb/8/8e/Encoder_Connection.PNG/444px-Encoder_Connection.PNG


Encoder Sampling Code
```


const byte encoder0pinA = 2;//A pin -> the interrupt pin 0
const byte encoder0pinB = 4;//B pin -> the digital pin 4
byte encoder0PinALast;
int duration;//the number of the pulses
boolean Direction;//the rotation direction 


void setup()
{  
  Serial.begin(57600);//Initialize the serial port
  EncoderInit();//Initialize the module
}

void loop()
{
  Serial.print("Pulse:");
  Serial.println(duration);
  duration = 0;
  delay(500);
}

void EncoderInit()
{
  Direction = true;//default -> Forward  
  pinMode(encoder0pinB,INPUT);  
  attachInterrupt(0, wheelSpeed, CHANGE);
}

void wheelSpeed()
{
  int Lstate = digitalRead(encoder0pinA);
  if((encoder0PinALast == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoder0pinB);
    if(val == LOW && Direction)
    {
      Direction = false; //Reverse
    }
    else if(val == HIGH && !Direction)
    {
      Direction = true;  //Forward
    }
  }
  encoder0PinALast = Lstate;

  if(!Direction)  duration++;
  else  duration--;
}

```



Open source home care robot from roboticfan.com


欢迎大家看一下chai2010撰写的关于SVN的使用教程。非常浅显易懂！

http://code.google.com/p/hcrobot/wiki/svn_tutorial

几段HCR的视频：

HCR Introduction：

<a href='http://www.youtube.com/watch?feature=player_embedded&v=MV_9lHqNs4I' target='_blank'><img src='http://img.youtube.com/vi/MV_9lHqNs4I/0.jpg' width='425' height=344 /></a>



HCR First Demo：

<a href='http://www.youtube.com/watch?feature=player_embedded&v=LIBpRbcQY5I' target='_blank'><img src='http://img.youtube.com/vi/LIBpRbcQY5I/0.jpg' width='425' height=344 /></a>
