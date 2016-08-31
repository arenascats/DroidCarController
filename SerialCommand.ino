#include <avr/pgmspace.h>
  String Command = {""};
 int inputdate = 0;
 int mark = 0;

void SerialCommand()
  {
    while (Serial.available() > 0)
    {
      Command += char(Serial.read());//读入单次收到的所有指令
      delay(2);
    mark = 1;
  }
    
    if(mark == 1) 
    {
      //Serial.println(Command);//----------调试语句回传
    if (Command.length() > 0)
    {
   /////  -----------------------用于数字的判断--------------------
      for (int i = 1; i < Command.length() ; i++)
      {
        if( i ==1 && Command[0] == 'b')
        inputdate += (Command[i] - '0');
        else
        {
          inputdate = inputdate*10 + (Command[i]-'0');
        }
      }
        if(Command[0] == 'B')
        {
        WarningVoltage = inputdate;//存储用于显示的波频率
        Serial.print("Warning Voltage:");
        Serial.println(WarningVoltage);
        }
        else if(Command[0] == 's')
        {
          Serial.print("Set Servo:");
        Serial.println(inputdate);
        inset = inputdate * 10;
        }
        else if(Command[0] == 'm')
        {
          Serial.print("Set Motor:");
        Serial.println(inputdate);
        insetM = inputdate * 10;
        }
        
      
/////---------------------------------------------------------------
      switch (Command[0])//对单个字母进行判断
      {
        case 'I':
          Serial.println("System Run after 5s ");
          delay(3000);
          SystemRun =1;
          break;
          
        case 'O':
          Serial.println("System Shutdown after 5s ");
          delay(3000);
          SystemRun =0;
          break;
          case 'b'://显示当前电量
          ReportBattery();
          break;
          case 'B'://显示当前电量
          CheckVoltage();
          break;
          
          case 't'://显示当前温度
          Serial.print("Current Temperature:");
          Serial.print(Templete);
          break;
          
          case 'r'://进行复位
          StopShut();
          break;
          
          case 's'://设置SERVO
          
          break;
          
          case 'F'://Filter内容输出输出启动
          if(FirstCol == 0 && ReportFilter == 1)//如果首行打印标志位为0说明目前正在通过串口输出中
          {
            FirstCol = 1;
            ReportFilter = 0;//停止串口汇报数据,置位为0
            }
          else if(ReportFilter == 0)  ReportFilter = 1;
          break;
          
          case 'A'://输出/关闭输出角度数据
          if(ReportA == 0)
            ReportA =1;
            else if(ReportA == 1)
            ReportA =0;
          break;
          
          case 'a'://输出/关闭输出加速度数据
          if(Reporta == 0)
            Reporta =1;
            else if(Reporta == 1)
            Reporta =0;
          break;
          
          case 'g'://输出/关闭输出重力数据
          if(Reportg == 0)
            Reportg =1;
            else if(Reportg == 1)
            Reportg =0;
          break;
          
          case 'T'://当前道路情况汇报
          break;
          
          case'm':
          CommandHelp();
          break;
          
          default:
          break;

      }
      Command = String("");//Clear Command
      inputdate = 0;
      }
    }
    mark = 0;
  }


 void CommandHelp()
 {
//  Serial.println("============Command==============");
//  Serial.println("Command   What use");
//  Serial.println(" m       Use for display command help ");
//  Serial.println(" g       Display gravity data");
//  Serial.println(" a       Display angle change data");
//  Serial.println(" I       System Start after 5s");
//  Serial.println(" O       System Stop after 5s");
//  Serial.println(" r       Reset servo and motor ");
//  Serial.println(" s<num>  Set servo ");
//  Serial.println(" m<num>  Set motor,such as m155   ");
//  Serial.println(" t       Display temperature ");
//  Serial.println(" B       Check whether infered to warning voltage");
//  Serial.println(" b       Display Battery infomation");
 }
  void CurrentSetting()
  {
    Serial.println("======ManlessControlSystem=======");
  
    
  }

