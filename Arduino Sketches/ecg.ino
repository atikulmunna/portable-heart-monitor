#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ECG_PIN PA0

int ecgData[SCREEN_WIDTH];

// Optional: fake BPM value for display
int bpm = 72;

// Draw a simple grid (like hospital monitors)
void drawGrid() {
  for (int x = 0; x < SCREEN_WIDTH; x += 16) {
    for (int y = 0; y < SCREEN_HEIGHT; y += 4) {
      display.drawPixel(x, y, SSD1306_WHITE);
    }
  }

  for (int y = 0; y < SCREEN_HEIGHT; y += 16) {
    for (int x = 0; x < SCREEN_WIDTH; x += 4) {
      display.drawPixel(x, y, SSD1306_WHITE);
    }
  }
}

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  display.clearDisplay();
  display.display();

  for (int i = 0; i < SCREEN_WIDTH; i++) {
    ecgData[i] = SCREEN_HEIGHT / 2;
  }
}

void loop() {
  // Shift ECG data to the left
  for (int i = 0; i < SCREEN_WIDTH - 1; i++) {
    ecgData[i] = ecgData[i + 1];
  }

  // Read new value
  int ecgValue = analogRead(ECG_PIN);
  int y = map(ecgValue, 0, 4095, SCREEN_HEIGHT - 1, 0);
  ecgData[SCREEN_WIDTH - 1] = y;

  // Clear display buffer
  display.clearDisplay();

  // Draw grid
  drawGrid();

  // Draw ECG waveform
  for (int i = 1; i < SCREEN_WIDTH; i++) {
    display.drawLine(i - 1, ecgData[i - 1], i, ecgData[i], SSD1306_WHITE);
  }

  // Draw labels
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 0);
  display.print("ECG Monitor");

  display.setCursor(90, 0);
  display.print("BPM:");
  display.print(bpm);

  // Push buffer to screen
  display.display();

  // Debug output
  Serial.println(ecgValue);

  delay(10);
}
