#include <ESP8266WiFi.h>
#include <espnow.h>

typedef struct Smessage {
    bool ledStatus;
} Smessage;

Smessage myData;


void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Led: ");
  Serial.println(myData.ledStatus);
  Serial.println();
  digitalWrite(LED_BUILTIN, myData.ledStatus);
}
 
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
