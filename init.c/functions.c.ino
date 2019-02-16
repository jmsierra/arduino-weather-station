// Seria library
#include <SPI.h>

// Wifi library
#include <WiFiNINA.h>

// MySQL libraries
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

// Sensor BME280 library
#include <Adafruit_BME280.h>


// Own libraries
#include "functions.h"

/**
 * It prints out Wifi Data for debugging purposes.
 */
void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

/**
 * It prints out Net info for debugging purpose
 */
void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}

boolean sendDatatoMySQL(MySQL_Connection conn, char measure[], float value, char units[]){
  // Variables:
  char INSERT_SQL[] = "insert into home_sensors.sensor_data (sensor_name, sensor_measure, sensor_unit, sensor_value, datetime) values ('%s', '%s', '%s', %f, now())";
  char query[100]; 
  boolean result;

  // Send the query:
  sprintf(query, INSERT_SQL, SECRET_MYSQL_USER, measure, units, value);
  Serial.println("Executing insert query");
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  result = cur_mem->execute(query);

  // Free up memory
  delete cur_mem;

  if(result == 0){
    Serial.println("Query executed successfully");
  } else {
    Serial.println("Query wasn't executed successfully");
    Serial.println(query);
  }
  
  return result;
}
