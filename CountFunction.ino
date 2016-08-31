//#include "SerialCommand.h"
const int Vmultiple = 2; //分压测量电压和实际电压的倍数比
const int CarVoltageInput = 7;//从A7口输入电压
int Vdebug = 1;
int V_miltime = 0;//电压测量间隔次数,用来减少电压测量程序的执行次数
int checkTime = 0;
int DTime = 10000;




void checkRode()//检测道路状态
{

}
void judgeSpeedup()
{

}
void setNowAsCalibrater()
{

}
void CheckMCUVoltage()//检测供给MCU的电压并并保护MCU
{
  double Vol;
  if (checkTime == 0)
  {
    checkTime = millis();
  }
  if (millis() - checkTime > DTime)
  {
    if ((analogRead(6) * 5 ) / 1024.0 < WarningVoltageMCU ) //插在A6口上面
    {
      Serial.println("--------WARNING :MCU ENERGY LOW---------");
    }
  }
}
void ReportBattery()
{
  double Vol = 0;
  Serial.print("MCU Voltage: ");
  Serial.print((analogRead(6) * 5 ) / 1024.0);
  Serial.println("V");
  Serial.print("MainPower Voltage: ");
  Vol = ( analogRead(7) * 5 ) / 1024.0;
  Serial.print(Vol * 2);
  Serial.println("V");
}



void CheckVoltage()//-------------------------主电源电量检测
{
  double Vol;
  if (V_miltime % 50 == 0) //328p性能为1000000条指令集每秒,可估算大概时间为4ms
  {
    Vol = analogRead(CarVoltageInput) * 5 / 1024;

    if (Vdebug == 1)
    {
      Serial.print("Current MainPower Voltage:");
      Serial.print(Vol * Vmultiple);
      Serial.println("V");
    }
  }
  else
  {
    V_miltime ++;
  }
}
void angleCount(double Xa, double Ya, double Za) //通过读取的数据来计算角度
{
  double Xangle  = 180 * Xa / (0.92 * 3.14 * 16384);
  double Yangle  = 180 * Ya / (0.92 * 3.14 * 16384);
  double Zangle  = 180 * Za / (0.92 * 3.14 * 16384);
  allYangle = Yangle;
  allXangle = Xangle;
  allZangle = Yangle;
  if (DebugFlag_out == 1)
    PrintXYZ(Xangle, Yangle, Zangle);

}


void JudgeCarCurrent(double Xangle, double Yangle, double Zangle) //判断车辆当前状态(上坡下坡),XYZ三轴综合判断
{
  //Yangle 指针变量的地址,*Yangle指针变量的数值

  mil ++;
  double XStateChangeAngle = 3; //另外定义左右加速度之变化幅度范围
  if (Yangle > StateChangeAngle)
  {
    if (Xangle < -XStateChangeAngle)
    {
      if (mil % 10 == 0)
        Serial.println("向左前方加速");
      StateFlag = 0;
    }
    else if (Xangle > XStateChangeAngle)
    {
      if (mil % 10 == 0)
        Serial.println("向右前方加速");
      StateFlag = 0;
    }
    else
    {
      if (mil % 10 == 0)
        Serial.println("向前加速");
      StateFlag = 0;
    }

  }
  else if (Yangle > -StateChangeAngle && Yangle < StateChangeAngle)
  {
    if (mil % 10 == 0)
      Serial.println("平地行进");
    StateFlag = 2;
  }
  else if (Yangle < -StateChangeAngle)
  {
    if (Xangle < -XStateChangeAngle)
    {
      if (mil % 10 == 0)
        Serial.println("向左后方减速");
      StateFlag = 1;
    }
    else if (Xangle > XStateChangeAngle)
    {
      if (mil % 10 == 0)
        Serial.println("向右后方减速");
      StateFlag = 1;
    }
    else
    {
      if (mil % 10 == 0)
        Serial.println("后方减速");
      StateFlag = 1;
    }

  }



}
