#include <SPI.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include "functions.h"

char wifi_ssid[]  = SECRET_WIFI_SSID;
char wifi_pass[]  = SECRET_WIFI_PASS;
int status        = WL_IDLE_STATUS;     // the Wifi radio's status

void setup() {  
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Serial connection established");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(wifi_ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(wifi_ssid, wifi_pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();
}

void loop() {
  // put your main code here, to run repeatedly:
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();
}
