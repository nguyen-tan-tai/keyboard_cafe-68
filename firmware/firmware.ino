#include "BleKeyboard.h"
#include <EEPROM.h>
BleKeyboard bleKeyboard("Caphe-68", "Tai Sang", 50);
const byte MAX_DEVICE = 3;
uint8_t MACAddress[MAX_DEVICE][6] = {
  {0x35, 0xAF, 0xA4, 0x07, 0x0B, 0x66},
  {0x31, 0xAE, 0xAA, 0x47, 0x0D, 0x61},
  {0x31, 0xAE, 0xAC, 0x42, 0x0A, 0x31}
};

#define KEY_NULL 0x00
#define KEY_MENU 0xED
#define MED_MUT 256
#define MED_VUP 257
#define MED_VDN 258
#define MED_CAL 259
#define MED_PRE 260
#define MED_STO 261
#define MED_PLY 262
#define MED_NXT 263
#define MED_BRO 264

#define RESTART 1000
#define DEVICE_1 1001
#define DEVICE_2 1002
#define DEVICE_3 1003

#define MATRIX_COL_CNT 15
#define MATRIX_ROW_CNT 5
#define LAYER_CNT 2

#define MAXIMUM_STROKES   10
#define DEBOUNCE_DELAY    15

const byte ROW_PINS[MATRIX_ROW_CNT] = {36, 39, 33, 34, 35};
const byte COL_PINS[MATRIX_COL_CNT] = {15, 2, 4, 16, 17, 18, 23, 19, 32, 13, 12, 14, 25, 27, 26};
const int KEY_MAP[LAYER_CNT][MATRIX_ROW_CNT][MATRIX_COL_CNT] = {
  {
    {KEY_ESC,        '1',          '2',          '3',      '4',      '5',      '6',      '7',      '8',      '9',      '0',            '-',             '=',            KEY_BACKSPACE,  KEY_DELETE},
    {KEY_TAB,        'q',          'w',          'e',      'r',      't',      'y',      'u',      'i',      'o',      'p',            '[',             ']',            '\\',           KEY_INSERT},
    {KEY_CAPS_LOCK,  'a',          's',          'd',      'f',      'g',      'h',      'j',      'k',      'l',      ';',            '\'',            ' ',            KEY_RETURN,     KEY_HOME},
    {KEY_LEFT_SHIFT, 'z',          'x',          'c',      'v',      'b',      'n',      'm',      ',',      '.',      '/',            KEY_RIGHT_SHIFT, ' ',            KEY_UP_ARROW,   KEY_END},
    {KEY_LEFT_CTRL,  KEY_LEFT_GUI, KEY_LEFT_ALT, KEY_NULL, KEY_NULL, ' ',      KEY_NULL, KEY_NULL, KEY_NULL, KEY_MENU, KEY_RIGHT_CTRL, KEY_LEFT_ARROW,  ' ',            KEY_DOWN_ARROW, KEY_RIGHT_ARROW}
  },
  {
    {'`',            KEY_F1,       KEY_F2,       KEY_F3,   KEY_F4,   KEY_F5,   KEY_F6,   KEY_F7,   KEY_F8,   KEY_F9,   KEY_F10,        KEY_F11,         KEY_F12,        KEY_BACKSPACE,  KEY_DELETE},
    {KEY_TAB,        MED_MUT,      MED_VUP,      MED_VDN,  'r',      't',      'y',      'u',      'i',      'o',      'p',            '[',             ']',            RESTART,        KEY_INSERT},
    {KEY_CAPS_LOCK,  MED_PRE,      MED_STO,      MED_PLY,  MED_NXT,  'g',      'h',      'j',      'k',      'l',      ';',            '\'',            ' ',            KEY_RETURN,     KEY_PAGE_UP},
    {KEY_LEFT_SHIFT, 'z',          'x',          MED_CAL,  'v',      MED_BRO,  'n',      'm',      DEVICE_1, DEVICE_2, DEVICE_3,       KEY_RIGHT_SHIFT, ' ',            KEY_UP_ARROW,   KEY_PAGE_DOWN},
    {KEY_LEFT_CTRL,  KEY_LEFT_GUI, KEY_LEFT_ALT, KEY_NULL, KEY_NULL, ' ',      KEY_NULL, KEY_NULL, KEY_NULL, KEY_MENU, KEY_RIGHT_CTRL, KEY_LEFT_ARROW,  ' ',            KEY_DOWN_ARROW, KEY_RIGHT_ARROW}
  }
};
byte PRE_MATRIX_VAL[MATRIX_ROW_CNT][MATRIX_COL_CNT] = {};
byte CUR_MATRIX_VAL[MATRIX_ROW_CNT][MATRIX_COL_CNT] = {};

void changeId(int deviceNumber) {
  if (deviceNumber < MAX_DEVICE) {
    EEPROM.write(0, deviceNumber);
    EEPROM.commit();
    esp_sleep_enable_timer_wakeup(1);
    esp_deep_sleep_start();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(39, INPUT);
  for (int i = 0; i < MATRIX_COL_CNT; i++) {
    pinMode(COL_PINS[i], OUTPUT);
    digitalWrite(COL_PINS[i], LOW);
  }
  for (int i = 0; i < MATRIX_ROW_CNT; i++) {
    pinMode(ROW_PINS[i], INPUT_PULLDOWN);
  }
  EEPROM.begin(4);
  int deviceChose = EEPROM.read(0);
  esp_base_mac_addr_set(&MACAddress[deviceChose][0]);
  bleKeyboard.begin();
}

void readMatrixValue() {
  for (int c = 0; c < MATRIX_COL_CNT; c++) {
    digitalWrite(COL_PINS[c], HIGH);
    for (int r = 0; r < MATRIX_ROW_CNT; r++) {
      CUR_MATRIX_VAL[r][c] = digitalRead(ROW_PINS[r]);
    }
    digitalWrite(COL_PINS[c], LOW);
  }
}

void rotateMatrixValue() {
  for (int c = 0; c < MATRIX_COL_CNT; c++) {
    for (int r = 0; r < MATRIX_ROW_CNT; r++) {
      PRE_MATRIX_VAL[r][c] = CUR_MATRIX_VAL[r][c];
    }
  }
}

#define EMPTY = -1;
#define KEY_PRESS = 1;
#define KEY_RELEASE = 2;

byte defaultLayout = 0;

boolean isFnPress() {
  return CUR_MATRIX_VAL[4][8];
}

boolean isMenuPress() {
  return CUR_MATRIX_VAL[4][9];
}

boolean isEscPress() {
  return CUR_MATRIX_VAL[0][0];
}

void doKeyboard() {
  if (isFnPress() && isEscPress()) {
    defaultLayout = !defaultLayout;
    Serial.println("defaultLayout = " + defaultLayout);
    bleKeyboard.releaseAll();
    delay(250);
    return;
  }
  byte layout = defaultLayout;
  if (isFnPress()) {
    if (isMenuPress()) {
      layout = 2;
    } else {
      layout = (defaultLayout == 0) ? 1 : 0;
    }
  }
  for (int r = 0; r < MATRIX_ROW_CNT; r++) {
    for (int c = 0; c < MATRIX_COL_CNT; c++) {
      int nk = CUR_MATRIX_VAL[r][c];
      int ok = PRE_MATRIX_VAL[r][c];
      int key = KEY_MAP[layout][r][c];
      if (ok == nk || key == KEY_NULL) {
        continue;
      }
      if (nk) {
        Serial.println("layout = " + String(layout) + ", press = " + key);
        keyPress(key);
      } else {
        Serial.println("layout = " + String(layout) + ", release = " + key);
        keyRelease(key);
      }
    }
  }
}

void keyPress(int key) {
  switch (key) {
    case KEY_NULL:
      break;
    case MED_MUT:
      bleKeyboard.press(KEY_MEDIA_NEXT_TRACK);
    case MED_VUP:
      bleKeyboard.press(KEY_MEDIA_VOLUME_UP);
      break;
    case MED_VDN:
      bleKeyboard.press(KEY_MEDIA_VOLUME_DOWN);
      break;
    case RESTART:
      ESP.restart();
      break;
    case MED_CAL:
      bleKeyboard.press(KEY_MEDIA_CALCULATOR);
      break;
    case MED_PRE:
      bleKeyboard.press(KEY_MEDIA_PREVIOUS_TRACK);
      break;
    case MED_PLY:
      bleKeyboard.press(KEY_MEDIA_PLAY_PAUSE);
      break;
    case MED_STO:
      bleKeyboard.press(KEY_MEDIA_STOP);
      break;
    case MED_NXT:
      bleKeyboard.press(KEY_MEDIA_NEXT_TRACK);
      break;
    case MED_BRO:
      bleKeyboard.press(KEY_MEDIA_LOCAL_MACHINE_BROWSER);
      break;
    case DEVICE_1:
      changeId(1);
      break;
    case DEVICE_2:
      changeId(2);
      break;
    case DEVICE_3:
      changeId(3);
      break;
    default:
      bleKeyboard.press(key);
  }
}

void keyRelease(int key) {
  switch (key) {
    case KEY_NULL:
      break;
    case MED_MUT:
      bleKeyboard.release(KEY_MEDIA_NEXT_TRACK);
    case MED_VUP:
      bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
      break;
    case MED_VDN:
      bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
      break;
    case RESTART:
      ESP.restart();
      break;
    case MED_CAL:
      bleKeyboard.release(KEY_MEDIA_CALCULATOR);
      break;
    case MED_PRE:
      bleKeyboard.release(KEY_MEDIA_PREVIOUS_TRACK);
      break;
    case MED_PLY:
      bleKeyboard.release(KEY_MEDIA_PLAY_PAUSE);
      break;
    case MED_STO:
      bleKeyboard.release(KEY_MEDIA_STOP);
      break;
    case MED_NXT:
      bleKeyboard.release(KEY_MEDIA_NEXT_TRACK);
      break;
    case MED_BRO:
      bleKeyboard.release(KEY_MEDIA_LOCAL_MACHINE_BROWSER);
      break;
    case DEVICE_1:
      changeId(1);
      break;
    case DEVICE_2:
      changeId(2);
      break;
    case DEVICE_3:
      changeId(3);
      break;
    default:
      bleKeyboard.release(key);
  }
}

unsigned long lastScanTime = 0;

void loop() {
  delay(1);
  unsigned long scanTime = millis();
  if (scanTime - lastScanTime < DEBOUNCE_DELAY) {
    return;
  }
  lastScanTime = scanTime;
  readMatrixValue();
  if (bleKeyboard.isConnected()) {
    doKeyboard();
  } else {
    for (int r = 0; r < MATRIX_ROW_CNT; r++) {
      for (int c = 0; c < MATRIX_COL_CNT; c++) {
        int nk = CUR_MATRIX_VAL[r][c];
        if (nk) {
          Serial.print("  +   ");
        } else {
          Serial.print("  .   ");
        }
      }
      Serial.println("");
    }
    Serial.println("-------------------------------------------------------------------------------------");
  }
  rotateMatrixValue();
}
