#include <Wire.h>
#include <INA3221.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#define TFT_CS       10   // Chip select (CS) line for TFT display
#define TFT_RST      9    // Reset (RST) line for TFT display
#define TFT_DC       8    // Data/command (DC) line for TFT display

#define TFT_WIDTH    240  // Width of the TFT display
#define TFT_HEIGHT   320  // Height of the TFT display

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

#define INA3221_ADDR 0x40 // I2C address of the INA3221
INA3221 ina3221(INA3221_ADDR);

// Include the custom font
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifBoldItalic12pt7b.h>

void setup() {
  Serial.begin(9600);

  tft.init(TFT_WIDTH, TFT_HEIGHT); // Initialize the TFT display
  tft.setRotation(2);               // Set display rotation

  tft.fillScreen(ST77XX_YELLOW);
  tft.setFont(&FreeSerifBold9pt7b);
  tft.setCursor(10, 60);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.print("     FAST");
  tft.setCursor(10, 110);
  tft.print(" CHARGER");
  tft.setCursor(10, 160);
  tft.print("  MADE BY");
  tft.setCursor(10, 210);
  tft.print("    CHAND");
  tft.setCursor(10, 260);
  tft.print("  MOBILES");
  delay(5000); // Display for one second

  tft.fillScreen(ST77XX_WHITE); // Set background color to black

  ina3221.begin(); // Initialize the INA3221

  // Display labels for USB ports with increased text size
  displayUSBLabels();
}

void loop() {
  float voltage[3];
  float current[3];
  float power[3];

  // Read voltage, current, and power for each channel of INA3221
  for (int i = 0; i < 3; i++) {
    voltage[i] = ina3221.getVoltage(i);
    current[i] = ina3221.getCurrent(i) / 1.0; // Convert mA to A
    power[i] = voltage[i] * current[i];
  }

  // Display readings on the screen
  for (int i = 0; i < 3; i++) {
    displayReading(i, voltage[i], current[i], power[i]);
  }

  delay(1000); // Update every 1 second
}

void displayUSBLabels() {
  tft.setTextColor(ST77XX_BLACK);
  
  // Set the custom font size for USB labels
  
  tft.setTextSize(1);

  for (int i = 0; i < 3; i++) {
    tft.setCursor(0, i * 100 + 60); // Set cursor position
    tft.print("  USB : ");
    tft.print(i + 1);
    tft.println("");
  }
}

void displayReading(int port, float voltage, float current, float power) {
  tft.setTextColor(ST77XX_MAGENTA);
  
  // Set the default font size for readings
  tft.setTextSize(1);
  tft.setFont(&FreeSerifBoldItalic12pt7b);

  tft.fillRect(80, port * 100 + 15, TFT_WIDTH - 80, 99, ST77XX_WHITE); // Clear background for readings

  tft.setCursor(100, port * 100 + 40);
  tft.print(voltage, 1);
  tft.println("  V");
  tft.setCursor(100, port * 100 + 60);
  tft.print(current, 2); // Print current with 3 decimal places
  tft.println("  A");
  tft.setCursor(100, port * 100 + 80);
  tft.print(power, 1);   // Print power with 1 decimal place
  tft.println("  W");
}
