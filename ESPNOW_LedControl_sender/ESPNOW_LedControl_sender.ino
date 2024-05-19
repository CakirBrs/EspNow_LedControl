#include <ESP8266WiFi.h>
#include <espnow.h>
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; //Replace with receiver's MAC address.

typedef struct Smessage {
  bool ledStatus;
} Smessage;

Smessage myData;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void sendBlinker(bool ledS){
    myData.ledStatus = ledS;
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}
 
void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);  
  pinMode(D8, INPUT); //Button connected pin
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}

int button_state;
int prev_button_state = 1;

void loop() {
  button_state =digitalRead(D8);
  if (prev_button_state == HIGH && button_state == LOW)
  {
    Serial.println("The button is released");
    sendBlinker(0);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (prev_button_state == LOW && button_state == HIGH)
  {
    Serial.println("The button is pressed");
    sendBlinker(1);
    digitalWrite(LED_BUILTIN, LOW);
  }
  prev_button_state = button_state;
  delay(100);
}
