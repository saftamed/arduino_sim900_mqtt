#ifndef tl
#define tl

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#define LINE_BUFFER_LENGTH 256
class Mqtt {
public:
  Mqtt(bool displayMsg);
  void begin(int baudRate = 4800);
  void connect(String server, String server1, String port, String s,
               bool auth = false, String user = "", String pswd = "");
  void publish(String topic, String msg);
  void subscribe(String topic);
  void getData();
  bool available();
  String readString();


private:
  void initAt();
  void rr();
  char line[LINE_BUFFER_LENGTH];
  int lineIndex = 0;
  String serverr;
  String userr ="";
  String pwdd ="";
  String portt ="1883";
  String clientId ="";
  bool withauth = false;
  String topicc="";
  void sendPing();
  bool connected =false;
 
};
#endif
