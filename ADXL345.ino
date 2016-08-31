const unsigned char ADXL_Address = 0xA7 >> 1; //转换为七位地址
double ADXL_Xa = 0, ADXL_Ya = 0, ADXL_Za = 0;
double ADXL_Xac = 0, ADXL_Yac = 0, ADXL_Zac = 0;
double ADXL_Xac2 = 0, ADXL_Yac2 = 0, ADXL_Zac2 = 0;
double Xuc = 0, Yuc = 0, Zuc = 0; //自行设定的偏移值
double xa, ya, za; //--------经过了处理后的加速度
//accel 除以16384.0得到加速度值

class ZeroThan {
  public:
    double X, Y, Z;
    double ValTemp;//存储用于平均的数值
    int FT = 0;//ZeroFindTime
} ADXL_Zero, MPU_Zero;

//ADXL读数据函数
void ADXL345_ReadData()
{

  Wire.beginTransmission(ADXL_Address);//地址
  Wire.write(0x32);//指定X轴的数据寄存器
  Wire.requestFrom(ADXL_Address , 2 * 3 , true); //输出数据6个字节每两个分别表示XYZ
  Wire.endTransmission(true);//关闭本次传输
  ADXL_Xa = Wire.read() << 8 | Wire.read(); //设置val为数据的存储变量
  ADXL_Ya = Wire.read() << 8 | Wire.read();
  ADXL_Za = Wire.read() << 8 | Wire.read();
  ADXL_Xa -= ADXL_Xac ;
  ADXL_Ya -= ADXL_Yac;
  ADXL_Za -= ADXL_Zac;
  ADXL_Xa -= ADXL_Xac2 ;
  ADXL_Ya -= ADXL_Yac2;
  ADXL_Za -= ADXL_Zac2;

  CountRealAccel();
}

void AcceltoGravity(int Xa, int Ya, int Za)
{
  double Xg = Xa / 256.00;
  double Yg = Ya / 256.00;
  double Zg = Za / 256.00;
}
//ADXL的初始化需要较多设定因此独立出一个写指令函数
void ADXL345_Write_Byte(unsigned char addr, unsigned char data)
{
  Wire.beginTransmission(ADXL_Address);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission(true);
}
void ADXL345_Init()
{
  ADXL345_Write_Byte(0x31, 0x0b);
  ADXL345_Write_Byte(0x2c, 0x08);
  ADXL345_Write_Byte(0x2d, 0x08);
  ADXL345_Write_Byte(0x2e, 0x80);
  ADXL345_Write_Byte(0x1e, 0x00);
  ADXL345_Write_Byte(0x1f, 0x00);
  ADXL345_Write_Byte(0x20, 0x05);
}
void ADXL345_AccelDataCalibration(int t)//初始化偏移校准函数
{
  //t参数为采集多少个数据后进行平均
  int Accel[3];
  for (int i = 0; i < t; i++) //循环方式存储读取的数据
  {
    Wire.beginTransmission(ADXL_Address);//开启传输
    Wire.write(0x32);
    Wire.requestFrom(ADXL_Address, 3 * 2, true); //从0x68设备输出3个轴数据每个2个字节
    Wire.endTransmission(true);//关闭本次传输
    //将数据在这一循环中相加
    ADXL_Xac += Wire.read() << 8 | Wire.read();
    ADXL_Yac += Wire.read() << 8 | Wire.read();
    ADXL_Zac += Wire.read() << 8 | Wire.read();
    delay(1);
  }
  ////平均后得到偏移值
  ADXL_Xac = ADXL_Xac / (t);
  ADXL_Yac = ADXL_Yac / (t);
  ADXL_Zac = ADXL_Zac / (t);
  Serial.print("Xac:");
  Serial.print(ADXL_Xac);
  Serial.print("  Yac:");
  Serial.print(ADXL_Yac);
  Serial.print("  Zac:");
  Serial.println(ADXL_Zac);
}
void ADXL345_AccelDataCalibration2(int t)//第二次偏移校准函数
{
  //t参数为采集多少个数据后进行平均
  int Accel[3];
  for (int i = 0; i < t; i++) //循环方式存储读取的数据
  {
    Wire.beginTransmission(ADXL_Address);//开启传输
    Wire.write(0x32);
    Wire.requestFrom(ADXL_Address, 3 * 2, true); //从0x68设备输出3个轴数据每个2个字节
    Wire.endTransmission(true);//关闭本次传输
    //将数据在这一循环中相加
    ADXL_Xac2 += Wire.read() << 8 | Wire.read();
    ADXL_Yac2 += Wire.read() << 8 | Wire.read();
    ADXL_Zac2 += Wire.read() << 8 | Wire.read();

    ADXL_Xac2 -= ADXL_Xac ;
    ADXL_Yac2 -= ADXL_Yac;
    ADXL_Zac2 -= ADXL_Zac;
    delay(1);
  }
  ////平均后得到偏移值
  ADXL_Xac2 = ADXL_Xac2 / t;
  ADXL_Yac2 = ADXL_Yac2 / t;
  ADXL_Zac2 = ADXL_Zac2 / t;

  Serial.print("Xac:");
  Serial.print(ADXL_Xac);
  Serial.print("  Yac:");
  Serial.print(ADXL_Yac);
  Serial.print("  Zac:");
  Serial.println(ADXL_Zac);
}
void PrintRawData()
{
  Serial.print("Xa:");
  Serial.print(ADXL_Xa);
  Serial.print(" Ya:");
  Serial.print(ADXL_Ya);
  Serial.print(" Za:");
  Serial.println(ADXL_Za);
}
void CountRealAccel()
{
  Xuc = 0.06, Yuc = -0.19, Zuc = 0.10; //设置先前已经测量的校准度数
  xa =  ADXL_Xa /  16384.0 - Xuc;
  ya =  ADXL_Ya /  16384.0 - Yuc;
  za =  ADXL_Za /  16384.0 - Zuc;
  if (ReportA == 1 )
  {
    Serial.print("Xa:");
    Serial.print(xa);
    Serial.print("  Ya:");
    Serial.print(ya);
    Serial.print("  Za:");
    Serial.println(za);
  }
}




