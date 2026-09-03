/*
Ping the servo to check if it is ready.
*/

#include <SCServo.h>

SMS_STS sms_sts;
// the UART used to control servos.
// GPIO 18 - S_RXD, GPIO 19 - S_TXD, as default.

void setup()
{
  Serial.begin(115200);
  Serial2.begin(1000000, SERIAL_8N1);
  sms_sts.pSerial = &Serial2;
  delay(1000);
}

void loop()
{
  for (int id=0; id<16; id++) {
    int ID = sms_sts.Ping(id);
    if(ID!=-1){
      Serial.print("Servo ID:");
      Serial.println(ID, DEC);
      delay(100);
    }else{
      Serial.println("Ping servo ID error!");
      delay(1000);
    }
  }
}