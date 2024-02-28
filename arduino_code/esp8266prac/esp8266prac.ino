#include <ESP8266WiFi.h>           // Use this for WiFi instead of Ethernet.h
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

IPAddress server_addr(166,104,80,140);  // IP of the MySQL *server* here
char user[] = "root";              // MySQL user login username
char password[] = "ys3110hs!!";        // MySQL user login password

// Sample query
char INSERT_SQL[] = "INSERT INTO test_db1.test1 (channel, value) VALUES (1,1234)";

// WiFi card example
char ssid[] = "EPL312";         // your SSID
char pass[] = "eplab312";     // your SSID Password

WiFiClient client;                 // Use this for WiFi instead of EthernetClient
MySQL_Connection conn(&client);
MySQL_Cursor* cursor;

void setup()
{
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect. Needed for Leonardo only

  // Begin WiFi section
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // print out info about the connection:
  Serial.println("\nConnected to network");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());

  Serial.print("Connecting to SQL...  ");
  if (conn.connect(server_addr, 3309, user, password)){
    Serial.println("OK.");
  } else{
    Serial.println("FAILED.");
  }
  
  // create MySQL cursor object
  cursor = new MySQL_Cursor(&conn);
}

void loop()
{
  if (conn.connected())
    cursor->execute(INSERT_SQL);

  delay(5000);
}