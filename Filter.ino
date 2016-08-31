//Copy by internet  http://www.geek-workshop.com/thread-10172-1-1.html
//#include "SerialCommand.h"

////=========================二阶互补滤波===================================
float K2 = 0.2; // 对加速度计取值的权重
float Xx1 = 0, Xx2 = 0, Xy1 = 0;
float dt = 10 * 0.001; //注意：dt的取值为滤波器采样时间,这里表示10毫秒
float XAngleOut = 0;//输出
float SecondFillFilterX(float Angle, float Gravity)
{
  Xx1 = (Angle - XAngleOut) * (1 - K2) * (1 - K2);
  Xy1 = Xy1 + Xx1 * dt;
  Xx2 = Xy1 + 2 * (1 - K2) * (Angle - XAngleOut) + Gravity;
  XAngleOut = XAngleOut + Xx2 * dt;
  // Serial.print("Second:");
  Serial.println(XAngleOut);
  return XAngleOut;
}
float Yx1 = 0, Yx2 = 0, Yy1 = 0;
float YAngleOut = 0;//输出
float SecondFillFilterY(float Angle, float Gravity)
{
  Yx1 = (Angle - YAngleOut) * (1 - K2) * (1 - K2);
  Yy1 = Yy1 + Yx1 * dt;
  Yx2 = Yy1 + 2 * (1 - K2) * (Angle - YAngleOut) + Gravity;
  YAngleOut = YAngleOut + Yx2 * dt;
  // Serial.print("Second:");
  Serial.println(YAngleOut);
  return YAngleOut;
}
float Zx1 = 0, Zx2 = 0, Zy1 = 0;
float ZAngleOut = 0;//输出
float SecondFillFilterZ(float Angle, float Gravity)
{
  Zx1 = (Angle - ZAngleOut) * (1 - K2) * (1 - K2);
  Zy1 = Zy1 + Zx1 * dt;
  Zx2 = Zy1 + 2 * (1 - K2) * (Angle - ZAngleOut) + Gravity;
  ZAngleOut = ZAngleOut + Zx2 * dt;
  // Serial.print("Second:");
  Serial.println(ZAngleOut);
  return ZAngleOut;
}


void SecondFilter(float XAngle, float XGravity, float YAngle, float YGravity, float ZAngle, float ZGravity)
{

  if (FirstCol == 1)  Serial.println("X Y Z");//如果是第一行的话,输出X Y Z
  FirstCol = 0;
  Xx1 = (XAngle - XAngleOut) * (1 - K2) * (1 - K2);
  Yx1 = (YAngle - YAngleOut) * (1 - K2) * (1 - K2);
  Zx1 = (ZAngle - ZAngleOut) * (1 - K2) * (1 - K2);

  Zy1 = Zy1 + Zx1 * dt;
  Yy1 = Yy1 + Yx1 * dt;
  Xy1 = Xy1 + Xx1 * dt;


  Xx2 = Xy1 + 2 * (1 - K2) * (XAngle - XAngleOut) + XGravity;
  Yx2 = Yy1 + 2 * (1 - K2) * (YAngle - YAngleOut) + YGravity;
  Zx2 = Zy1 + 2 * (1 - K2) * (ZAngle - ZAngleOut) + ZGravity;

  XAngleOut = XAngleOut + Xx2 * dt;
  YAngleOut = YAngleOut + Yx2 * dt;
  ZAngleOut = ZAngleOut + Zx2 * dt;
  if ( ReportFilter == 1)
  {
    Serial.print(XAngleOut);
    Serial.print(" ");
    Serial.print(YAngleOut);
    Serial.print(" ");
    Serial.println(ZAngleOut);
  }
  // return XAngleOut;
}
////====================================================================


//二阶互补滤波
//float K2 = 0.2; // 对加速度计取值的权重,其他为重力取值
//float x1 =0, x2 =0, y1 =0;
//float x3, x4, y2;
//float x5, x6, y3;
//float Xangle2 =0, Yangle2, Zangle2; //综合后所得到的最终角度
//
//void SecondStageFilterX(double angle_m, double gyro_m) //采集后计算的角度和角加速度
//{
//
//  x1 = (angle_m - Xangle2) * (1 - K2) * (1 - K2);
//  y1 = y1 + x1 * stage_dt;
//  x2 = y1 + 2 * (1 - K2) * (angle_m - Xangle2) + gyro_m;
//  Xangle2 = Xangle2 + x2 * stage_dt;
//  Serial.print("Xangle2: ");
//  Serial.println(Xangle2);
//
//
//}
//void SecondStageFilterY(double angle_m, double gyro_m) //采集后计算的角度和角加速度
//{
//
//  x3 = (angle_m - Yangle2) * (1 - K2) * (1 - K2);
//  y2 = y2 + x3 * stage_dt;
//  x4 = y2 + 2 * (1 - K2) * (angle_m - Yangle2) + gyro_m;
//  Yangle2 = Yangle2 + x4 * stage_dt;
//
//}
//void SecondStageFilterZ(double angle_m, double gyro_m) //采集后计算的角度和角加速度
//{
//
//  x5 = (angle_m - Zangle2) * (1 - K2) * (1 - K2);
//  y3 = y3 + x5 * stage_dt;
//  x6 = y3 + 2 * (1 - K2) * (angle_m - Zangle2) + gyro_m;
//  Zangle2 = Zangle2 + x6 * stage_dt;
//
//}








