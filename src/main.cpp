#include <Arduino.h>
#include <ArduinoJson.h>
#include <PIRSensor.cpp>
#include <HM3301Sensor.cpp>

#define SERIAL_BAUD_RATE 115200

CSensor *currentSensor=(0);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  delay(10000);
  Serial.println("Start begin");

  /*// create PIRSensor
  StaticJsonDocument<800> myJsonDoc;
  char myJsonStr[] = "{\"name\":\"PIR\",\"driver\": \"PIRSensor\",\"config\": {\"pin\":36},\"interval\": 10,\"enabled\": \"true\"}";
  deserializeJson(myJsonDoc, myJsonStr);
  JsonObject myObject = myJsonDoc.as<JsonObject>();
  currentSensor = new PIRSensor(myObject);
  */

  // create HM3301Sensor
  StaticJsonDocument<800> myJsonDoc;
  char myJsonStr[] = "{\"name\":\"HM3301\",\"driver\": \"HM3301Sensor\",\"config\": {\"pin\":36},\"interval\": 10,\"enabled\": \"true\"}";
  deserializeJson(myJsonDoc, myJsonStr);
  JsonObject myObject = myJsonDoc.as<JsonObject>();
  currentSensor = new HM3301Sensor(myObject);



  Serial.println((String)"mySensor name:"+currentSensor->name+" ,driver:"+currentSensor->driver);
  Serial.println((String)"mySensor description"+currentSensor->description);
  
  
  currentSensor->begin();
}

void loop() {
  //sensor->loop()
  Serial.printf("Getting value for %s\n", currentSensor->name);
  char buffer[256];
  int size = currentSensor->getValuesAsJson(buffer);
  if (size > 0) {
    StaticJsonDocument<200> sensorDoc;
    deserializeJson(sensorDoc, buffer);
    JsonObject sensorObj = sensorDoc.as<JsonObject>();
    serializeJsonPretty(sensorObj, Serial);
    for (JsonPair kvp : sensorObj) {
      int size = sizeof(kvp.key().c_str()) + sizeof(currentSensor->name) + 1;
      char keyname[size];
      sprintf(keyname, "%s-%s", currentSensor->name, kvp.key().c_str());
      Serial.printf("Got JSON val: %s:%f\n", keyname, kvp.value().as<float>());
    }
  } else {
    Serial.print("single Value:");
    float val = currentSensor->getValue();
    Serial.println(val);
    if (val != NAN){

    }else{
      Serial.println("return value is NAN, discard.");
    }
  }
  //Serial.println((String)mySensor->name+" "+mySensor->getValue());
  delay(5000);
}