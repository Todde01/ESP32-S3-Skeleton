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
#include <iostream>
#include <esp_http_client.h>
#include <WiFiClientSecure.h>
#include <../include/json.hpp>

const String rateAPIBaseURL = "http://api.coinbase.com/v2/exchange-rates?currency=";
const String nameAPIBaseURL = "http://api.coinbase.com/v2/currencies/crypto?currency=";
String cryptoIDs[] = {"BTC", "ETH", "USDT"};
const float USD_TO_SEK = 9.6;
int pageIndex = 0;
bool btnPressed = false;
const char *HOST = "api.coinbase.com";
const int HTTPS_PORT = 443;
const char *PRICES_ENDPOINT = "/v2/exchange-rates?currency=USD";
const char *CURRENCIES_ENDPOINT = "/v2/currencies/crypto?currency=";
const char *SPOT_PRICE_ENDPOINT = "/v2/prices/BTC-USD/buy";
float lastBTCprice;

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

const uint8_t triangle_up[32] = {
  0x00, 0xC0,  // 00000000 11000000
  0x01, 0xE0,  // 00000001 11100000
  0x03, 0xF0,  // 00000011 11110000
  0x07, 0xF8,  // 00000111 11111000
  0x0F, 0xFC,  // 00001111 11111100
  0x1F, 0xFE,  // 00011111 11111110
  0x3F, 0xFF,  // 00111111 11111111
  0x7F, 0xFF,  // 01111111 11111111
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00
};

const uint8_t triangle_down[32] = {
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00,
  0x7F, 0xFF,  // 01111111 11111111
  0x3F, 0xFF,  // 00111111 11111111
  0x1F, 0xFE,  // 00011111 11111110
  0x0F, 0xFC,  // 00001111 11111100
  0x07, 0xF8,  // 00000111 11111000
  0x03, 0xF0,  // 00000011 11110000
  0x01, 0xE0,  // 00000001 11100000
  0x00, 0xC0   // 00000000 11000000
};

// Remember to remove these before commiting in GitHub
String ssid = "Nothing";
String password = "nothing123";

// "tft" is the graphics libary, which has functions to draw on the screen
TFT_eSPI tft = TFT_eSPI();

// Display dimentions
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 170

WiFiClientSecure wifi_client;

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

  // Connect to WIFI
  WiFi.begin(ssid, password);

  // Will be stuck here until a proper wifi is configured
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Connecting to WiFi...", 10, 10);
    tft.drawString("wifi status: " + String(WiFi.status()), 10, 30);
    Serial.println("Attempting to connect to WiFi...");
  }

  wifi_client.setInsecure();

  const long gmtOffset_sec = 3600; // UTC+1
  const char *ntpServer = "pool.ntp.org";
  configTime(gmtOffset_sec, 0, ntpServer); // no daylight offset since its not summer time

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo, 5000))
  { // 5s timeout
    Serial.println("Failed to obtain time");
  }
  else
  {
    Serial.printf("Time set: %02d:%02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  }
  
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
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
  unsigned long lastRefreshOverview = 0;
  unsigned long lastRefreshBTC = 0;
  
  switch (pageIndex)
  {
    case 0:
    infoScreen();
    btnListener();
    break;
    case 1:
    nameScreen();
    btnListener();
    break;
    case 2:
    Serial.println("Crypto overview page\n");
    tft.fillScreen(TFT_BLACK);
    
    btnPressed = false;
    while (!btnPressed)
    {
      unsigned long now = millis();
      if (lastRefreshOverview == 0 || now - lastRefreshOverview >= 3000)
      {
        cryptoOverviewPage();
        lastRefreshOverview = now;
      }
      
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
          pageIndex = 3;
        }
        else if (pageIndex > 3)
        {
          pageIndex = 0;
        }
        if (digitalRead(PIN_BUTTON_1) == LOW && digitalRead(PIN_BUTTON_2) == LOW)
        {
          delay(500); // debounce delay
          pomodoroPage();
        }
      }

      delay(50); // avoid busy loop while still polling quickly
    }
    break;
  case 3:
    Serial.println("Should call BTC page\n");
    tft.fillScreen(TFT_BLACK);

    btnPressed = false;
    while (!btnPressed)
    {
      unsigned long now = millis();
      if (lastRefreshBTC == 0 || now - lastRefreshBTC >= 3000)
      {
        cryptoPage("BTC");
        lastRefreshBTC = now;
      }

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
          pageIndex = 3;
        }
        else if (pageIndex > 3)
        {
          pageIndex = 0;
        }
        if (digitalRead(PIN_BUTTON_1) == LOW && digitalRead(PIN_BUTTON_2) == LOW)
        {
          delay(500); // debounce delay
          pomodoroPage();
        }
      }

      delay(50); // avoid busy loop while still polling quickly
    }
    break;
  }
}

void infoScreen()
{
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Group 6, v 1.0", 10, 10);
  tft.drawBitmap(15, 40, logo_bitmap, 64, 64, TFT_ORANGE, TFT_BLACK);
}

void nameScreen()
{
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Theodor Qvarlander", 10, 10);
  tft.drawString("Saga Lindqvist", 10, 30);
  tft.drawString("Josefine Johnsson", 10, 50);
  tft.drawString("Olof Carlander", 10, 70);
}

void btnListener()
{
  btnPressed = false;
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
        pageIndex = 5;
      }
      else if (pageIndex > 5)
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
  return;
}

void pomodoroPage()
{
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Pomodoro Mode", 10, 10);

  int workDuration = 25 * 60 * 1000;      // 25 minutes
  int breakDuration = 5 * 60 * 1000;      // 5 minutes
  int longbreakDuration = 15 * 60 * 1000; // 15 minutes
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
      lastUpdate += (millis() - pausedTime);               // adjust lastUpdate to account for paused time
    }
  }
}

// Overview with symbol and price in SEK for each cryptocurrency
void cryptoOverviewPage()
{
  Serial.printf("Entering crypto overview function\n");

  String response = fetchRates();

  if (response == "")
  {
    Serial.println("Failed to fetch rates.");
    tft.drawString("Failed to fetch rates.", 10, 40);
    return;
  }

  auto json = nlohmann::json::parse(response.c_str());
  Serial.println("JSON parsed successfully.");

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Crypto Overview", 10, 10);
  tft.fillScreen(TFT_BLACK);

  for (int i = 0; i < 3; i++)
  {
    String symbol = cryptoIDs[i];
    Serial.println("Processing " + symbol);

    String rate = String(json["data"]["rates"][symbol.c_str()].get<std::string>().c_str());
    float rateFloat = (1 / rate.toFloat()) * USD_TO_SEK;
    rate = String(rateFloat, 2);

    Serial.println("Extracted rate for " + symbol + ":  " + rate);

    tft.drawString(symbol + " " + rate + " SEK", 10, 40 + i * 30);
  }
}

void cryptoPage(String symbol)
{

  String name = getName(symbol);
  if (name == "Unknown")
  {
    Serial.println("Unknown symbol: " + symbol);
    tft.drawString("Unknown symbol: " + symbol, 10, 10);
    return;
  }

  Serial.println("Entering crypto page function for " + symbol + "\n");

  String response = fetchRates();

  if (response == "")
  {
    Serial.println("Failed to fetch rates.");
    tft.drawString("Failed to fetch rates.", 10, 40);
    return;
  }

  auto json = nlohmann::json::parse(response.c_str());
  Serial.println("JSON parsed successfully.");

  // tft.fillScreen(TFT_BLACK);
  
  String UsdRate = String(json["data"]["rates"][symbol.c_str()].get<std::string>().c_str());
  float UsdRateFloat = 1 / UsdRate.toFloat();
  UsdRate = String(UsdRateFloat, 2);
  
  float SekRateFloat = (1 / UsdRate.toFloat()) * USD_TO_SEK;
  String SekRate = String(SekRateFloat, 2);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString(getName(symbol).c_str(), 10, 10);
  tft.drawString(symbol.c_str(), 10, 30);
  tft.drawString("USD: " + UsdRate, 10, 50);
  tft.drawString("SEK: " + SekRate, 10, 70);
  
  if (getLastPrice(symbol) < UsdRateFloat)
  {
    tft.drawBitmap(15, 100, triangle_up, 16, 16, TFT_GREEN, TFT_BLACK);
  } else if (getLastPrice(symbol) > UsdRateFloat) {
    tft.drawBitmap(15, 100, triangle_down, 16, 16, TFT_RED, TFT_BLACK);
  }

  setLastPrice(symbol, UsdRateFloat);
}

String getName(String symbol)
{
  switch (symbol.c_str()[0])
  {
  case 'B':
    return "Bitcoin";
  case 'E':
    return "Ethereum";
  case 'U':
    return "Tether";
  default:
    return "Unknown";
  }
}

float getLastPrice(String symbol) //USD rate used for comparison
{
  if (symbol == "BTC")
  {
    return lastBTCprice;
  }
  else
  {
    return -1.0; // unknown symbol
  }
}

void setLastPrice(String symbol, float price) //USD rate used for comparison
{
  if (symbol == "BTC")
  {
    lastBTCprice = price;
  }
}

String fetchRates()
{
  Serial.println("Connecting to Coinbase...");

  if (!wifi_client.connect(HOST, HTTPS_PORT))
  {
    Serial.println("Connection to api.coinbase.com failed!");
    return "";
  }

  Serial.println("Connected, sending request...");

  Serial.println("Requesting data");
  wifi_client.print(String("GET ") + PRICES_ENDPOINT + " HTTP/1.1\r\n" +
                    "Host: " + HOST + "\r\n" +
                    "Connection: close\r\n\r\n");

  while (wifi_client.connected() && !wifi_client.available())
  {
    delay(10);
  }

  String json;
  while (wifi_client.available())
  {
    String line = wifi_client.readStringUntil('\n');
    if (line[0] == '{')
    { //wonky way to find the start of the JSON payload
      json = line;
    }
  }

  wifi_client.stop();
  Serial.println("Connection closed.");
  return json;
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
