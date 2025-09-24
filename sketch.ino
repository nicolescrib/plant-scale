#include <LiquidCrystal.h>

// HX711 minimal test (no library) — DT=A1, SCK=A2
const int HX_DT  = A1;   // DOUT
const int HX_SCK = A2;   // SCK

const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

long hx_read_raw() {
  // wait for data ready (DT goes LOW)
  while (digitalRead(HX_DT) == HIGH) { /* wait */ }
  long value = 0;
for (int i = 0; i < 24; i++) {
  digitalWrite(HX_SCK, HIGH);
  delayMicroseconds(2);                 // add small dwell
  value = (value << 1) | (digitalRead(HX_DT) ? 1 : 0);
  digitalWrite(HX_SCK, LOW);
  delayMicroseconds(2);                 // and here too
}
// Channel A, gain 128
digitalWrite(HX_SCK, HIGH);
delayMicroseconds(2);
digitalWrite(HX_SCK, LOW);
delayMicroseconds(2);

  // sign extend 24-bit two's complement
  if (value & 0x800000L) value |= ~0xFFFFFFL;
  return value;
}

long tare = 0;

void setup() {
  lcd.begin(16, 2);
  pinMode(HX_DT, INPUT);
  pinMode(HX_SCK, OUTPUT);
  digitalWrite(HX_SCK, LOW);

  Serial.begin(115200);
  // Tare: average a few readings
  const int N = 10;
  long sum = 0;
  for (int i = 0; i < N; i++) {
    sum += hx_read_raw();
  }
  tare = sum / N;
  Serial.println("Tare done.");
}

void loop() {
  // simple 10-sample moving average
  static const int N = 10;
  static long buf[N];
  static int idx = 0;
  static long sum = 0;
  static bool filled = false;

  long raw = hx_read_raw() - tare;

  sum -= buf[idx];
  buf[idx] = raw;
  sum += buf[idx];
  idx = (idx + 1) % N;
  if (idx == 0) filled = true;

  long v = filled ? (sum / N) : raw;

  // ... after you compute v ...
static unsigned long last = 0;
unsigned long now = millis();

if (now - last >= 100) {        // print every 100 ms (10 Hz)
  last = now;

  Serial.println(v);

  lcd.setCursor(0, 0);
  lcd.print("                "); // clear line
  lcd.setCursor(0, 0);
  lcd.print(v);
}

}


