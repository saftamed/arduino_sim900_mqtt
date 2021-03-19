#include "mqtt.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>


SoftwareSerial mySerial(10,11);
Mqtt::Mqtt(bool displayMsg) {

}

void Mqtt::begin(int baudRate) {
  mySerial.begin(baudRate);
  Serial.begin(baudRate);
  //initAt();
}
void Mqtt::connect(String server,String server1,String port,String s,bool auth=false,String user="",String pswd="") {
   mySerial.println("AT+CIPSHUT\r");
  delay(2000);
  byte co[] = {0x00, 0x04, 0x4d, 0x51, 0x54,
               0x54, 0x04, 0xc2, 0x00, 0x3c, 0x00};
  byte pwd[] = {0x00};

  int length = s.length()+user.length()+pswd.length()+16;
  if(!auth){
    co[7] = 0x02;
    length -4;
  }
  mySerial.print("AT+CIPSTART=\"TCP\",\"");
  delay(1000);

  mySerial.print(server);
  delay(1000);
  rr();
    mySerial.print(server1);
  delay(1000);
  rr();
   mySerial.print("\",\"");
  delay(1000);
 rr();
  mySerial.print(port);
  delay(1000);
 rr();
  mySerial.println("\"\r");

  delay(5000);
 rr();

  mySerial.print("AT+CIPSEND\r");
  delay(3000);

 rr();
  mySerial.write(0x10);
  mySerial.write(length);
  mySerial.write(co, 11);

  mySerial.write(s.length());
  mySerial.print(s);
 
  if(auth){
    mySerial.write(pwd, 1);
    mySerial.write(user.length());
    mySerial.print(user);
    mySerial.write(pwd, 1);  

    mySerial.write(pswd.length());
    mySerial.print(pswd);
  }


  delay(1000);
 rr();
  // mySerial.write(0x1a);
  mySerial.println("\r");
  delay(1000);
 rr();
}
void Mqtt::publish(String topic,  String msg) {
  int length = 5 + topic.length() + msg.length();
  byte pu[] = {0x00};
  mySerial.print("AT+CIPSEND");
  // mySerial.print(length);
  mySerial.print("\r");
  delay(3000);
   rr();
  mySerial.write(0x31);
  mySerial.write(length - 3);
  mySerial.write(pu, 1);
  mySerial.write(topic.length());
  mySerial.print(topic);
  mySerial.print(msg);
  delay(1000);
   rr();
  mySerial.write(0x1a);
  delay(3000);
   rr();
}
void Mqtt::subscribe(String topic) {
  int length = 7 + topic.length();
  byte su[] = {0x9b, 0x9c, 0x00};
  byte s[] = {0x00};
  mySerial.print("AT+CIPSEND=");
  mySerial.print(length);
  mySerial.print("\r");
  delay(1000);
   rr();
  mySerial.write(0x82);
  mySerial.write(length - 2);
  mySerial.write(su, 3);
  mySerial.write(topic.length());
  mySerial.print(topic);
  mySerial.write(s, 1);
  mySerial.write(0X1a);
  delay(3000);
   rr();
}
void Mqtt::initAt() {
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r");
  delay(2000);
   rr();
  mySerial.println("AT+CIPSHUT\r");
  delay(2000);
   rr();
  mySerial.println("AT+CIPSTATUS\r");
  delay(2000);
   rr();
  mySerial.println("AT+CIPMUX=0\r");
  delay(2000);
   rr();
  mySerial.println("AT+CSTT=\"internet.ooredoo.tn\"\r");
  delay(2000);
   rr();
  mySerial.println("AT+CIICR\r");
  delay(5000);
   rr();
  mySerial.println("AT+CIFSR\r");
  delay(5000);
   rr();
  mySerial.println("AT+CIPSPRT=0\r");
  delay(2000);
}
void Mqtt::rr() {
  while (mySerial.available() >= 1) {
    Serial.write(mySerial.read());
  }
}
void Mqtt::getData(){
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
bool Mqtt::available(){
     while ( Serial.available()>0 ) {
     char  c = Serial.read();
    // Serial.print(c);
      if (( c == '\n') || ( c == '\r')) {    
          return true;         
          //checkResponse();
        }else if ( c == '}'){
          line[ lineIndex++ ] = c;      
          return true;                                   
          //checkResponse(true);

        } else {
          if ( lineIndex >= LINE_BUFFER_LENGTH-1 ) {
            Serial.println("overflow");
             lineIndex = 0;
          } 
          else if ( c >= 'a' && c <= 'z' ) {        
            line[ lineIndex++ ] = c-'a'+'A';
          } 
          else {
            line[ lineIndex++ ] = c;
          }
        return false; 
      }
    }
    return false; 
}



String Mqtt::readString(){
    if ( lineIndex > 0 ) {                        
        line[ lineIndex ] = '\0';                   
    }
    lineIndex = 0;
    String s(line);
    //Serial.println(s);
    int topicInde = s.indexOf("IOT") ;
    if(topicInde>0){
      int msgIndex = s.indexOf("{") ;
      // Serial.print("Topic :");
      // Serial.println(s.substring(topicInde,topicInde+22));
      // Serial.print("Msg :");
      // Serial.println(s.substring(msgIndex));
      // DynamicJsonDocument doc(1024);
      // deserializeJson(doc, s.substring(msgIndex));
      // Serial.print("Safta :");
      // Serial.println((char*)doc["SAFTA"]);
      return s.substring(msgIndex);
    }else{
      if(s.indexOf("CLOSED")>=0 || s.indexOf("ERROR")>=0){
          //reconnect

          return "ERROR";
      }
    }
  return s;
}













