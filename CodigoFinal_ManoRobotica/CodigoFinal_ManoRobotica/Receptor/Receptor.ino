#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>

// Pines para los servos
const int SERVO_PINS[5] = {23, 22, 19, 18, 17};
Servo servos[5];

// Rango de pulsos típico
const int minUs = 500;
const int maxUs = 2400;

// Estructura recibida
struct Payload {
  uint16_t ang[5];
};

volatile Payload rx;
volatile bool hasNew = false;

// Callback al recibir datos
void onRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  if (len == sizeof(Payload)) {
    memcpy((void*)&rx, incomingData, sizeof(Payload));
    hasNew = true;
  }
}

void setup() {
  Serial.begin(115200);

  // Inicializar servos
  for (int i = 0; i < 5; i++) {
    servos[i].setPeriodHertz(50);
    servos[i].attach(SERVO_PINS[i], minUs, maxUs);
    servos[i].write(90); // posición neutra
  }

  // Configurar Wi-Fi y ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    while (true) delay(1000);
  }
  esp_now_register_recv_cb(onRecv);

  Serial.println("Receptor listo ⚙️");
}

void loop() {
  if (hasNew) {
    noInterrupts();
    Payload local;
    memcpy(&local, (const void*)&rx, sizeof(Payload));
    hasNew = false;
    interrupts();

    for (int i = 0; i < 5; i++) {
      uint16_t a = local.ang[i];
      if (a > 180) a = 180;
      servos[i].write(a);
    }
  }
}
