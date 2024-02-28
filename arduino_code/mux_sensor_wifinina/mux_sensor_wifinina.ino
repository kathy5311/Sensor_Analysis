#include <SPI.h>
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#define WIFI_SSID "EPL312"
#define WIFI_PASSWORD "eplab312"

IPAddress server_addr(166, 104, 80, 140);
const int server_port = 3309;

char db_user[] = "root";
char db_password[] = "ys3110hs!!";

WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor cursor(&conn);

float TGS2600_R0;
float TGS2603_R0;
float TGS2620_1_R0; 
float TGS2620_2_R0; 

int s0 =D3, s1 =D4, s2 =D5, s3 =D6;
#define SIG_pin A0

float R0_values[13];

unsigned long dt;

String channelNames[] = {
  "TGS2600", "TGS2603", "TGS2620-1", "TGS2620-2"
};

void switchSensor(int channel) {
  int controlPin[] = {s0, s1, s2, s3};
  int muxChannel[16][4] = {
    {0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0}, {1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0},
    {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1}, {0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}
  };

  for (int i = 0; i < 4; i++) {
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
}

int readMux(int channel) {
  switchSensor(channel);
  int val = analogRead(SIG_pin);
  return val;
}

void calibrateSensors() {
  for (int i = 0; i < 13; i++) {
    R0_values[i] = 0;
  }

  for (int j = 1; j <= 180; j++) {
    for (int i = 0; i < 4; i++) {
      R0_values[i] += readMux(i);
    }
    delay(1000);
  }

  for (int i = 0; i < 4; i++) {
    R0_values[i] /= 180;
  }
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
  connectToWiFi();

  // Connect to MySQL server
  connectToMySQL();

  // Calibration
  calibrateSensors();
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
}

void connectToMySQL() {
  Serial.print("Connecting to MySQL Server...");
  while (conn.connect(server_addr, server_port, db_user, db_password) != true) {
    Serial.print(".");
    delay(200);
  }
  Serial.println("MySQL server successfully connected");
}

void loop() {
  if (!conn.connected()) {
    connectToMySQL();
  }

  if (millis() - dt > 2000) {
    float out_values[4];
    for (int i = 0; i < 4; i++) {
      switchSensor(i);
      out_values[i] = analogRead(SIG_pin) / R0_values[i];
      Serial.print("Sensor: ");
      Serial.print(channelNames[i]);
      Serial.print(", Value: ");
      Serial.println(out_values[i]);

      String query = "INSERT INTO test_db1.test2 (channel, value ) VALUES ('" + channelNames[i] + "','" + String(out_values[i], 9) + "')";
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

