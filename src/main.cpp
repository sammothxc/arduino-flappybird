/*
    Arduino Flappybird Game
    Copyright (C) 2023  Sam Warr

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

///////////////////////////////////////////////////////////
/////////////////////// LIBRARIES /////////////////////////
///////////////////////////////////////////////////////////
#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include "images.h"
///////////////////////////////////////////////////////////
/////////////////////// CONFIGS  //////////////////////////
///////////////////////////////////////////////////////////
#define HELTEC_BOARD            //uncomment if using heltec board
#define flipped                 //uncomment to flip screen
//#define clear_hiscore         //uncomment to clear hi score
//#define flash_led             //uncomment to flash led on death
#define button 0                //button pin
#define led 25                  //led pin
#define oled_sda 4              //sda pin
#define oled_scl 15             //scl pin
#define oled_rst 16             //rst pin
#define oled_i2c 0x3c           //oled i2c address
///////////////////////////////////////////////////////////
/////////////////////// DEFINES ///////////////////////////
///////////////////////////////////////////////////////////
#ifdef ARDUINO_ARCH_AVR
  #include <Adafruit_SSD1306.h>
  #include <Adafruit_GFX.h>
#endif
#ifdef HELTEC_BOARD
  #include "heltec.h"
#elif defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
  #include <SSD1306Wire.h>
#endif
#define space 32
#define fx 30
#define speed 0.01
#define oled_w 128
#define oled_h 64
#define eeprom_size 5
#define flappy_addr 0
#define xbm_h 9
#define xbm_w 14
#define map_buffer 4
#define speed_u 0.03
#define speed_d 0.01
#ifdef HELTEC_WIFI_LORA_32_V2
  SSD1306Wire  display(oled_i2c, oled_sda, oled_scl, oled_rst);
#else
  SSD1306Wire  display(oled_i2c, oled_sda, oled_scl, GEOMETRY_128_64);
#endif
///////////////////////////////////////////////////////////
/////////////////////// VARIABLES /////////////////////////
///////////////////////////////////////////////////////////
unsigned long current = 0;
float wallx[map_buffer];
float fy = 22.00;
bool playing = false;
bool played = false;
bool up = false;
bool printed = false;
bool debouncer = false;
int gap[map_buffer];
int gap_size[map_buffer];
int hiscore = 0;
int score = 0;
int last = 0;
int led_state = LOW;
///////////////////////////////////////////////////////////
/////////////////////// FUNCTIONS /////////////////////////
///////////////////////////////////////////////////////////
void wait4start(){
    if(digitalRead(button) == 0){
      playing = true;
      printed = false;
  }
}

void generate_map(){
  for(int i=0; i<map_buffer; i++){
    wallx[i] = oled_w + ((i + 1) * space);
    gap[i] = random(0, 33);
    gap_size[i] = random(27, (61 - gap[i]));
  }
}

#ifdef ARDUINO_ARCH_ESP32 || ARDUINO_ARCH_ESP8266
void homescreen(){
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 4, "Flappy");
  display.drawXbm(0, 0, oled_w, oled_h, background);
  display.drawXbm(55, 7, xbm_w, xbm_h, bird);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 47, "Press to Start");
  if(not played){
    display.drawString(0, 28, "Hi score: " + String(hiscore));
  }
  if(played){
    display.drawString(0, 24, "Scored: " + String(last));
    display.drawString(0, 34, "Hi score: " + String(hiscore));
  }
  display.display();
  printed = true;
  generate_map();
}

void draw_map(){
  for(int j=0; j<map_buffer; j++){
    display.fillRect(wallx[j], 0, 6, oled_h);
    display.setColor(BLACK);
    display.fillRect(wallx[j], gap[j], 6, gap_size[j]);
    display.setColor(WHITE);
  }
}

void draw_bird(){               //draw bird image
  display.drawXbm(fx, fy, xbm_w, xbm_h, bird);
}

void draw_score(){              //draw scorebox
  display.fillRect(0, 0, 15, 13);
  display.setColor(BLACK);
  display.drawString(2, 0, String(score));
  display.setColor(WHITE);
}

void clearDisplay(){
  display.clear();
}

void setup(){
  pinMode(button, INPUT_PULLUP);
  EEPROM.begin(eeprom_size);
  hiscore = EEPROM.read(flappy_addr);
  display.init();
  display.setFont(ArialMT_Plain_10);
  #ifdef flipped
    display.flipScreenVertically();
  #endif
  #ifdef clear_hiscore
    clear_hiscore();
  #endif
}
#endif

#ifdef ARDUINO_ARCH_AVR
void homescreen(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 4);
  display.print("Flappy");
  display.drawXBitmap(0, 0, background, oled_w, oled_h, SSD1306_WHITE);
  display.drawXBitmap(55, 7, bird, xbm_w, xbm_h, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 47);
  display.print("Press to Start");
  if(not played){
    display.setCursor(0, 28);
    display.print("Hi score: " + String(hiscore));
  }
  if(played){
    display.setCursor(0, 24);
    display.print("Scored: " + String(last));
    display.setCursor(0, 34);
    display.print("Hi score: " + String(hiscore));
  }
  display.display();
  printed = true;
  generate_map();
}

void draw_map(){
  for(int j=0; j<map_buffer; j++){
    display.fillRect(wallx[j], 0, 6, oled_h, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
    display.fillRect(wallx[j], gap[j], 6, gap_size[j], SSD1306_BLACK);
    display.setTextColor(SSD1306_WHITE);
  }
}

void draw_bird(){
  display.drawXBitmap(fx, fy, bird, xbm_w, xbm_h, SSD1306_WHITE);
}

void draw_score(){              //draw scorebox
  display.fillRect(0, 0, 15, 13, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(2, 0);
  display.print(String(score));
  display.setTextColor(SSD1306_WHITE);
}

void clearDisplay(){
  display.clearDisplay();
}

void setup(){
  pinMode(button, INPUT_PULLUP);
  EEPROM.begin();
  hiscore = EEPROM.read(flappy_addr);
  display.begin(SSD1306_SWITCHCAPVCC, oled_i2c);
  display.setTextSize(1);
  #ifdef clear_hiscore
    clear_hiscore();
  #endif
}
#endif

void clear_hiscore(){
  EEPROM.write(flappy_addr, 0);
  EEPROM.commit();
}

void death(){                  //death screen and clear game
  playing = false;
  fy = 22;
  last = score;
  score = 0;
  played = true;
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  if(last > hiscore){
    hiscore = last;
    EEPROM.write(flappy_addr, hiscore);
    EEPROM.commit();
  }
}

void loop(){
  if(not playing && not printed){
    homescreen();
  }
  if(not playing){
    wait4start();
  }
  if(playing){
    clearDisplay();
    if(digitalRead(button) == 0){
      if(not debouncer){
        current = millis();
        up = true;
        led_state = HIGH;
        debouncer = true;
      }
    }
      else{
        debouncer = false;
      }
    for(int j=0; j<map_buffer; j++){
      wallx[j] = wallx[j] - speed;
      if(wallx[j] < -7){
        score ++;
        gap[j] = random(8, 32);
        wallx[j] = oled_w;
      }
    }
    if((current + 185) < millis()){
      up = false;
    }
    if((current + 40) < millis()){
      led_state = LOW;
    }
    if(not up){
      fy = fy + speed_d;
    }
    else{
      fy = fy - speed_u;
    }
    digitalWrite(led, led_state);
    if(fy > oled_h || fy < 0){
      death();
    }
    for(int m=0; m<map_buffer; m++){
      if(wallx[m] <= fx + 7 && fx + 7 <= wallx[m] + 6){
        if(fy < gap[m] || fy + 8 > gap[m] + gap_size[m]){
          death();
        }
      }
    }
    draw_map();
    draw_bird();
    draw_score();
    clearDisplay();
  }
}