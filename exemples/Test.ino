#include "mqtt.h"

Mqtt mqtt(false);

void setup() {
  mqtt.begin(4800);
  // mqtt.connect(a,"ernetofthings.ibmcloud.com","1883","d:3mda17:c:sim",true,"use-token-auth","g+fjh-hUsMx8lqw!2i");
  //  delay(6000);
  //  mqtt.publish("iot-2/evt/send/fmt/json", "{\"d\":{\"led\":5}}");
  //  delay(4000);
  //  mqtt.subscribe("iot-2/cmd/led/fmt/json");
}

void loop() {
  if(mqtt.available()){
    String line = mqtt.readString();
    Serial.println(line);
  }
}
