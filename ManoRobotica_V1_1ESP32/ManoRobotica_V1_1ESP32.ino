#include <ESP32Servo.h>


int SERVOS[5] = {23, 22, 19, 18, 17};
int POTES[5]   = {34, 35, 32, 33, 15};

Servo servos[5];


int minUs = 500;
int maxUs = 2400;

void setup() {
  for (int i = 0; i < 5; i++) {
    servos[i].setPeriodHertz(50);
    servos[i].attach(SERVOS[i], minUs, maxUs);
  }
}

void loop() {
  for (int i = 0; i < 5; i++) {
    int lectura = analogRead(POTES[i]);
    int angulo  = map(lectura, 0, 4095, 0, 180);
    if (angulo < 0)  { 
      angulo = 0; //limitar angulo minimo
    }
    if (angulo > 180) {
      angulo = 180; //limitar angulo maximo
    }
    servos[i].write(angulo);
  }
  delay(10);
}