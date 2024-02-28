
int s0 = D3, s1 = D4, s2 = D5, s3 = D6;

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>


#ifndef WIFI
#define WIFI "EPL312"
#define PW  "eplab312"
#endif
#define SIG_pin A0

const char* ssid     = WIFI;
const char* password = PW;
//const char* server = "api.pushingbox.com";

IPAddress server_addr = IPAddress(166,104,80,140);
const int server_port = 3309;

const uint16_t port = 3309;

ESP8266WiFiMulti WiFiMulti;

char db_user[] = "root";
char db_password[] = "ys3110hs!!";

WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor cursor(&conn);


float TGS2600_R0;
float TGS2602_R0;
float TGS2603_R0; 
float TGS2610_R0; 
float TGS2620_R0;  

float MQ2_R0;
float MQ3_R0;
float MQ4_R0;
float MQ5_R0;
float MQ6_R0;
float MQ7_R0;
float MQ8_R0;
float MQ9_R0;




unsigned long dt;

String channelNames[]={

  "TGS2600",
  "TGS2602",
  "TGS2603",
  "TGS2610",
  "TGS2620",
  "MQ2",
  "MQ3",
  "MQ4",
  "MQ5",
  "MQ6",
  "MQ7",
  "MQ8",
  "MQ9",

};

void switchSensor(int channel) { 
  
  int controlPin[] = {s0, s1, s2, s3}; 
  int muxChannel[16][4]={ {0,0,0,0},  // channel 1
  {1,0,0,0}, //channel 2 
  {0,1,0,0}, //channel 3 
  {1,1,0,0}, //channel 4 
  {0,0,1,0}, //channel 5 
  {1,0,1,0}, //channel 6 
  {0,1,1,0}, //channel 7 
  {1,1,1,0}, //channel 8 
  {0,0,0,1}, //channel 9 
  {1,0,0,1}, //channel 10 
  {0,1,0,1}, //channel 11 
  {1,1,0,1}, //channel 12 
  {0,0,1,1}, //channel 13 
  {1,0,1,1}, //channel 14 
  {0,1,1,1}, //channel 15 
  {1,1,1,1} //channel 16 
  
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

float R0_values[] = {0,0,0};

void calibrateSensors() {
  
  TGS2600_R0 = 0;
  TGS2602_R0 = 0;
  TGS2603_R0 = 0;
  TGS2610_R0 = 0;
  TGS2620_R0 = 0;
  MQ2_R0 = 0;
  MQ3_R0 = 0;
  MQ4_R0 = 0;
  MQ5_R0 = 0;
  MQ6_R0 = 0;
  MQ7_R0 = 0;
  MQ8_R0 = 0;
  MQ9_R0 = 0;


  for(int i = 1; i <= 180; i++) {
    TGS2600_R0 += readMux(0);
    TGS2602_R0 += readMux(1);
    TGS2603_R0 += readMux(2);
    TGS2610_R0 += readMux(3);
    TGS2620_R0 += readMux(4);
    MQ2_R0 += readMux(5);
    MQ3_R0 += readMux(6);
    MQ4_R0 += readMux(7);
    MQ5_R0 += readMux(8);
    MQ6_R0 += readMux(9);
    MQ7_R0 += readMux(10);
    MQ8_R0 += readMux(11);
    MQ9_R0 += readMux(12);
    delay(1000);
  }

  TGS2600_R0 /= 180;
  TGS2602_R0 /= 180;
  TGS2603_R0 /= 180;
  TGS2610_R0 /= 180;
  TGS2620_R0 /= 180;
  MQ2_R0 /= 180;
  MQ3_R0 /= 180;
  MQ4_R0 /= 180;
  MQ5_R0 /= 180;
  MQ6_R0 /= 180;
  MQ7_R0 /= 180;
  MQ8_R0 /= 180;
  MQ9_R0 /= 180;


  R0_values[0] = TGS2600_R0;
  R0_values[1] = TGS2602_R0;
  R0_values[2] = TGS2603_R0;
  R0_values[3] = TGS2610_R0;
  R0_values[4] = TGS2620_R0;
  R0_values[5] = MQ2_R0;
  R0_values[6] = MQ3_R0;
  R0_values[7] = MQ4_R0;
  R0_values[8] = MQ5_R0;
  R0_values[9] = MQ6_R0;
  R0_values[10] = MQ7_R0;
  R0_values[11] = MQ8_R0;
  R0_values[12] = MQ9_R0;

}

void setup(){
  
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  Serial.begin(115200);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);


  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("WiFi successfully connected");

  delay(500);

  // 1시간 20분 예열 - 실험의 1사이클에 따라 계속 바꿔주기
  for (int i=0; i<4800; i++){
    delay(1000);
  }

calibrateSensors();

}

//#define DeviceID "v6627CDD4EB29D17"

void loop() {

  Serial.println(R0_values[0]);

  if (!conn.connected()) {

    while (conn.connect(server_addr, server_port, db_user, db_password) != true) {
      delay(200);
      Serial.print(".");
    }

    Serial.println("MySQL server successfully reconnected");
  }

  if (millis() - dt > 2000) {

    float TGS2600_V = analogRead(SIG_pin);

    float out_values[] = {0,0,0,0,0,0,0,0,0,0,0,0,0}; // Initialize the array

    for (int i=0; i<13; i++){
      switchSensor(i);
      out_values[i] = analogRead(SIG_pin)/ R0_values[i];

      Serial.print("Sensor: ");
      Serial.print(channelNames[i]);
      Serial.print(", Value: ");
      Serial.println(out_values[i]);

      String query = "INSERT INTO test_db1.Sensor2 (channel, value ) VALUES ('" + channelNames[i] + "','" + String(out_values[i],9) + "')";
      char INSERT_SQL[query.length() + 1];
      query.toCharArray(INSERT_SQL, sizeof(INSERT_SQL));
      
      int res = cursor.execute(INSERT_SQL);
      if (res == -1) {
      Serial.println("Error executing MySQL query");
      }

      dt = millis();
    }
  }


}
