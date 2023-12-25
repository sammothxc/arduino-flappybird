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
#ifdef HELTEC_BOARD
  #include "heltec.h"
#else
  #include <SSD1306Wire.h>
#endif
#define space 32
#define fx 30
#define speed 0.01
#define oled_w 128
#define oled_h 64
#define eeprom_size 1
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
void wait4start(){                  //wait for button press
    if(digitalRead(button) == 0){
      playing = true;
      printed = false;
  }
}

void generate_map(){               //generate map of pipes
  for(int i=0; i<map_buffer; i++){
    wallx[i] = oled_w + ((i + 1) * space);
    gap[i] = random(0, 33);
    gap_size[i] = random(27, (61 - gap[i]));
  }
}

void homescreen(){                 //draw homescreen
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

void clear_hiscore(){            //clear hi score from EEPROM
  EEPROM.write(flappy_addr, 0);
  EEPROM.commit();
}

void draw_map(){                 //draw map of pipes
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

void setup(){
  pinMode(button, INPUT_PULLUP);
  EEPROM.begin(eeprom_size);
  hiscore = EEPROM.read(flappy_addr);
  display.init();
  display.setFont(ArialMT_Plain_10);
  #ifdef flipped
    display.flipScreenVertically();
  #endif
  #ifdef flash_led
    pinMode(led, OUTPUT);
  #endif
  #ifdef clear_hiscore
    clear_hiscore();
  #endif
}

void loop(){
  if(not playing && not printed){
    homescreen();
  }
  if(not playing){
    wait4start();
  }
  if(playing){
    display.clear();
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
    display.display();
  }
}