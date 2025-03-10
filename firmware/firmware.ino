#include <BleKb.h>

BleComboKeyboard keyboard("CAFE_68", "ESP32_S3", 100);
BleComboMouse mouse(&keyboard);

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

#define MATRIX_COL_CNT 15
#define MATRIX_ROW_CNT 5
#define LAYER_CNT 2

#define MAXIMUM_STROKES 6
#define DEBOUNCE_DELAY 15

const byte ROW_PINS[MATRIX_ROW_CNT] = { 42, 40, 15, 17, 1 };
const byte COL_PINS[MATRIX_COL_CNT] = { 10, 12, 14, 21, 13, 11, 9, 18, 8, 16, 7, 5, 2, 4, 6 };
const int KEY_MAP[LAYER_CNT][MATRIX_ROW_CNT][MATRIX_COL_CNT] = {
  { { KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', KEY_BACKSPACE, KEY_DELETE },
    { KEY_TAB, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\\', KEY_INSERT },
    { KEY_CAPS_LOCK, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', ' ', KEY_RETURN, KEY_HOME },
    { KEY_LEFT_SHIFT, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', KEY_RIGHT_SHIFT, ' ', KEY_UP_ARROW, KEY_END },
    { KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, KEY_NULL, KEY_NULL, ' ', KEY_NULL, KEY_NULL, KEY_NULL, KEY_MENU, KEY_RIGHT_CTRL, KEY_LEFT_ARROW, ' ', KEY_DOWN_ARROW, KEY_RIGHT_ARROW } },
  { { '`', KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12, KEY_BACKSPACE, KEY_DELETE },
    { KEY_TAB, MED_MUT, MED_VUP, MED_VDN, 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', KEY_NULL, KEY_INSERT },
    { KEY_CAPS_LOCK, MED_PRE, MED_STO, MED_PLY, MED_NXT, 'g', 'h', 'j', 'k', 'l', ';', '\'', ' ', KEY_RETURN, KEY_PAGE_UP },
    { KEY_LEFT_SHIFT, 'z', 'x', MED_CAL, 'v', MED_BRO, 'n', 'm', KEY_NULL, KEY_NULL, KEY_NULL, KEY_RIGHT_SHIFT, ' ', KEY_UP_ARROW, KEY_PAGE_DOWN },
    { KEY_LEFT_CTRL, KEY_LEFT_GUI, KEY_LEFT_ALT, KEY_NULL, KEY_NULL, ' ', KEY_NULL, KEY_NULL, KEY_NULL, KEY_MENU, KEY_RIGHT_CTRL, KEY_LEFT_ARROW, ' ', KEY_DOWN_ARROW, KEY_RIGHT_ARROW } }
};

byte PRE_MATRIX_VAL[MATRIX_ROW_CNT][MATRIX_COL_CNT] = {};
byte CUR_MATRIX_VAL[MATRIX_ROW_CNT][MATRIX_COL_CNT] = {};

void setup() {
  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);
  for (int i = 0; i < MATRIX_COL_CNT; i++) {
    pinMode(COL_PINS[i], OUTPUT);
    digitalWrite(COL_PINS[i], LOW);
  }
  for (int i = 0; i < MATRIX_ROW_CNT; i++) {
    pinMode(ROW_PINS[i], INPUT);
  }
  Serial.begin(115200);
  Serial.println("START");
}


void loop() {
  delay(50);
  readMatrixValue();
}

void readMatrixValue() {
  for (int c = 0; c < MATRIX_COL_CNT; c++) {
    digitalWrite(COL_PINS[c], HIGH);
    for (int r = 0; r < MATRIX_ROW_CNT; r++) {
      CUR_MATRIX_VAL[r][c] = digitalRead(ROW_PINS[r]);
      if (CUR_MATRIX_VAL[r][c] == HIGH) {
        Serial.print("r = ");
        Serial.print(r);
        Serial.print(", c = ");
        Serial.println(c);
      }
    }
    digitalWrite(COL_PINS[c], LOW);
  }
}