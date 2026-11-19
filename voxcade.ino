/* -------------------------------------------------------------------------
   VOXCADE - ESP32 BLE Gamepad (4 Tombol + Indikator LED Hijau & Merah)
   Nama Bluetooth: "VOXCADE Controller"
------------------------------------------------------------------------- */
#include <Arduino.h>
#include <BleGamepad.h>

// Definisi Pin 4 Push Button pada ESP32
#define BUTTON_PIN1 12  // GPIO 12 (Tombol 1 - Gerak Kiri)
#define BUTTON_PIN2 14  // GPIO 14 (Tombol 2 - Gerak Kanan)
#define BUTTON_PIN3 27  // GPIO 27 (Tombol 3 - Lompat / Jump)
#define BUTTON_PIN4 26  // GPIO 26 (Tombol 4 - Dash / Action)

// Definisi Pin Indikator LED
#define LED_HIJAU 25   // GPIO 18 (Menyala saat Bluetooth Terhubung)
#define LED_MERAH 33   // GPIO 19 (Menyala saat Bluetooth Belum Terhubung)

// Jumlah total tombol
#define NUM_BUTTONS 4

// Inisialisasi BLE Gamepad dengan nama "VOXCADE Controller"
BleGamepad bleGamepad("VOXCADE Controller", "VOXCADE", 100);

// Array Pin & Pemetaan Tombol HID Gamepad
int buttonPins[NUM_BUTTONS] = { BUTTON_PIN1, BUTTON_PIN2, BUTTON_PIN3, BUTTON_PIN4 };
int buttons[NUM_BUTTONS]    = { BUTTON_1, BUTTON_2, BUTTON_4, BUTTON_5 };
int prevState[NUM_BUTTONS]  = { HIGH, HIGH, HIGH, HIGH };

void setup() {
  Serial.begin(115200);

  // Inisialisasi ke-4 pin tombol sebagai INPUT_PULLUP
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Inisialisasi pin LED sebagai OUTPUT
  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);

  // Kondisi awal sebelum terhubung: LED Merah menyala, LED Hijau mati
  digitalWrite(LED_HIJAU, LOW);
  digitalWrite(LED_MERAH, HIGH);

  // Mulai layanan Bluetooth BLE Gamepad
  bleGamepad.begin();
  Serial.println("VOXCADE Controller (4 Tombol + LED) Siap!");
}

void loop() {
  bool stateChanged = false;
  int currentPressed[NUM_BUTTONS];

  // Cek apakah "VOXCADE Controller" terhubung ke Bluetooth PC/HP
  if (bleGamepad.isConnected()) {
    // Bluetooth Terhubung -> Nyalakan LED Hijau, Matikan LED Merah
    digitalWrite(LED_HIJAU, HIGH);
    digitalWrite(LED_MERAH, LOW);
  } else {
    // Bluetooth Belum Terhubung / Terputus -> Matikan LED Hijau, Nyalakan LED Merah
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_MERAH, HIGH);
  }

  // Baca status ke-4 tombol (berlaku untuk Bluetooth BLE & Kabel USB Serial)
  for (int i = 0; i < NUM_BUTTONS; i++) {
    int state = digitalRead(buttonPins[i]);
    currentPressed[i] = (state == LOW) ? 1 : 0;

    if (state != prevState[i]) {
      stateChanged = true;
      prevState[i] = state;
    }

    // Kirim input via Bluetooth jika sedang terhubung
    if (bleGamepad.isConnected()) {
      if (state == LOW) {
        bleGamepad.press(buttons[i]);
      } else {
        bleGamepad.release(buttons[i]);
      }
    }
  }

  // Kirim juga status tombol ke USB Serial (agar mode kabel USB di website VOXCADE tetap jalan)
  if (stateChanged) {
    Serial.print("B:");
    Serial.print(currentPressed[0]); Serial.print(",");
    Serial.print(currentPressed[1]); Serial.print(",");
    Serial.print(currentPressed[2]); Serial.print(",");
    Serial.println(currentPressed[3]);
  }

  delay(10);
}