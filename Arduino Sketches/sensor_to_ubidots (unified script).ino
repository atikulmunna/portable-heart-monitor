#include <Wire.h>
#include <U8g2lib.h>
#include "MAX30105.h"
#include <Keypad.h>

// Initialize U8g2 in "Page Buffer" mode
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

MAX30105 particleSensor;
bool sensorAvailable = false; 

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[4] = {PA7, PA6, PA5, PA4};   
byte colPins[4] = {PA3, PA2, PB0, PB1}; 
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

enum Mode { MAIN_MENU, ECG_MODE, BPM_MODE, SPO2_MODE };
Mode currentMode  = MAIN_MENU;
Mode previousMode = MAIN_MENU;

#define ECG_PIN PA0                    
uint8_t ecgData[128];   
unsigned long ecgLastUpdate = 0;

unsigned long bpmLastBeatTime = 0;
uint16_t bpmBuffer[5] = {0};
int bpmIndex = 0;
long bpmPreviousIR = 0;
uint32_t bpmSmoothedThresh = 5000;
unsigned long bpmLastUpdate = 0;

#define SPO2_SAMPLE_SIZE 15
uint32_t spo2LastIrValue = 0;
uint32_t spo2LastRedValue = 0;
unsigned long spo2LastDisplayTime = 0;
int spo2Value = 0;
uint16_t spo2IrSamples[SPO2_SAMPLE_SIZE] = {0};
uint16_t spo2RedSamples[SPO2_SAMPLE_SIZE] = {0};
uint8_t spo2SampleIndex = 0;
unsigned long spo2LastSteadyTime = 0;

// --- NEW VARIABLES FOR ESP8266 COMMS ---
unsigned long lastUbidotsUpdate = 0;
int currentBPM = 0; 
// ---------------------------------------

void setup() {
  // Start USART1 to talk to the ESP8266 (MUST be 115200 baud)
  Serial1.begin(115200);

  Wire.begin();
  
  u8g2.begin();
  u8g2.setFont(u8g2_font_5x8_tr); 

  if (particleSensor.begin(Wire, I2C_SPEED_STANDARD)) {
    particleSensor.setup(0x1F, 8, 3, 100, 411, 4096);
    sensorAvailable = true;
  }

  for (int i = 0; i < 128; i++) ecgData[i] = 32;
  showMainMenu();
}

void loop() {
  char key = keypad.getKey(); 
  if (key) {
    if (key == '1') currentMode = ECG_MODE;
    else if (key == '2') currentMode = BPM_MODE;
    else if (key == '3') currentMode = SPO2_MODE;
    else if (key == 'A') currentMode = MAIN_MENU;
  }

  if (currentMode != previousMode) {
    if (currentMode == MAIN_MENU) {
      showMainMenu();
    } else if (currentMode == ECG_MODE) {
      for (int i = 0; i < 128; i++) ecgData[i] = 32;
    } else if (currentMode == BPM_MODE) {
      for (int i = 0; i < 5; i++) bpmBuffer[i] = 0;
      bpmSmoothedThresh = 5000;
      currentBPM = 0; // Reset global BPM on mode change
    } else if (currentMode == SPO2_MODE) {
      for (int i = 0; i < SPO2_SAMPLE_SIZE; i++) { spo2IrSamples[i] = 0; spo2RedSamples[i] = 0; }
      spo2LastSteadyTime = millis();
      spo2Value = 0; // Reset global SpO2 on mode change
    }
    previousMode = currentMode;
  }

  if (currentMode == ECG_MODE) runECGMode();
  else if (currentMode == BPM_MODE) runBPMMode();
  else if (currentMode == SPO2_MODE) runSpO2Mode();

  // --- NEW: ESP8266 Ubidots Transmitter (Runs every 3 seconds) ---
  if (millis() - lastUbidotsUpdate > 3000) {
    lastUbidotsUpdate = millis();

    // Grab the live ECG value (always available regardless of screen mode)
    int liveECG = analogRead(ECG_PIN);

    // Format the JSON exactly how your ESP8266 expects it
    String jsonPayload = "{\"bpm\":";
    jsonPayload += currentBPM;
    jsonPayload += ",\"spo2\":";
    jsonPayload += spo2Value;
    jsonPayload += ",\"ecg\":";
    jsonPayload += liveECG;
    jsonPayload += "}";

    // Shoot it out of PA9 to the ESP8266's RX pin
    Serial1.println(jsonPayload);
  }
}

void showMainMenu() {
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 10); u8g2.print(F("MENU"));
    u8g2.setCursor(0, 25); u8g2.print(F("1: ECG"));
    u8g2.setCursor(0, 40); u8g2.print(F("2: BPM"));
    u8g2.setCursor(0, 55); u8g2.print(F("3: SpO2   A: Back"));
  } while (u8g2.nextPage());
}

void runECGMode() {
  unsigned long now = millis();
  if (now - ecgLastUpdate < 10) return; 
  ecgLastUpdate = now;

  for (int i = 0; i < 127; i++) ecgData[i] = ecgData[i + 1];
  ecgData[127] = 63 - (analogRead(ECG_PIN) >> 6);

  u8g2.firstPage();
  do {
    for (int x = 0; x < 128; x += 32) {
      for (int y = 0; y < 64; y += 8) u8g2.drawPixel(x, y);
    }
    for (int i = 1; i < 128; i++) {
      u8g2.drawLine(i - 1, ecgData[i - 1], i, ecgData[i]);
    }
  } while (u8g2.nextPage());
}

void runBPMMode() {
  if (!sensorAvailable) {
    u8g2.firstPage();
    do { u8g2.setCursor(0, 10); u8g2.print(F("NO SENSOR")); } while(u8g2.nextPage());
    return;
  }
  
  unsigned long now = millis();
  if (now - bpmLastUpdate < 100) return;
  bpmLastUpdate = now;

  long irValue = particleSensor.getIR();
  if (irValue > 50000) {
    bpmSmoothedThresh = ((bpmSmoothedThresh * 9) + irValue) / 10;
    if (irValue > bpmSmoothedThresh && bpmPreviousIR <= (long)bpmSmoothedThresh) {
      unsigned long beat = now - bpmLastBeatTime;
      bpmLastBeatTime = now;
      if (beat > 400 && beat < 1500) {
        bpmBuffer[bpmIndex] = 60000UL / beat;
        bpmIndex = (bpmIndex + 1) % 5;
      }
    }
  } else {
    for (int i = 0; i < 5; i++) bpmBuffer[i] = 0;
  }

  uint32_t sum = 0;
  int ct = 0;
  for (int i = 0; i < 5; i++) {
    if (bpmBuffer[i] > 0) { sum += bpmBuffer[i]; ct++; }
  }
  
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 10); 
    u8g2.print(F("BPM: "));
    if (ct > 0) {
      currentBPM = sum / ct; // Update global variable for ESP8266
      u8g2.print(currentBPM);
    } else {
      currentBPM = 0; // Update global variable for ESP8266
      u8g2.print('-');
    }
    
    int bar = (irValue > 50000) ? ((irValue - 50000) * 128) / 70000 : 0;
    if (bar > 128) bar = 128;
    u8g2.drawBox(0, 40, bar, 10);
  } while (u8g2.nextPage());

  bpmPreviousIR = irValue;
}

void runSpO2Mode() {
  if (!sensorAvailable) {
    u8g2.firstPage();
    do { u8g2.setCursor(0, 10); u8g2.print(F("NO SENSOR")); } while(u8g2.nextPage());
    return;
  }

  uint32_t ir = particleSensor.getIR();
  uint32_t red = particleSensor.getRed();

  spo2IrSamples[spo2SampleIndex] = ir;
  spo2RedSamples[spo2SampleIndex] = red;
  spo2SampleIndex = (spo2SampleIndex + 1) % SPO2_SAMPLE_SIZE;

  uint32_t aIR = 0, aRed = 0;
  for (int i = 0; i < SPO2_SAMPLE_SIZE; i++) { aIR += spo2IrSamples[i]; aRed += spo2RedSamples[i]; }
  aIR /= SPO2_SAMPLE_SIZE; aRed /= SPO2_SAMPLE_SIZE;

  if (aIR < 1000 || aRed < 1000) {
    spo2Value = 0;
    spo2LastSteadyTime = millis();
  } else {
    if (aRed > 0) spo2Value = 102 - (int)((5UL * aIR) / aRed);
    
    uint32_t dIR = (spo2LastIrValue > ir) ? (spo2LastIrValue - ir) : (ir - spo2LastIrValue);
    uint32_t dRed = (spo2LastRedValue > red) ? (spo2LastRedValue - red) : (red - spo2LastRedValue);

    if (dIR < 20 && dRed < 20) {
      if (millis() - spo2LastSteadyTime > 3000) spo2Value = 0; 
    } else {
      spo2LastSteadyTime = millis(); 
    }
  }

  spo2LastIrValue = ir;
  spo2LastRedValue = red;

  if (millis() - spo2LastDisplayTime < 1000) return;
  spo2LastDisplayTime = millis();

  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 10); 
    u8g2.print(F("SpO2: "));
    if (spo2Value > 0) { u8g2.print(spo2Value); u8g2.print('%'); } 
    else u8g2.print(F("--"));

    u8g2.setCursor(0, 50);
    if (spo2Value > 94) u8g2.print(F("OK"));
    else if (spo2Value > 0) u8g2.print(F("LOW"));
    else u8g2.print(F("ERR"));
  } while (u8g2.nextPage());
}