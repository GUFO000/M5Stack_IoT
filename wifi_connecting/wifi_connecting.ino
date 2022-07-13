/*
  Example from WiFi > WiFiScan
  Complete details at https://RandomNerdTutorials.com/esp32-useful-wi-fi-functions-arduino/
*/

#include "WiFi.h"
#include <M5Atom.h>

 char ssid[] = "Gugliemo_laptop";
 char password[] = "ciaociao";

void initWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to wifi...");

  while(WiFi.status() != WL_CONNECTED){
    Serial.println('.');
    delay(1000);
  }

  Serial.println(WiFi.localIP());
}

void setup() {
  M5.begin(true, false, true);

  //M5.dis.fillpix(colors[2]);
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  initWifi();

  Serial.println("Setup done");

  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());

}

void loop() {

}
