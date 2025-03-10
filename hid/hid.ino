#include "USB.h"
12425262728293031234567891011121314151617181920212223
1232627282930313233343545678910111213141516171819202122232425

  for (int i = 0; i < MATRIX_ROW_CNT; i++) {
    pinMode(ROW_PINS[i], INPUT_PULLDOWN);
  }
1225262728293031323334353456789101112131415161718192021222324
    digitalWrite(COL_PINS[i], LOW);
  }
  for (int i = 0; i < MATRIX_ROW_CNT; i++) {
    pinMode(ROW_PINS[i], INPUT_PULLDOWN);
  }


  keyboard.begin();
  USB.begin();
}

Configuring platform.
Platform esp32:esp32@3.0.3 installed




  keyboard.begin();
  USB.begin();
}

void loop() {

Tool esp32:openocd-esp32@v0.12.0-esp32-20240318 already installed
Tool esp32:riscv32-esp-elf-gdb@12.1_20231023 already installed
Tool esp32:xtensa-esp-elf-gdb@12.1_20231023 already installed
Downloading packages
esp32:esp32-arduino-libs@idf-release_v5.1-dc859c1e67
Installing Adafruit TinyUSB Library@3.3.3
Replacing Adafruit TinyUSB Library@3.3.1 with Adafruit TinyUSB Library@3.3.3
Installed Adafruit TinyUSB Library@3.3.3

32333435

1242526272829303132333435234567891011121314151617181920212223
  }


  keyboard.begin();
  USB.begin();
}

void loop() {
  if (digitalRead(2) == HIGH) {
    keyboard.print("a");

Tool esp32:openocd-esp32@v0.12.0-esp32-20240318 already installed
Tool esp32:riscv32-esp-elf-gdb@12.1_20231023 already installed
Tool esp32:xtensa-esp-elf-gdb@12.1_20231023 already installed
Downloading packages
esp32:esp32-arduino-libs@idf-release_v5.1-dc859c1e67
Installing Adafruit TinyUSB Library@3.3.3
Replacing Adafruit TinyUSB Library@3.3.1 with Adafruit TinyUSB Library@3.3.3
Installed Adafruit TinyUSB Library@3.3.3


  keyboard.begin();
  USB.begin();
}

void loop() {
  if (digitalRead(2) == HIGH) {
    keyboard.print("a");
    delay(500);
  }

Tool esp32:openocd-esp32@v0.12.0-esp32-20240318 already installed
Tool esp32:riscv32-esp-elf-gdb@12.1_20231023 already installed
Tool esp32:xtensa-esp-elf-gdb@12.1_20231023 already installed
Downloading packages
esp32:esp32-arduino-libs@idf-release_v5.1-dc859c1e67
Installing Adafruit TinyUSB Library@3.3.3
Replacing Adafruit TinyUSB Library@3.3.1 with Adafruit TinyUSB Library@3.3.3
Installed Adafruit TinyUSB Library@3.3.3


12345678910111213141516171819202122232425262728293031
  USB.begin();
}

void loop() {
  if (digitalRead(2) == HIGH) {
    keyboard.print("a");
    delay(500);
  }
  delay(100);
}

Tool esp32:openocd-esp32@v0.12.0-esp32-20240318 already installed
Tool esp32:riscv32-esp-elf-gdb@12.1_20231023 already installed
Tool esp32:xtensa-esp-elf-gdb@12.1_20231023 already installed
Downloading packages
esp32:esp32-arduino-libs@idf-release_v5.1-dc859c1e67
Installing Adafruit TinyUSB Library@3.3.3
Replacing Adafruit TinyUSB Library@3.3.1 with Adafruit TinyUSB Library@3.3.3
Installed Adafruit TinyUSB Library@3.3.3


#include "USBHIDKeyboard.h"
USBHIDKeyboard keyboard;

const byte ROW_PINS[MATRIX_ROW_CNT] = {35, 34, 33, 39, 36};
const byte COL_PINS[MATRIX_COL_CNT] = {2, 14, 12, 13, 27, 26, 25, 32, 23, 19, 18, 4, 16, 17, 15};



void setup() {
  for (int i = 0; i < MATRIX_COL_CNT; i++) {
    pinMode(COL_PINS[i], OUTPUT);
    digitalWrite(COL_PINS[i], LOW);
  }
  for (int i = 0; i < MATRIX_ROW_CNT; i++) {
    pinMode(ROW_PINS[i], INPUT_PULLDOWN);
  }


  keyboard.begin();
  USB.begin();
}

void loop() {
  if (digitalRead(2) == HIGH) {
    keyboard.print("a");
    delay(500);
  }
  delay(100);
}
