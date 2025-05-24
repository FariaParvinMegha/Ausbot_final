#include <Wire.h>
#include <Average.h>
//#include <MPU6050.h>
Average<float> avest(100);
Average<float> avemvc(100);
float avemv[100],AccZg[1000];
const int lum=5;
const int rum=6;
const int in1=13;
const int in2=12;
const int in3=4;
const int in4=2;
const int MPU = 0x68;  // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime, veloX,crtime;
float AccZst,stdst=0,AccZgmv,AccZmv,prtAcc,crtAcc,cgAcc,stdmv,TAccg,TAcc,dn,stdgst,df=0,prtime;
int c = 0;
int i;
int j;
char out=0,a=0;
void setup() {
  Serial.begin(115200);
  Wire.begin();                 // Initialize comunication
  Wire.beginTransmission(MPU);  // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);             // Talk to the register 6B
  Wire.write(0x00);             // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);   //end the transmission

  // Configure Accelerometer Sensitivity - Full Scale Range (default +/- 2g)
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);  //Talk to the ACCEL_CONFIG register (1C hex)
  Wire.write(0x10);  //Set the register bits as 00010000 (+/- 8g full scale range)
  Wire.endTransmission(true);
  // Configure Gyro Sensitivity - Full Scale Range (default +/- 250deg/s)
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);  // Talk to the GYRO_CONFIG register (1B hex)
  Wire.write(0x10);  // Set the register bits as 00010000 (1000deg/s full scale)
  Wire.endTransmission(true);
  delay(20);
  // Call this function if you need to get the IMU error values for your module
  //calculate_IMU_error();
  delay(20);
  pinMode(lum,OUTPUT);
pinMode(rum,OUTPUT);
pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);
pinMode(in3,OUTPUT);
pinMode(in4,OUTPUT);
Serial1.begin(9600);
}
void loop() {
  prtime=crtime;
   crtime=millis();
   elapsedTime = (crtime - prtime) / 1000;
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);  // Read 6 registers total, each axis value is stored in 2 registers
  AccZg[1000] = (Wire.read() << 8 | Wire.read()) / 16384.0;  // Z-axis value
   avest.push(AccZg[1000]);//bucket 
   stdgst=avest.stddev();
   Serial.print("static standard dev: ");
   Serial.print(stdgst);
   prtAcc=stdst;
   stdst=stdgst*2*9.8;
   TAcc=stdst-prtAcc;
   Serial.print("\n stdst:");
   Serial.println(stdst);
   Serial.println(TAcc);
   
   
   dn=0.5*TAcc*elapsedTime*elapsedTime*100;
   df=df+dn ;
   Serial.print("\n distance:");
   Serial.println(df);
delay(700);
   if (df>=0&&TAcc==0&&df<6){
     analogWrite(lum,250);
    analogWrite(rum,250);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in1,HIGH);
    digitalWrite(in4,HIGH); 
   }
   if( df>6&&TAcc>0&&df<8){
    analogWrite(lum,180);
    analogWrite(rum,180);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in1,HIGH);
    digitalWrite(in4,HIGH); 
    
   }
   if (df>8&&TAcc>0&&df<10){
   analogWrite(lum,250);
    analogWrite(rum,250);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in1,LOW);
    digitalWrite(in4,LOW);
    if(Serial1.available() > 0)  
  {
    a = Serial1.read();         
  Serial.print(a);
  if(a=='1')
  {
     analogWrite(lum,250);
    analogWrite(rum,250);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);
    digitalWrite(in1,LOW);
    digitalWrite(in4,LOW); 
    delay(10);
  }
  else {
    analogWrite(lum,250);
    analogWrite(rum,250);
    digitalWrite(in2,LOW);
    digitalWrite(in3,LOW);
    digitalWrite(in1,LOW);
    digitalWrite(in4,LOW); 
    return;  
  } }
  else{
    return;
  }
}
  if (df>10&&TAcc>0){
    if(Serial1.available() > 0)  
  {
    a = Serial1.read();         
  Serial.print(a);
  if(a=='1') 
  {
     analogWrite(lum,250);
    analogWrite(rum,250);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);
    digitalWrite(in1,LOW);
    digitalWrite(in4,LOW); 
    delay(10);
  }
  else {
    analogWrite(lum,250);
    analogWrite(rum,250);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);
    digitalWrite(in1,LOW);
    digitalWrite(in4,LOW); 
    delay(10);    
  }
  }
  else
  {
  return;    
  } }
  if (df>10&&TAcc<0){
    if(Serial1.available() > 0)  
  {
    a = Serial1.read();         
  Serial.print(a);
  if(a=='1') 
  {
     analogWrite(lum,250);
    analogWrite(rum,250);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);
    digitalWrite(in1,LOW);
    digitalWrite(in4,LOW); 
    delay(10);
  }
  else {
    analogWrite(lum,250);
    analogWrite(rum,250);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);
    digitalWrite(in1,LOW);
    digitalWrite(in4,LOW); 
    delay(10);    
  }
  }
  else
  {
  //return;
  analogWrite(lum,250);
    analogWrite(rum,250);
    digitalWrite(in2,HIGH);
    digitalWrite(in3,HIGH);
    digitalWrite(in1,LOW);
    digitalWrite(in4,LOW); 
    delay(10);     
  } }
  } 
  
  
