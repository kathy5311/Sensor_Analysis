//10 channel
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

long randNumber; // Choose Channel randomly
#define VOC_DURATION 5 //2sec
#define WASH_DURATION 10 //5min

//wifi
const char* ssid = "EPL312"; //wifi 이름 수정해주세요
const char* wifi_password = "eplab312"; //비밀번호 수정해주세요

// Define the MySQL server parameters
IPAddress server_ip(166, 104, 80, 140);  // Change this to your MySQL server's IP address
char user[] = "root";        // MySQL username
char password[] = "ys3110hs!!";    // MySQL password
char db[] = "test_db1";          // MySQL database name

WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor cursor(&conn);

unsigned long dt;

void setup() {
  pinMode(D5,OUTPUT); //PUMP
  pinMode(D4,OUTPUT); //FAN


  //randomSeed (analogRead(10));
  // String status;
  Serial.begin(115200);
  digitalWrite(D5, LOW); //pump멈춤
  digitalWrite(D4, LOW); //fan멈춤

  // Connect to WiFi
  WiFi.begin(ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to MySQL server
  if (conn.connect(server_ip, 3309, user, password, db)) {
      Serial.println("Connected to MySQL server");
  } else {
      Serial.println("Connection failed");
      while (true);
  }
}

void loop(){
  digitalWrite(D5, LOW); //pump멈춤
  digitalWrite(D4, LOW); //fan멈춤

  if (!conn.connected()) {

    while (conn.connect(server_ip, 3309, user, password) != true) {
      delay(200);
      Serial.print(".");
    }

    Serial.println("MySQL server successfully reconnected");
  }
  
  for (int k=2; k <3; k++){
    randNumber = k;
    valve(randNumber);
    delay(5000); 
  }
}

void valve(int val){
  String status;

  if (val==2){
    String query = "INSERT INTO test_db1.ethan2 (injection) VALUES (NOW())";
    char INSERT_SQL[query.length() + 1];
    query.toCharArray(INSERT_SQL, sizeof(INSERT_SQL));
      
    int res = cursor.execute(INSERT_SQL);
    if ((res == -1)){
      Serial.println("Error executing MySQL query");
    }  

    digitalWrite(D5, HIGH); //pump가동
    digitalWrite(D4, LOW); //팬멈춤
    status = "{\"VOC\":"+String('1')+"}";
    // Serial.println(val);
    Serial.println(status);
    for (int i = 1; i < VOC_DURATION; i++) {
      delay(1000);
      }
    digitalWrite(D5, LOW); //pump멈춤
    digitalWrite(D4, LOW); //fan멈춤
    status = "{\"VOC\":"+String('2')+"}";
    // Serial.println(val);
    Serial.println(status);
    for (int i = 1; i < WASH_DURATION; i++) {
      delay(1000);
      }

    //digitalWrite(2, LOW); //pump멈춤
    //digitalWrite(3, HIGH); //fan가동
    //status = "{\"VOC\":"+String('3')+"}";
    // Serial.println(val);
    //Serial.println(status);
    //for (int i = 1; i < WASH_DURATION; i++) {
      //delay(1000);
      //}
  } 

  else{
   Serial.println("else");
  }
}