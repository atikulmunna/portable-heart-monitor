#include <Wire.h>
#include "MAX30105.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MAX30105 particleSensor;

uint32_t lastIrValue = 0;
uint32_t lastRedValue = 0;
unsigned long lastTime = 0;

// 🔥 SpO2 strictly integer
int SpO2 = 0;

#define SAMPLE_SIZE 15
uint16_t irSamples[SAMPLE_SIZE] = {0};
uint16_t redSamples[SAMPLE_SIZE] = {0};
uint8_t sampleIndex = 0;

uint16_t irThreshold = 1000;
unsigned long steadyTimeThreshold = 3000;
unsigned long lastSteadyTime = 0;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    display.setCursor(0, 0);
    display.println(F("Sensor Error"));
    display.display();
    while (1);
  }

  particleSensor.setup(0x1F, 8, 3, 100, 411, 4096);
}

void loop() {
  uint32_t irValue = particleSensor.getIR();
  uint32_t redValue = particleSensor.getRed();

  irSamples[sampleIndex] = irValue;
  redSamples[sampleIndex] = redValue;

  sampleIndex++;
  if (sampleIndex >= SAMPLE_SIZE) sampleIndex = 0;

  uint32_t avgIR = 0;
  uint32_t avgRed = 0;

  for (uint8_t i = 0; i < SAMPLE_SIZE; i++) {
    avgIR += irSamples[i];
    avgRed += redSamples[i];
  }

  avgIR /= SAMPLE_SIZE;
  avgRed /= SAMPLE_SIZE;

  if (avgIR < irThreshold || avgRed < irThreshold) {
    SpO2 = 21;
    lastSteadyTime = millis();
  } else {
    // 🔥 Replaced all float math with mathematically identical integer math
    // Old: 100 - 5 * ((avgIR / avgRed) - 0.4)
    // New: 102 - ((5 * avgIR) / avgRed)
    if (avgRed > 0) { // Prevent division by zero
      SpO2 = 102 - ((5 * avgIR) / avgRed);
    }

    // 🔥 Removed abs() and cast to prevent standard library bloat
    uint32_t diffIR = (lastIrValue > irValue) ? (lastIrValue - irValue) : (irValue - lastIrValue);
    uint32_t diffRed = (lastRedValue > redValue) ? (lastRedValue - redValue) : (redValue - lastRedValue);

    if (diffIR < 20 && diffRed < 20) {
      if (millis() - lastSteadyTime > steadyTimeThreshold) {
        SpO2 = 21;
      }
    } else {
      lastSteadyTime = millis();
    }
  }

  // Display update
  if (millis() - lastTime >= 1000) {
    display.clearDisplay();

    display.setCursor(0, 0);
    display.print(F("SpO2"));

    display.setTextSize(2);
    display.setCursor(0, 16);
    display.print(SpO2);   
    display.print('%');

    display.display();

    display.setTextSize(1);
    lastTime = millis();
  }

  lastIrValue = irValue;
  lastRedValue = redValue;
}
