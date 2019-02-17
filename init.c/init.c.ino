// Serial library
#include <SPI.h>

// Wifi Library
#include <WiFiNINA.h>

// BME280
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; //I2C

// MySQL libraries
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

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
char * mysql_user = SECRET_MYSQL_USER;
char * mysql_pass = SECRET_MYSQL_PASS;
char * mysql_dbp  = SECRET_MYSQL_DB;
WiFiClient client;
MySQL_Connection conn((Client *)&client);

// Sensor name
char sensor_name[] = "home_weather_station";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

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
  // weather monitoring
  Serial.println("-- Weather Station Scenario --");
  Serial.println("forced mode, 1x temperature / 1x humidity / 1x pressure oversampling,");
  Serial.println("filter off");
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF   );
  Serial.println("BME initiated");

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  float temp;
  float pres;
  float hum;
  String res;
  String msg;

  // Act every 30s
  delay(30000);

  // Turn builtin led on while it's working.
  digitalWrite(LED_BUILTIN, HIGH);

  // Check if MySQL connection is alive and connect if not
  if (!conn.connected()) {
    // Try to connect
    Serial.println("MySQL connection is dead");
    msg = "Connecting to MySQL...";
    msg = msg + mysql_user + "/...@" + mysql_host[0] + "." + mysql_host[1] + "." + mysql_host[2] + "." + mysql_host[3] + ":" + mysql_port;
    Serial.println(msg);

    if (!conn.connect(mysql_host, mysql_port, mysql_user, mysql_pass)) {
      Serial.println("MySQL connection failed");
      return;
    }
    Serial.println("MySQL connection established");
  }

  // Read from sensor
  bme.takeForcedMeasurement(); 
  temp = bme.readTemperature();
  pres = bme.readPressure() / 100.0F; //hPa
  hum = bme.readHumidity();
  res = "Temperature: ";
  res = res + temp + "; Pressure: " + pres + "; Humdity: " + hum;
  Serial.println(res);

  // Write to MySQL
  //sendDatatoMySQL(conn, "Temperature", temp, "C");
  //sendDatatoMySQL(conn, "Humidity", hum, "%");
  //sendDatatoMySQL(conn, "Pressure", pres, "hPa");
  sendDatatoMySQL(conn, temp, hum, pres);

  printCurrentNet();

  // Check battery:
  // https://www.hackster.io/Arduino_Genuino/mkr-zero-read-battery-voltage-4853ac
  // read the input on analog pin 0:
  int sensorValue = analogRead(ADC_BATTERY);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 4.3V):
  float voltage = sensorValue * (4.3 / 1023.0);
  // print out the value you read:
  Serial.print(voltage);
  Serial.println("V");

  // Switch off LED
  digitalWrite(LED_BUILTIN, LOW);
}
