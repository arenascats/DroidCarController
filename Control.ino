

void setServo()
{

}

void setServo(int i)//from 500 - 2500
{
  // if( i >1850)  i = 1850;
  //if(i < 1150) i = 1150;
  digitalWrite(ServoOut, LOW);
  digitalWrite(ServoOut , HIGH);
  delayMicroseconds(i);
  digitalWrite(ServoOut, LOW);
  // delay(18);//低电平时间
  delayMicroseconds(2000 - i);

  //  while( millis()- j <= 20000 - i)
  //  {
  //
  //  }

}

void setMotor(int i)//from 500 - 2500
{
  // if( i >1850)  i = 1850;
  //if(i < 1150) i = 1150;
  // if(!i) i = 1600;
  digitalWrite(MotorOut, LOW);
  digitalWrite(MotorOut , HIGH);
  delayMicroseconds(i);
  digitalWrite(MotorOut, LOW);
  //delay(18);//低电平时间
  delayMicroseconds(2000 - i);
}


void setMotorAndServo(int servoTime, int motorTime)
{
  digitalWrite(ServoOut, LOW);
  digitalWrite(MotorOut, LOW);
  digitalWrite(MotorOut , HIGH);
  digitalWrite(ServoOut , HIGH);

  if (servoTime > motorTime)
  {
    delayMicroseconds(motorTime);
    digitalWrite(MotorOut, LOW);
    delayMicroseconds(servoTime - motorTime);
    digitalWrite(ServoOut, LOW);
    delayMicroseconds(2000 - servoTime);
  }
  else if (servoTime < motorTime)
  {
    delayMicroseconds(servoTime);
    digitalWrite(ServoOut, LOW);
    delayMicroseconds( motorTime - servoTime);
    digitalWrite(MotorOut, LOW);
    delayMicroseconds(2000 - motorTime);
  }
  else
  {

  }

}
void StopShut()
{
  insetM = 1550;
  inset = 1480;
  Serial.println("Stop Running");
}
void TrigOne()
{
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  int Readpulse = pulseIn(Echo, HIGH, 5000); //如果20ms都没有回波的话停止
  double CM = Readpulse / 58.0;
  if (CM < 30 && CM > 25)
  {
    StopShut();
    Serial.println("Trigger now,Stop Running");
  }
}

