#include <Wire.h>
#include "MAX30105.h"
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include <math.h>

// OLED setup
SSD1306AsciiWire oled;
MAX30105 particleSensor;

// BPM calculation
unsigned long lastBeatTime = 0;
float beatsPerMinute = 0;
float bpmBuffer[5] = {0};
int bpmIndex = 0;
long previousIR = 0;
float smoothedThreshold = 5000;
bool heartbeatDetected = false;
unsigned long beatFlashTime = 0;

// Graph parameters
const int barMaxWidth = 20; // Max width of IR bar graph
const int barX = 0;
const int barY = 56;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Initializing..."));

  // OLED Init
  Wire.begin();
  oled.begin(&Adafruit128x64, 0x3C);
  oled.setFont(System5x7);
  oled.clear();
  oled.println(F("Starting..."));

  // MAX30105 Init
  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    Serial.println(F("MAX30105 not found"));
    oled.println(F("Sensor not found!"));
    while (1);
  }

  oled.println(F("Sensor ready"));
  Serial.println(F("MAX30105 ready"));

  // Sensor configuration
  particleSensor.setup(0x1F, 8, 2, 100, 411, 4096);
}

void loop() {
  long irValue = particleSensor.getIR();
  unsigned long currentTime = millis();
  heartbeatDetected = false;

  // Finger detection
  if (irValue > 50000) {
    smoothedThreshold = (0.9 * smoothedThreshold) + (0.1 * irValue);

    if (irValue > smoothedThreshold && previousIR <= smoothedThreshold) {
      unsigned long timeSinceLastBeat = currentTime - lastBeatTime;
      lastBeatTime = currentTime;

      if (timeSinceLastBeat > 400 && timeSinceLastBeat < 1500) {
        beatsPerMinute = 60000.0 / timeSinceLastBeat;
        bpmBuffer[bpmIndex] = beatsPerMinute;
        bpmIndex = (bpmIndex + 1) % 5;
        heartbeatDetected = true;
        beatFlashTime = currentTime;
      }
    }
  } else {
    for (int i = 0; i < 5; i++) bpmBuffer[i] = 0;
  }

  // BPM average
  float bpmSum = 0;
  int validCount = 0;
  for (int i = 0; i < 5; i++) {
    if (bpmBuffer[i] > 0) {
      bpmSum += bpmBuffer[i];
      validCount++;
    }
  }
  float averagedBPM = (validCount > 0) ? (bpmSum / validCount) : 0;
  averagedBPM = floor(averagedBPM);

  // === OLED DISPLAY ===
  oled.clear();

  // Blinking heart: only show <3 for 300ms after beat
  oled.setCursor(40, 0);
  if (currentTime - beatFlashTime < 300) {
    oled.println(F("<3 HEART"));
  } else {
    oled.println(F("   HEART"));
  }

  // BPM line
  oled.setCursor(10, 20);
  oled.print(F("BPM: "));
  if (averagedBPM > 0) {
    oled.println((int)averagedBPM);
  } else {
    oled.println(F("--"));
  }

  // IR value line
  oled.setCursor(0, 40);
  oled.print(F("IR: "));
  oled.println(irValue);

  // IR bar graph (scaled to barMaxWidth)
  int barWidth = map(irValue, 50000, 120000, 0, barMaxWidth);
  barWidth = constrain(barWidth, 0, barMaxWidth);

  oled.setCursor(barX, barY);
  oled.print("[");
  for (int i = 0; i < barMaxWidth; i++) {
    if (i < barWidth) oled.print(char(219)); // Full block
    else oled.print(" ");
  }
  oled.print("]");

  // === Serial Output ===
  Serial.print(F("IR: "));
  Serial.print(irValue);
  Serial.print(F(" | BPM: "));
  Serial.println(averagedBPM);

  previousIR = irValue;
  delay(100);
}
