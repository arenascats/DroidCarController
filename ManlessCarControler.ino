//Version 2.2 2016.05.26
//I2c    MPU6050 & ADXL345
//D2     MPU6050's INT
//A6     MCUsupportPower
//A7     MainPower

#include "SerialCommand.h"
#include <Wire.h>
const int DebugFlag_out = 0;//是否输出XYZ数据
const int StateChangeAngle = 5;
double Xa, Ya, Za, Xg, Yg, Zg ; //从陀螺仪寄存器所读取到的生数据
double Xac = 0, Yac = 0, Zac = 0; //初始化误差偏移值
double Xgc = 0, Ygc = 0, Zgc = 0; //初始化误差偏移值
double Xangle = 0, Yangle = 0, Zangle = 0;
double Xgp = 0, Ygp = 0, Zgp = 0;
double allYangle, allXangle, allZangle = 0;
int mil = 0;//发送串口数据的间隔
int StateFlag, oldStateFlag; //车辆当前/上一次所处于的基础状态(0上坡 1下坡 2平地)

//超声波接口
const int Trig = 11;
const int Echo = 12;



//舵机控制接口
const int ServoIn = 3;
const int ServoOut = 4;
int inset = 1450;

//电机控制接口
const int MotorIn = 5;
const int MotorOut = 6;
int insetM = 1550;


void ADXL345_Init();
void ADXL345_ReadData();

void PrintXYZ(double x, double y, double z) //用于输出XYZ当前的状态
{
  Serial.print("X:");
  Serial.print(x);
  Serial.print(" Y:");
  Serial.print(y);
  Serial.print(" Z:");
  Serial.println(z);
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);

  //------------------------------------接口初始化
  pinMode(ServoOut, OUTPUT);
  pinMode(ServoIn , INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(MotorOut, OUTPUT);
  pinMode(MotorIn, INPUT);


  MPU6050_Init();
  ADXL345_Init();
  delay(1);
  //MPU6050_GyroDataCalibration(200);//平均二百次
  delay(1);
  //MPU6050_AccelDataCalibration(200);//平均二百次
  // ADXL345_AccelDataCalibration(200);//一次初始化滤波
  //ADXL345_AccelDataCalibration2(200);//二次滤波
}

//-------------------时间驱动系统函数
int lastTime = 200, newTime;//lastTime是一个最后记录时间,newTime是一个新时间
//int newSTime, newMTime ;
void UpdataTime(int runTime)//数据更新时间 (ms)
{

  setServo(inset);
  const long  newSTime = millis();//记录时间
  setMotor(insetM);
  const long newMTime = millis();
  //setMotorAndServo(inset, insetM);//设置servo和电机的PWM波
  //const unsigned long  newSTime = millis();
  for (;;)//这里面的是在PWM空情况下执行的程序
  {
    if ( 18 + newSTime < millis() || 18 + newMTime < millis())  break; //如果时间到程序退出该循环返回对电机和舵机操作的部分
    newTime = millis();
    SerialCommand();//串口指令

    if (newTime - lastTime >= runTime)
    {
      ///////////////运行程序写在这里面/////////
      //TrigOne();
      MPU6050_AccelAndGyro();
      ADXL345_ReadData();
      // XYZAngleCount();
      ////////////////////////////////////////
      lastTime = newTime;
    }
    SecondFilter(Xangle, Xg, Yangle, Yg, Zangle, Zg);
  }

}

void loop() {
  //MPU6050_ReadAccelData();//读取当前角度讯息并输出
  //JudgeCarCurrent(allXangle,allYangle,allZangle);//车辆目前状态判断语句
  // MPU6050_ReadGyroData();//读取当前角速度讯息并输出
  //ADXL345_ReadData();
  //MPU6050_AccelAndGyro();
  //XYZAngleCount();

  UpdataTime(10);
  //setServo(1300);
  // setMotor(1600);

}
