[readme (1).md](https://github.com/user-attachments/files/35091680/readme.1.md)
# VOXCADE Controller — ESP32 Retro Gamepad

Firmware *controller* retro 4 tombol berbasis **ESP32** yang mendukung mode **Bluetooth Low Energy (BLE HID Gamepad)** sekaligus **USB Serial**, dilengkapi indikator LED status koneksi. Perangkat ini dirancang sebagai pengendali fisik untuk game retro **VOXCADE**.

## Fitur Utama

- **Dual Mode (BLE Gamepad & USB Serial):** Mengirim input tombol secara nirkabel melalui Bluetooth dengan nama **`VOXCADE Controller`**, sekaligus mengirim status tombol ke port USB Serial (`115200 baud`) agar tetap bisa digunakan melalui kabel USB.
- **4 Tombol Aksi Retro:** Menggunakan mode `INPUT_PULLUP` internal ESP32 sehingga tombol cukup dihubungkan langsung ke pin GPIO dan `GND`.
- **Indikator LED Status Koneksi:**
  - **LED Merah menyala:** Bluetooth belum terhubung atau terputus.
  - **LED Hijau menyala:** Bluetooth terhubung ke PC atau ponsel.
- **Respons Cepat:** Pembacaan tombol dilakukan setiap `10 ms` dan pengiriman serial hanya dipicu saat ada perubahan status tombol.

## Pemetaan Pin ESP32

### Tombol (*Push Button*)

Setiap tombol dihubungkan antara pin GPIO di bawah ini dan **`GND`** (*active LOW* dengan `INPUT_PULLUP`):

| Komponen | Pin ESP32 | Pemetaan HID BLE | Fungsi di Game |
| :--- | :---: | :---: | :--- |
| **Tombol 1** (`BUTTON_PIN1`) | GPIO 12 | `BUTTON_1` | Gerak Kiri |
| **Tombol 2** (`BUTTON_PIN2`) | GPIO 14 | `BUTTON_2` | Gerak Kanan |
| **Tombol 3** (`BUTTON_PIN3`) | GPIO 27 | `BUTTON_4` | Lompat / *Jump* |
| **Tombol 4** (`BUTTON_PIN4`) | GPIO 26 | `BUTTON_5` | *Dash* / *Action* |

### Indikator LED

Hubungkan anoda LED (`+`) ke pin GPIO melalui resistor pembatas arus dan katoda (`-`) ke **`GND`**:

| Indikator | Pin ESP32 | Keterangan |
| :--- | :---: | :--- |
| **LED Hijau** (`LED_HIJAU`) | GPIO 25 | Menyala saat Bluetooth terhubung |
| **LED Merah** (`LED_MERAH`) | GPIO 33 | Menyala saat Bluetooth belum terhubung / terputus |

## Format Data USB Serial

Saat menggunakan koneksi kabel USB dengan *baud rate* **`115200`**, ESP32 mengirim baris status setiap kali salah satu tombol ditekan atau dilepas dengan format:

```text
B:<tombol1>,<tombol2>,<tombol3>,<tombol4>
```

- Nilai `1` berarti tombol sedang ditekan, dan `0` berarti dilepas.
- Contoh: `B:1,0,1,0` menandakan **Tombol 1 (Kiri)** dan **Tombol 3 (Lompat)** sedang ditekan bersamaan.

## Prasyarat Perangkat Lunak

- **Arduino IDE** dengan paket board **ESP32** terpasang.
- Library **`BleGamepad`** (*ESP32-BLE-Gamepad*) beserta dependensinya (*NimBLE-Arduino*).

## Cara Upload & Penggunaan

1. Buka file `voxcade.ino` di **Arduino IDE**.
2. Pastikan library **ESP32-BLE-Gamepad** sudah terpasang melalui *Library Manager* atau instalasi manual.
3. Pilih board **ESP32** yang sesuai dan pilih **Port COM** yang terdeteksi.
4. Klik **Upload** untuk mengunggah firmware ke ESP32.
5. Setelah selesai, **LED Merah** akan menyala menandakan perangkat siap dipasangkan.
6. **Mode Bluetooth:** Cari dan hubungkan perangkat bernama **`VOXCADE Controller`** dari menu Bluetooth PC atau HP hingga **LED Hijau** menyala.
7. **Mode Kabel USB:** Biarkan ESP32 terhubung ke komputer dan baca port serial pada **`115200 baud`** melalui aplikasi atau website VOXCADE.

## Struktur Proyek

```text
Controler button retro/
└── voxcade.ino    # Firmware utama ESP32 (BLE Gamepad, USB Serial, & indikator LED)
```
