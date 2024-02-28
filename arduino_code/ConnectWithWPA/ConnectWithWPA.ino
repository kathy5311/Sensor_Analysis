
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiClient.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
//MySQL info
IPAddress server_addr(166,104,80,140);
char user[] = "root";
char password[]= "ys3110hs!!";

//sample query
char INSERT_SQL[]="INSERT INTO test_db1.test1 (channel, value) VALUES ('1','123')";

//WIFI card
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiClient client;
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");


}

void loop() {
  // check the network connection once every 10 seconds:
  if (conn.connected()) {
    cursor->execute(INSERT_SQL);
    Serial.println("Done SQL");
  }
  delay(10000);
  
}



