// Serial library
#include <SPI.h>

// Wifi Library
#include <WiFiNINA.h>

// BME280
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; //I2C

// Own libraries
#include "secrets.h"
#include "functions.h"

// Wifi related variables
char wifi_ssid[]  = SECRET_WIFI_SSID;
char wifi_pass[]  = SECRET_WIFI_PASS;
int status        = WL_IDLE_STATUS;     // the Wifi radio's status

// MySQL related variables
IPAddress mysql_host(192, 168, 148, 106);
int mysql_port = SECRET_MYSQL_PORT;
char mysql_user[] = SECRET_MYSQL_USER;
char mysql_pass[] = SECRET_MYSQL_PASS;
char mysql_dbp[]  = SECRET_MYSQL_DB;

// Sensor name
char sensor_name[] = "home_weather_station";

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

  Serial.println("Trying to initialize the bme...");
  if (!bme.begin()) {  
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  Serial.println("BME initiated");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  float temp;
  int pres;
  float hum;
  String res;
  
  // Act every 10s
  delay(10000);
  
  // Turn builtin led on while it's working.
  digitalWrite(LED_BUILTIN, HIGH);

  // Read from sensor
  temp = bme.readTemperature();
  pres = bme.readPressure();
  hum = bme.readHumidity();
  res = "Temperature: ";
  res = res + temp + "; Pressure: " + pres + "; Humdity: " + hum;
  Serial.println(res);

  // Write to MySQL
  
  printCurrentNet();

  digitalWrite(LED_BUILTIN, LOW);
}
