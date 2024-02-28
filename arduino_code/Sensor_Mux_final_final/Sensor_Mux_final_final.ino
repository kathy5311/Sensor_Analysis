
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#define SIG_pin A0
//wifi 연결코드
//그리고 추가사항으로 인터넷 연결 끊어지면 다시연결
int s0 = D1, s1 = D2, s2 = D3, s3 = D4;
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

float TGS2600_R0;
float TGS2602_R0;
float TGS2612_R0; 
float TGS2620_R0; 
float TGS2603_R0;  

float TGS2610_R0;
float TGS2611_R0;
float MQ4_R0;
float MQ5_R0;
float MQ6_R0;
float MQ7_R0;
float MQ8_R0;
float MQ9_R0;
float MQ10_R0;
float MQ11_R0;
float MQ12_R0;


unsigned long dt;

String channelNames[]={

  "MQ4",
  "MQ5",
  "MQ6",
  "MQ7",
  "MQ8",
  "TGS2600",
  "TGS2602",
  "TGS2612",
  "MQ9",
  "MQ10",
  "MQ11",
  "TGS2620",
  "TGS2603",
  "TGS2610",
  "TGS2611",
  "MQ12",
  };

void switchSensor(int channel) { 
  
  int controlPin[] = {s0, s1, s2, s3}; 
  int muxChannel[16][4]={ {0,0,0,0},  // channel 0
  {1,0,0,0}, //channel 1
  {0,1,0,0}, //channel 2 
  {1,1,0,0}, //channel 3 
  {0,0,1,0}, //channel 4 
  {1,0,1,0}, //channel 5 
  {0,1,1,0}, //channel 6 
  {1,1,1,0}, //channel 7 
  {0,0,0,1}, //channel 8 
  {1,0,0,1}, //channel 9 
  {0,1,0,1}, //channel 10 
  {1,1,0,1}, //channel 11 
  {0,0,1,1}, //channel 12 
  {1,0,1,1}, //channel 13 
  {0,1,1,1}, //channel 14 
  {1,1,1,1} //channel 15
  
  };

  for(int i = 0; i < 4; i ++){ 
    digitalWrite(controlPin[i], muxChannel[channel][i]); 
    }  
}

int readMux(int channel)  {

  switchSensor(channel);
  int val = analogRead(SIG_pin); //return the value 
  return val; 
  
}

float R0_values[] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

void calibrateSensors() {
  

  MQ4_R0=0;
  MQ5_R0=0;
  MQ6_R0=0;
  MQ7_R0=0;
  MQ8_R0=0;
  TGS2600_R0=0;
  TGS2602_R0=0;
  TGS2612_R0=0;
  MQ9_R0=0;
  MQ10_R0=0;
  MQ11_R0=0;
  TGS2620_R0=0;
  TGS2603_R0=0;
  TGS2610_R0=0;
  TGS2611_R0=0;
  MQ12_R0=0;



  for(int i = 1; i <= 180; i++) {
    MQ4_R0 += readMux(0);
    MQ5_R0 += readMux(1);
    MQ6_R0 += readMux(2);
    MQ7_R0 += readMux(3);
    MQ8_R0 += readMux(4);
    TGS2600_R0 += readMux(5);
    TGS2602_R0 += readMux(6);
    TGS2612_R0 += readMux(7);
    MQ9_R0 += readMux(8);
    MQ10_R0 += readMux(9);
    MQ11_R0 += readMux(10);
    TGS2620_R0 += readMux(11);
    TGS2603_R0 += readMux(12);
    TGS2610_R0 += readMux(13);
    TGS2611_R0 += readMux(14);
    MQ12_R0 += readMux(15);
    delay(1000);
  }


  MQ4_R0/= 180;
  MQ5_R0/= 180;
  MQ6_R0/= 180;
  MQ7_R0/= 180;
  MQ8_R0/= 180;
  TGS2600_R0/= 180;
  TGS2602_R0/= 180;
  TGS2612_R0/= 180;
  MQ9_R0/= 180;
  MQ10_R0/= 180;
  MQ11_R0/= 180;
  TGS2620_R0/= 180;
  TGS2603_R0/= 180;
  TGS2610_R0/= 180;
  TGS2611_R0/= 180;
  MQ12_R0/= 180;



  R0_values[0] = MQ4_R0;
  R0_values[1] = MQ5_R0;
  R0_values[2] = MQ6_R0;
  R0_values[3] = MQ7_R0;
  R0_values[4] = MQ8_R0;
  R0_values[5] = TGS2600_R0;
  R0_values[6] = TGS2602_R0;
  R0_values[7] = TGS2612_R0;
  R0_values[8] = MQ9_R0;
  R0_values[9] = MQ10_R0;
  R0_values[10] = MQ11_R0;
  R0_values[11] = TGS2620_R0;
  R0_values[12] = TGS2603_R0;
  R0_values[13] = TGS2610_R0;
  R0_values[14] = TGS2611_R0;
  R0_values[15] = MQ12_R0;


}

void setup() {
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);

    digitalWrite(s0, LOW);
    digitalWrite(s1, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
    
    Serial.begin(115200);

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
calibrateSensors();

}

void loop() {

  //Serial.println(R0_values[0]);

  if (!conn.connected()) {

    while (conn.connect(server_ip, 3309, user, password) != true) {
      delay(200);
      Serial.print(".");
    }

    Serial.println("MySQL server successfully reconnected");
  }

  if (millis() - dt > 2000) {

    float TGS2600_V = analogRead(SIG_pin);

    float out_values[] = {0,0,0,0,0,0,0,0,0,0,0,0,0}; // Initialize the array

    for (int i=0; i<16; i++){
      switchSensor(i);
      out_values[i] = analogRead(SIG_pin)/ R0_values[i];

      Serial.print("Sensor: ");
      Serial.print(channelNames[i]);
      Serial.print(", Value: ");
      Serial.println(out_values[i]);

      String query = "INSERT INTO test_db1.ehtan (Sensor, Value, stamp) VALUES ('" + channelNames[i] + "','" + String(out_values[i],9) + "',NOW())";
      char INSERT_SQL[query.length() + 1];
      query.toCharArray(INSERT_SQL, sizeof(INSERT_SQL));
      
      int res = cursor.execute(INSERT_SQL);
      if ((res == -1)){
      Serial.println("Error executing MySQL query");
      }

      dt = millis();
    }
  }


}



