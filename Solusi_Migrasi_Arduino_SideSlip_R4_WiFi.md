# Solusi Migrasi Arduino SideSlip ke Uno R4 WiFi (V4.0)

## Tujuan
Migrasi dari Arduino Uno R3 (`sketch_BBM_SideSlip.V1.2.ino`) ke **Arduino Uno R4 WiFi**.

**Persyaratan:**
- Tetap membaca sensor data mentah (raw sensor data) sama seperti versi R3.
- **Kompatibel dengan software SSP2000** (protokol serial yang sudah berjalan normal tetap didukung via USB Serial).
- Menambahkan **WebSocket + Web UI** dengan struktur mirip SSP2000:
  - **Halaman Utama** (Main)
  - **Kalibrasi**
  - **Config**
  - **Hasil**
- Semua sudah include di board R4 WiFi (WiFi onboard).

## Keputusan Desain (Sudah Dikonfirmasi)

| Aspek | Keputusan |
|-------|-----------|
| **WiFi** | **AP Mode** dari Arduino R4 sendiri (connect langsung ke SSID Arduino) |
| **ADC Resolusi** | **14-bit** dibaca, lalu **dimapping ke 0–9999** agar tetap 4 digit |
| **Kalibrasi** | Sama seperti SSP2000. Zero + High. High = **10 mm/m** |
| **Penyimpanan** | **EEPROM** (ganti file text di SSP2000) |
| **Serial** | Buka 2 Serial hardware di R4 (tidak perlu SoftwareSerial) |

## Sistem Login (Halaman Utama)

Login wajib di halaman utama sebelum mengakses fitur lain.

| Akun | ID (Default) | Password (Default) | Role | Akses | Penyimpanan |
|------|--------------|---------------------|------|-------|-------------|
| **Kepala** | `1` | `1` | Kepala | Semua akses + bisa ubah ID/PW sendiri dan ID:2 | EEPROM |
| **Penguji** | `2` | `2` | Penguji | Hanya pengukuran / realtime | EEPROM |
| **Admin** | `sa` | `@abcd1234` | Admin | Full admin | **Hardcoded** (tidak disimpan di EEPROM) |

### Aturan Login
- ID:1 (Kepala) bisa mengubah:
  - ID & password miliknya sendiri
  - ID & password ID:2 (Penguji)
- ID:sa tidak bisa diubah lewat UI dan tidak disimpan di EEPROM.
- Setelah login, session disimpan di browser (localStorage / session).
- Halaman Kalibrasi, Config, dan Hasil hanya bisa diakses oleh ID:1 atau ID:sa.

### Command WebSocket untuk Login & Akun

```json
{ "cmd": "login", "id": "1", "pw": "1" }
{ "cmd": "change_account", "target": "1", "new_id": "kepala", "new_pw": "baru123" }
{ "cmd": "change_account", "target": "2", "new_id": "penguji", "new_pw": "uji456" }
```

## Arsitektur Solusi

### Dual Interface

```
[Sensors A5] → [Arduino Uno R4 WiFi]
                    ├── USB Serial (Serial) → SSP2000 (protokol lama)
                    ├── Serial1 (opsional)
                    └── WiFi AP + WebSocket + Web Server → Browser
                          ├── /          → Halaman Utama (dengan Login)
                          ├── /kalibrasi → Kalibrasi (hanya ID1 / sa)
                          ├── /config    → Config (hanya ID1 / sa)
                          └── /hasil     → Hasil (hanya ID1 / sa)
```

## Protokol Serial yang Sudah Ada (V1.2) — Harus Dipertahankan

Dari `sketch_BBM_SideSlip.V1.2.ino`:

- Sensor di pin **A5**
- Resolusi ADC di R4: **14-bit** dibaca, lalu dimapping ke **0–9999**
- Filter eksponensial (`alpha = 0.8`)
- Saat menerima karakter **'R'** (ASCII 82):
  - Kirim data dalam format: `*XXXX#` (4 digit ADC)
  - Contoh: `*0512#`, `*5000#`, `*9999#`
- Baudrate: 9600

### Cara Mapping ADC 14-bit → 4 Digit

```cpp
analogReadResolution(14);               // baca 0 - 16383
int raw14 = analogRead(A5);
int adc4digit = map(raw14, 0, 16383, 0, 9999);
// hasil: 0 - 9999 → tetap muat di *XXXX#
```

## Format Kalibrasi & Settings (dari SSP2000)

### Kalibrasi (disimpan di EEPROM)
Contoh dari `SideSlipCalibration.txt`:
```
*-511Z511H0#
```
Format di kode VB:
```
*Koef_ADC Z Zero_ADC H High_ADC #
```
- **Zero ADC**: nilai saat posisi nol (plat diam) — dalam range 0–9999
- **High ADC**: nilai saat plat digeser **10 mm/m** (geser 10 mm jika panjang plat 1 m)
- Koefisien dihitung dari selisih High - Zero

### Settings (disimpan di EEPROM)
Contoh dari `SideSlipSettings.txt`:
```
*0.3T3#
```
- Noise Zero Tolerance: `0.3` (%)
- Wait Time Measure: `3` (detik)

### Akun (disimpan di EEPROM)
- ID:1 + Password:1 (Kepala)
- ID:2 + Password:2 (Penguji)

## Struktur Web UI (Mirip SSP2000)

| Halaman | Path | Fungsi Utama | Akses |
|---------|------|--------------|-------|
| **Halaman Utama** | `/` | Login + Tampilan real-time nilai side slip, status sensor, grafik live | Semua (setelah login) |
| **Kalibrasi** | `/kalibrasi` | Set Zero ADC + High ADC (10 mm/m), simpan ke EEPROM | ID:1 / sa |
| **Config** | `/config` | Noise tolerance, wait time, ubah akun | ID:1 / sa |
| **Hasil** | `/hasil` | Hasil pengujian terakhir | ID:1 / sa |

### Komunikasi WebSocket (JSON)

**Data dari Arduino → Browser:**
```json
{
  "type": "data",
  "raw": 512,
  "value": 1.25,
  "unit": "mm/m",
  "calibrated": true,
  "status": "ok"
}
```

**Command dari Browser → Arduino:**
```json
{ "cmd": "login", "id": "1", "pw": "1" }
{ "cmd": "calibrate_zero" }
{ "cmd": "calibrate_high" }
{ "cmd": "save_calibration" }
{ "cmd": "save_settings", "noise": 0.3, "wait_time": 3 }
{ "cmd": "change_account", "target": "2", "new_id": "penguji", "new_pw": "uji456" }
{ "cmd": "get_result" }
```

## Rencana Implementasi Firmware

### Struktur Sketch

```
sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino
├── Sensor Reading (port dari V1.2, pin A5, 14-bit → map 0-9999)
├── Serial Protocol Handler (SSP2000 - tetap sama, 4 digit)
├── WiFi AP Mode
├── Web Server (4 halaman)
├── WebSocket Server
└── EEPROM Storage (kalibrasi + settings + akun ID1 & ID2)
```

### Alur Kalibrasi (mengikuti SSP2000)

1. Login sebagai ID:1 atau sa.
2. Buka halaman `/kalibrasi`.
3. Posisi nol → tekan "Set Zero" → simpan Zero ADC (0–9999).
4. Geser plat **10 mm/m** → tekan "Set High" → simpan High ADC.
5. Tekan "SAVE" → hitung koefisien → simpan ke EEPROM dalam format `*KoefZZeroHHigh#`.
6. Semua halaman berikutnya menggunakan nilai kalibrasi dari EEPROM.

## Library yang Dibutuhkan

- `WiFiS3` (bawaan R4 WiFi)
- `EEPROM` (bawaan)
- `ArduinoJson` (recommended)

## Langkah Kerja Selanjutnya

1. Porting sensor + serial protocol dari V1.2 (uji dengan SSP2000).
2. Buat WiFi AP + WebSocket dasar + halaman utama dengan login.
3. Tambah halaman Kalibrasi, Config, dan Hasil (dengan kontrol akses).
4. Integrasi penyimpanan EEPROM untuk kalibrasi, settings, dan akun.
5. Testing dual mode (Serial + WebSocket bersamaan).

---

**Status:** Detail desain sudah lengkap termasuk mapping ADC 14-bit → 0–9999 dan kalibrasi 10 mm/m.  
Siap dilanjutkan ke tahap coding `sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino`.
