# ESP32 Flappy Bird Game Clone
## Description
This is a Flappy Bird clone for the ESP32. It is written in C++ and uses the Arduino framework. It is designed to run on any ESP32 board, but can was written specifically with the Heltec Wifi kit 32. This board among others are perfect because they come with a 128x64 OLED display, a button, and an LED.

## Gameplay
The bird is controlled by a single button. Pressing the button causes the bird to jump. The goal is to get the bird through as many pipes as possible. The bird will fall if it hits a pipe or the ground. The game ends when the bird hits a pipe or the ground. The high score is saved in EEPROM. The game can be reset by pressing the button when the game is over.

## Features
- Written specifically for ESP32 OLED development boards
- EEPROM high score save persists across resets and power cycles
- Circuit and hardware simplified to a single button and an OLED display
- Configurable for different boards and displays
- Difficulty increases as the game progresses

## Configurations
The code defines several configurations, including:
- `HELTEC_BOARD`: If defined, the code assumes a Heltec WiFi Kit 32 V2 board is being used.
- `flipped`: If defined, the screen display is flipped.
- `clear_hiscore`: If defined, the high score is cleared.
- `flash_led`: If defined, an LED flashes on jump and upon death in the game.
- `button, led, oled_sda, oled_scl, oled_rst, oled_i2c`: These define various pins and the I2C address for the OLED display.

## Prerequisites
You will need the following libraries installed on your Arduino IDE or PlatformIO:
- ESP8266 and ESP32 OLED driver for SSD1306 displays
- U8g2
- Heltec ESP32 Dev-Boards (if using a Heltec board. IMO, this is the best ESP32 board for this project)

## Installation
- Clone the repository to your local machine.
- Open the project in your IDE.
- Install the necessary libraries.
- Upload the code to your ESP32 board.

## License
This project is licensed under the GNU General Public License v3.0 - see the LICENSE.md file for details.

## TODO
- Port to ESP8266
- Port to Arduino AVRs if possible