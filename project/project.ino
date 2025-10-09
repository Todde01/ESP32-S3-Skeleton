#include <Arduino.h>
#include <esp_task_wdt.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc_cal.h"
#include <SPI.h>
#include "pin_config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <time.h>
#include <String.h>

int pageIndex = 0;

const uint8_t logo_bitmap[512] = {
    0x00,
    0x00,
    0x1F,
    0xC0,
    0x00,
    0x18,
    0x00,
    0x00,
    0x00,
    0x00,
    0x3F,
    0xE0,
    0x01,
    0xFC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x7F,
    0xF0,
    0x07,
    0xFC,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xF8,
    0x0F,
    0xFE,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xF8,
    0x1F,
    0xFC,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFC,
    0xFF,
    0xF0,
    0x00,
    0x00,
    0x00,
    0x01,
    0xFF,
    0xFD,
    0xFF,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xF9,
    0x9C,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xF8,
    0x1E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x7F,
    0xF8,
    0x0E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x7F,
    0xF0,
    0x0E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1F,
    0xE0,
    0x0E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0F,
    0x00,
    0x0E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x07,
    0x00,
    0x0E,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x07,
    0x80,
    0x0F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x07,
    0x80,
    0x0F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x07,
    0x80,
    0x0F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x07,
    0x80,
    0x7F,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x07,
    0x83,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0x9F,
    0xFC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0xFF,
    0xF0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0xFF,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0xFC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x71,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0xF9,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0xF9,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x0F,
    0xE1,
    0xF0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1F,
    0xC0,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x3F,
    0x00,
    0xFF,
    0xFF,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x7E,
    0x00,
    0xFF,
    0xFF,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x7C,
    0x00,
    0xFF,
    0xFF,
    0xE0,
    0x00,
    0x00,
    0x00,
    0xF8,
    0x00,
    0x00,
    0x01,
    0xF0,
    0x00,
    0x00,
    0x00,
    0xF0,
    0x00,
    0x00,
    0x00,
    0xF0,
    0x00,
    0x00,
    0x01,
    0xF0,
    0x00,
    0x00,
    0x00,
    0xF8,
    0x00,
    0x00,
    0x01,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x78,
    0x00,
    0x00,
    0x03,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x7C,
    0x00,
    0x00,
    0x03,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x3C,
    0x00,
    0x00,
    0x03,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x3E,
    0x00,
    0x00,
    0x03,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x1E,
    0x00,
    0x00,
    0x03,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x1F,
    0x00,
    0x00,
    0x03,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x0F,
    0x00,
    0x00,
    0x03,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x0F,
    0x80,
    0x00,
    0x03,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x07,
    0x80,
    0x00,
    0x03,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x07,
    0xC0,
    0x00,
    0x03,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x03,
    0xC0,
    0x00,
    0x01,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x03,
    0xE0,
    0x00,
    0x01,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x01,
    0xE1,
    0x00,
    0x01,
    0xF0,
    0x00,
    0x00,
    0x00,
    0x01,
    0xFF,
    0x80,
    0x00,
    0xF8,
    0x00,
    0x00,
    0x00,
    0x08,
    0xFF,
    0x80,
    0x00,
    0x7C,
    0x00,
    0x00,
    0x00,
    0x1E,
    0xFF,
    0x00,
    0x00,
    0x7E,
    0x00,
    0x00,
    0x00,
    0x3E,
    0x7C,
    0x00,
    0x00,
    0x3F,
    0x00,
    0x00,
    0x00,
    0xFC,
    0x70,
    0x00,
    0x00,
    0x1F,
    0x80,
    0x00,
    0x01,
    0xF8,
    0x00,
    0x00,
    0x00,
    0x0F,
    0xE0,
    0x00,
    0x07,
    0xF0,
    0x00,
    0x00,
    0x00,
    0x07,
    0xF8,
    0x00,
    0x1F,
    0xE0,
    0x00,
    0x00,
    0x00,
    0x01,
    0xFF,
    0xC3,
    0xFF,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x1F,
    0xFF,
    0xF8,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x03,
    0xFF,
    0xC0,
    0x00,
    0x00,
    0x00,
};

// Remember to remove these before commiting in GitHub
String ssid = "ssid";
String password = "password";

// "tft" is the graphics libary, which has functions to draw on the screen
TFT_eSPI tft = TFT_eSPI();

// Display dimentions
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 170

WiFiClient wifi_client;

/**
 * Setup function
 * This function is called once when the program starts to initialize the program
 * and set up the hardware.
 * Carefull when modifying this function.
 */
void setup()
{
  // Initialize Serial for debugging
  Serial.begin(115200);
  // Wait for the Serial port to be ready
  while (!Serial)
    ;
  Serial.println("Starting ESP32 program...");
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  pinMode(PIN_BUTTON_1, INPUT_PULLUP);
  pinMode(PIN_BUTTON_2, INPUT_PULLUP);

  // // Connect to WIFI
  // WiFi.begin(ssid, password);

  // // Will be stuck here until a proper wifi is configured
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   tft.fillScreen(TFT_BLACK);
  //   tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //   tft.drawString("Connecting to WiFi...", 10, 10);
  //   Serial.println("Attempting to connect to WiFi...");
  // }

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Connected to WiFi", 10, 10);
  Serial.println("Connected to WiFi");
  // Add your code bellow
}

/**
 * This is the main loop function that runs continuously after setup.
 * Add your code here to perform tasks repeatedly.
 */
void loop()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);

  switch (pageIndex)
  {
  case 0:
    tft.drawString("Group 6, v 1.0", 10, 10);
    tft.drawBitmap(15, 40, logo_bitmap, 64, 64, TFT_ORANGE, TFT_BLACK);
    break;
  case 1:
    tft.drawString("Theodor Qvarlander", 10, 10);
    tft.drawString("Saga Lindqvist", 10, 30);
    tft.drawString("Josefine Johnsson", 10, 50);
    tft.drawString("Olof Carlander", 10, 70);
    break;
  }

  bool btnPressed = false;
  while (!btnPressed)
  {
    delay(100);
    if (digitalRead(PIN_BUTTON_1) == LOW || digitalRead(PIN_BUTTON_2) == LOW)
    {
      btnPressed = true;

      if (digitalRead(PIN_BUTTON_1) == LOW)
      {
        pageIndex--;
      }
      if (digitalRead(PIN_BUTTON_2) == LOW)
      {
        pageIndex++;
      }

      if (pageIndex < 0)
      {
        pageIndex = 1;
      }
      else if (pageIndex > 1)
      {
        pageIndex = 0;
      }
      // check if both buttons are pressed at the same time
      if (digitalRead(PIN_BUTTON_1) == LOW && digitalRead(PIN_BUTTON_2) == LOW)
      {
        delay(500); // debounce delay
        pomodoroPage();
      }
    }
  }
}

void pomodoroPage()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Pomodoro Mode", 10, 10);

  int workDuration = 25 * 1000;      // 25 minutes
  int breakDuration = 5 * 1000;      // 5 minutes
  int longbreakDuration = 15 * 1000; // 15 minutes
  int cyclesBeforeLongBreak = 4;
  int cycleCount = 0;

  int remainingTime = workDuration;
  bool onBreak = false;
  unsigned long lastUpdate = millis();
  bool paused = false;
  int pausedTime = 0;

  bool btnsPressed = false;
  while (!btnsPressed)
  {
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdate >= 1000) // update every second
    {
      lastUpdate = currentMillis;
      remainingTime -= 1000;

      int minutes = (remainingTime / 1000) / 60;
      int seconds = (remainingTime / 1000) % 60;

      tft.fillRect(10, 40, DISPLAY_WIDTH, DISPLAY_HEIGHT, TFT_BLACK); // clear previous time
      tft.setCursor(10, 40);
      tft.printf("%02d:%02d", minutes, seconds);

      if (onBreak)
      {
        if (cycleCount % cyclesBeforeLongBreak == 0 && cycleCount != 0)
        { // check for long break
          tft.drawString("Long Break", 10, 80);
        }
        else
        {
          tft.drawString("Short Break", 10, 80);
        }
      }
      else
      {
        tft.drawString("Work Time, session nr. " + String(cycleCount + 1), 10, 80);
      }

      if (remainingTime <= 0)
      {
        if (onBreak)
        {
          onBreak = false;
          remainingTime = workDuration;
          cycleCount++;
        }
        else
        {
          if (cycleCount % cyclesBeforeLongBreak == 0 && cycleCount != 0)
          {
            remainingTime = longbreakDuration;
          }
          else
          {
            remainingTime = breakDuration;
          }
          onBreak = true;
        }
      }
    }

    if (digitalRead(PIN_BUTTON_1) == LOW && digitalRead(PIN_BUTTON_2) == LOW)
    {
      delay(500); // debounce delay
      btnsPressed = true;
    }

    if (digitalRead(PIN_BUTTON_1) == LOW) // reset
    {
      cycleCount = 0;
      onBreak = false;
      remainingTime = workDuration;
    }
    else if (digitalRead(PIN_BUTTON_2) == LOW) // pause
    {
      pausedTime = millis();
      tft.drawString("Paused", 10, 110);
      delay(200); // debounce delay
      while (true)
      {
        if (digitalRead(PIN_BUTTON_2) == LOW) // when button is pressed again, exit pauseloop
        {
          delay(200); // debounce delay
          break;
        }
      }
      tft.fillRect(10, 110, DISPLAY_WIDTH, 20, TFT_BLACK); // clear paused text
      lastUpdate += (millis() - pausedTime); // adjust lastUpdate to account for paused time
    }
  }
}

// TFT Pin check
//////////////////
// DO NOT TOUCH //
//////////////////
#if PIN_LCD_WR != TFT_WR ||     \
    PIN_LCD_RD != TFT_RD ||     \
    PIN_LCD_CS != TFT_CS ||     \
    PIN_LCD_DC != TFT_DC ||     \
    PIN_LCD_RES != TFT_RST ||   \
    PIN_LCD_D0 != TFT_D0 ||     \
    PIN_LCD_D1 != TFT_D1 ||     \
    PIN_LCD_D2 != TFT_D2 ||     \
    PIN_LCD_D3 != TFT_D3 ||     \
    PIN_LCD_D4 != TFT_D4 ||     \
    PIN_LCD_D5 != TFT_D5 ||     \
    PIN_LCD_D6 != TFT_D6 ||     \
    PIN_LCD_D7 != TFT_D7 ||     \
    PIN_LCD_BL != TFT_BL ||     \
    TFT_BACKLIGHT_ON != HIGH || \
    170 != TFT_WIDTH ||         \
    320 != TFT_HEIGHT
#error "Error! Please make sure <User_Setups/Setup206_LilyGo_T_Display_S3.h> is selected in <TFT_eSPI/User_Setup_Select.h>"
#error "Error! Please make sure <User_Setups/Setup206_LilyGo_T_Display_S3.h> is selected in <TFT_eSPI/User_Setup_Select.h>"
#error "Error! Please make sure <User_Setups/Setup206_LilyGo_T_Display_S3.h> is selected in <TFT_eSPI/User_Setup_Select.h>"
#error "Error! Please make sure <User_Setups/Setup206_LilyGo_T_Display_S3.h> is selected in <TFT_eSPI/User_Setup_Select.h>"
#endif

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
#error "The current version is not supported for the time being, please use a version below Arduino ESP32 3.0"
#endif