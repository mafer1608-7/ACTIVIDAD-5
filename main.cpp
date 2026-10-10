#include <Arduino.h>

const int POT1_PIN = 4;
const int POT2_PIN = 5;

void setup() {
    Serial0.begin(115200);
    delay(2000);

    Serial0.println("ESP32 INICIADO");
    Serial0.println("Lectura de potenciometros:");
}

void loop() {
    int pot1 = analogRead(POT1_PIN);
    int pot2 = analogRead(POT2_PIN);

    Serial0.print(pot1);
    Serial0.print(",");
    Serial0.println(pot2);

    delay(50);
}