int Templete;//温度
//gx/131.0=x轴向角速度  单位 度/秒
void MPU6050_Init()
{
  Wire.beginTransmission(0x68);//6050的地址
  Wire.write(0x6B);//设置休眠寄存器
  Wire.write(0);//设置为0,不休眠
  Wire.endTransmission(true);//结束向6050通讯
}
void MPU6050_ReadData()
{
  Wire.beginTransmission(0x68);//开启传输
  Wire.write(0x41);//指定X轴的数据寄存器
  Wire.requestFrom(0x68, 2, true); //从0x68设备输出数据2个字节也就是十六位
  Wire.endTransmission(true);//关闭本次传输
  int val = Wire.read() << 8 | Wire.read(); //设置val为数据的存储变量
  Serial.print("Temp:");
  double temp = (double) val / 340 + 34.53;
  Serial.println(temp);
}

void MPU6050_ReadAccelData()
{

  int Accel[3];
  double Xa = 0, Ya = 0, Za = 0;
  Wire.beginTransmission(0x68);//开启传输
  Wire.write(0x3B);
  Wire.requestFrom(0x68, 3 * 2, true); //读取从0x68设备输出3个轴数据每个2个字节
  Wire.endTransmission(true);//关闭本次传输
  for (long i = 0; i < 3; i++) {
    Accel[i] = Wire.read() << 8 | Wire.read(); //将数据存入数组,分别为XYZ轴的数据
  }
  Xa = Accel[0] - Xac;//误差值的校准
  Ya = Accel[1] - Yac;
  Za = Accel[2] - Zac;
  double Xangle  = 180 * Xa / (0.92 * 3.14 * 16384); //转化为角度
  double Yangle  = 180 * Ya / (0.92 * 3.14 * 16384);
  double Zangle  = 180 * Za / (0.92 * 3.14 * 16384);
  Serial.print("Xa:");
  Serial.print(Xangle);
  Serial.print("  Ya:");
  Serial.print(Yangle);
  Serial.print("  Za:");
  Serial.println(Zangle);

}

void MPU6050_ReadGyroData()//读取陀螺仪信息并输出为角速度
{

  int Gyro[3];
  double Xg = 0, Yg = 0, Zg = 0;
  Wire.beginTransmission(0x68);//开启传输
  Wire.write(0x43);
  Wire.requestFrom(0x68, 3 * 2, true); //读取从0x68设备输出3个轴数据每个2个字节
  Wire.endTransmission(true);//关闭本次传输
  for (long i = 0; i < 3; i++) {
    Gyro[i] = Wire.read() << 8 | Wire.read(); //将数据存入数组,分别为XYZ轴的数据
  }
  Xg = Gyro[0] - Xgc;//误差值的校准
  Yg = Gyro[1] - Ygc;
  Zg = Gyro[2] - Zgc;

  Xg = Xg / 131.0;
  Yg = Yg / 131.0;
  Zg = Zg / 131.0;
  Serial.print("Xg:");
  Serial.print(Xg);
  Serial.print("  Yg:");
  Serial.print(Yg);
  Serial.print("  Zg:");
  Serial.println(Zg);
  delay(5);

}
void MPU6050_AccelDataCalibration(int t)
{
  //t参数为采集多少个数据后进行平均
  int Accel[3];
  for (int i = 0; i < t; i++) //循环方式存储读取的数据
  {
    Wire.beginTransmission(0x68);//开启传输
    Wire.write(0x3B);
    Wire.requestFrom(0x68, 3 * 2, true); //从0x68设备输出3个轴数据每个2个字节
    Wire.endTransmission(true);//关闭本次传输
    //将数据在这一循环中相加
    Xac += Wire.read() << 8 | Wire.read();
    Yac += Wire.read() << 8 | Wire.read();
    Zac += Wire.read() << 8 | Wire.read();
    delay(5);
  }
  ////平均后得到偏移值
  Xac = Xac / t;
  Yac = Yac / t;
  Zac = Zac / t;
  Serial.print("Xac:");
  Serial.print(Xac);
  Serial.print("  Yac:");
  Serial.print(Yac);
  Serial.print("  Zac:");
  Serial.println(Zac);

}

int CalibrationFlag = 1;//用来记录是否校准过

int RecorderNum = 0 , RecorderNumMax = 100;
void MPU6050_AccelAndGyro()//------------------------------------读取Accel和Gyro
{
  Xac = 2.60, Yac = -1.2, Zac = 54.92, Xgc = 0.07, Ygc = -0.03, Zgc = -0.01;//offset

  int Accel[3];
  int Gyro[3];


  Wire.beginTransmission(0x68);//开启传输
  Wire.write(0x3B);
  Wire.requestFrom(0x68, 7 * 2, true); //从0x68设备输出3个轴数据每个2个字节
  Wire.endTransmission(true);//关闭本次传输
  //将数据在这一循环中相加
  Xa = Wire.read() << 8 | Wire.read();
  Ya = Wire.read() << 8 | Wire.read();
  Za = Wire.read() << 8 | Wire.read();
  Templete = ( Wire.read() << 4 | Wire.read() ) / 340 + 34.53; //温度,暂时用不到
  Xg = Wire.read() << 8 | Wire.read();
  Yg = Wire.read() << 8 | Wire.read();
  Zg = Wire.read() << 8 | Wire.read();

  if (CalibrationFlag == 1)
  {
    Xg = Xg / 131.0;
    Yg = Yg / 131.0;
    Zg = Zg / 131.0;
    Xangle  = 180 * Xa / ( 3.14 * 16384); //转化为角度
    Yangle  = 180 * Ya / ( 3.14 * 16384);
    Zangle  = 180 * Za / ( 3.14 * 16384);

  }
  //  if (CalibrationFlag == 0)//校准如果结束
  //  {

  Xg = ( Xg / 131.0 ) - Xgc;
  Yg = ( Yg / 131.0 ) - Ygc;
  Zg = ( Zg / 131.0 ) - Zgc;
  Xangle  = (180 * Xa / ( 3.14 * 16384) ) - Xac; //转化为角度
  Yangle  = (180 * Ya / ( 3.14 * 16384) ) - Yac;
  Zangle  = (180 * Za / ( 3.14 * 16384) ) - Zac;

  //-------------------用于DEBUG的串口输出语句
  if ( Reporta == 1)
  {
    Serial.print("Xangle:");
    Serial.print(Xangle);
    Serial.print("  Yangle:");
    Serial.print(Yangle);
    Serial.print("  Zangle:");
    Serial.println(Zangle);
  }
  if ( Reportg == 1)
  {
    Serial.print("Xg:");
    Serial.print(Xg);
    Serial.print("  Yg:");
    Serial.print(Yg);
    Serial.print("  Zg:");
    Serial.println(Zg);
  }


  //  }
  //-------------- 一种校准函数
  //  if (CalibrationFlag == 1)//校准函数部分
  //  {
  //    if (RecorderNum < RecorderNumMax)
  //    { Xac =Xac + Xa;
  //      Yac =Yac + Ya;
  //      Zac =Zac + Za;
  //      Xgc =Xgc + Xg;
  //      Ygc =Ygc + Yg;
  //      Zgc =Zgc + Zg;
  //      RecorderNum++ ;
  //    }
  //    if (RecorderNum == RecorderNumMax-1)//如果等于设定的最大校准循环次数-1那么说明校准结束
  //    {
  //      Xac = Xac / RecorderNumMax;
  //      Yac =Yac / RecorderNumMax;
  //      Zac =Zac / RecorderNumMax;
  //      Xgc =Xgc / RecorderNumMax;
  //      Ygc =Ygc / RecorderNumMax;
  //      Zgc =Zgc / RecorderNumMax;
  //      CalibrationFlag = 0;
  //    }
  //  }

}
void MPU6050_GyroDataCalibration(int t)
{
  //t参数为采集多少个数据后进行平均
  int Gyro[3];
  for (int i = 0; i < t; i++) //循环方式存储读取的数据
  {
    Wire.beginTransmission(0x68);//开启传输
    Wire.write(0x43);
    Wire.requestFrom(0x68, 3 * 2, true); //从0x68设备输出3个轴数据每个2个字节
    Wire.endTransmission(true);//关闭本次传输
    //将数据在这一循环中相加
    Xgc += Wire.read() << 8 | Wire.read();
    Ygc += Wire.read() << 8 | Wire.read();
    Zgc += Wire.read() << 8 | Wire.read();
    delay(1);
  }
  ////平均后得到偏移值
  Xgc = Xgc / t;
  Ygc = Ygc / t;
  Zgc = Zgc / t;

  Serial.print("Xgc:");
  Serial.print(Xgc);
  Serial.print("  Ygc:");
  Serial.print(Ygc);
  Serial.print("  Zgc:");
  Serial.println(Zgc);

}


