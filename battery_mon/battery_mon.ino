/*
 * Copyright 2024 Jeffrey Chang (jeffsschang@gmail.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <xiaobattery.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() 
{
  Serial.begin(9600 /*115200*/);
  while (!Serial) {
    delay(10); // Wait for Serial monitor to open
  }
  Serial.println("Serial is initialized");

  // OLED
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
   for(;;); // Don't proceed, loop forever
  }
  Serial.println("SSD1306 init completed");

  // Battery
  pinMode(PIN_VBAT, INPUT);    //Battery Voltage monitoring pin

  pinMode(PIN_CHARGING_CURRENT, OUTPUT);   //Charge Current setting pin
  pinMode(VBAT_ENABLE, OUTPUT);   //Enable Battery Voltage monitoring pin
  digitalWrite(PIN_CHARGING_CURRENT, LOW); //Charge Current 100mA   
  digitalWrite(VBAT_ENABLE, LOW); //Enable

  analogReference(AR_INTERNAL_2_4);  //Vref=2.4V
  analogReadResolution(12);         //12bits
  Serial.println("Battery monitoring init completed");

}

void loop() 
{
  digitalWrite(LED_GREEN, LOW);
  delay(500);
  
  int Vadc = analogRead(PIN_VBAT);
  float Vbatt = ((510e3 + 1000e3) / 510e3) * 2.4 * Vadc / 4096;

  // display
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  //display.print("0x");display.print(Vadc, HEX);
  //display.print(", "); 
  display.print("Battery: ");
  display.print(Vbatt, 3);
  display.println("V");
  display.display(); 

  digitalWrite(LED_GREEN, HIGH);
  delay(1000);
}
