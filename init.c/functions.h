void printWifiData();
void printCurrentNet();
void printMacAddress(byte mac[]);

boolean sendDatatoMySQL(MySQL_Connection conn, char sensor_name[], char measure[], float value, char units[]);
boolean sendDatatoMySQL(MySQL_Connection conn, float temp_value, float hum_value, float press_value);
