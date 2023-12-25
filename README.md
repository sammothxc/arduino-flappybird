# ESP32 Flappy Bird Clone
## Description
This is a Flappy Bird clone for the ESP32. It is written in C++ and uses the Arduino framework. It is designed to run on a Heltec WiFi Kit 32 V2 board, but can be easily modified to run on other ESP32 boards and a 128x64 OLED display.

## Features
- EEPROM high score save
- Circuit and hardware simplified to a single button and an OLED display
- Configurable for different boards and displays

## Configurations
The code defines several configurations, including:

- `HELTEC_BOARD`: If defined, the code assumes a Heltec WiFi Kit 32 V2 board is being used.
- `flipped`: If defined, the screen display is flipped.
- `clear_hiscore`: If defined, the high score is cleared.
- `flash_led`: If defined, an LED flashes on jump and upon death in the game.
- `button, led, oled_sda, oled_scl, oled_rst, oled_i2c`: These define various pins and the I2C address for the OLED display.

## Controls
The game is controlled by a single button. Pressing the button causes the bird to jump. The game can be reset by pressing the button when the game is over.

## Gameplay
The game is a clone of Flappy Bird. The bird is controlled by a single button. Pressing the button causes the bird to jump. The goal is to get the bird through as many pipes as possible. The bird will fall if it hits a pipe or the ground. The game ends when the bird hits a pipe or the ground. The high score is saved in EEPROM.
