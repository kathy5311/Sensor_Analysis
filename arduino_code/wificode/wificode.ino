#include <ESP8266WiFi.h>        // WIFI 라이브러리

const char* ssid     = "EPL313";         // 접속할 WIFI SSID
const char* password = "eplab313";     // 접속할 WIFI 비밀번호

void setup() {
  Serial.begin(115200);

  delay(1000);
  Serial.println('\n');

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.print('.');

  }

  Serial.println('\n');
  Serial.println("Connected!");
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}
void loop() { }

