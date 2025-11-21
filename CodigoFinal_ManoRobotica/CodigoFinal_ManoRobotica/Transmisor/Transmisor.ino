#include <WiFi.h>
#include <esp_now.h>

int POT_PINS[5] = {39, 34, 35, 32, 33}; // solo ADC1, válidos con Wi-Fi

struct Payload {
  uint16_t ang[5];
};

//MAC del receptor
uint8_t receiverMAC[6] = {0x78, 0x1C, 0x3C, 0xA5, 0xE1, 0xFC};
Payload tx;

void onSend(uint8_t *mac_addr, esp_now_send_status_t status) {}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db); // rango aprox. 0-3.3 V

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    while (true) delay(1000);
  }

  esp_now_register_send_cb(onSend);
  esp_now_peer_info_t peer{};
  memcpy(peer.peer_addr, receiverMAC, 6);
  peer.channel = 0;
  peer.encrypt = false;
  esp_now_add_peer(&peer);
}

void loop() {
  for (int i = 0; i < 5; i++) {
    int lectura = analogRead(POT_PINS[i]);
    int ang = map(lectura, 0, 4095, 0, 180);
    if (ang < 0) ang = 0;
    if (ang > 180) ang = 180;
    tx.ang[i] = ang;
  }

  esp_now_send(receiverMAC, (uint8_t*)&tx, sizeof(tx));
  delay(20); // ~50 Hz
}

