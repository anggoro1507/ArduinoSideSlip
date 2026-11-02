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
| **Kalibrasi** | Sama seperti SSP2000 (PDF + kode VB). Simpan Zero ADC + High ADC |
| **Penyimpanan** | **EEPROM** (ganti file text di SSP2000) |
| **Serial** | Buka 2 Serial hardware di R4 (tidak perlu SoftwareSerial) |

## Arsitektur Solusi

### Dual Interface

```
[Sensors A5] → [Arduino Uno R4 WiFi]
                    ├── USB Serial (Serial) → SSP2000 (protokol lama)
                    ├── Serial1 (opsional)
                    └── WiFi AP + WebSocket + Web Server → Browser
                          ├── /          → Halaman Utama
                          ├── /kalibrasi → Kalibrasi
                          ├── /config    → Config
                          └── /hasil     → Hasil
```

## Protokol Serial yang Sudah Ada (V1.2) — Harus Dipertahankan

Dari `sketch_BBM_SideSlip.V1.2.ino`:

- Sensor di pin **A5**
- Filter eksponensial (`alpha = 0.8`)
- Saat menerima karakter **'R'** (ASCII 82):
  - Kirim data dalam format: `*XXXX#` (4 digit ADC)
  - Contoh: `*0512#`, `*0102#`, `*0009#`
- Baudrate: 9600

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
- Zero ADC: nilai saat posisi nol
- High ADC: nilai saat posisi tinggi (untuk hitung koefisien)

### Settings (disimpan di EEPROM)
Contoh dari `SideSlipSettings.txt`:
```
*0.3T3#
```
- Noise Zero Tolerance: `0.3` (%)
- Wait Time Measure: `3` (detik)

## Struktur Web UI (Mirip SSP2000)

| Halaman | Path | Fungsi Utama |
|---------|------|--------------|
| **Halaman Utama** | `/` | Tampilan real-time nilai side slip, status sensor, grafik live |
| **Kalibrasi** | `/kalibrasi` | Set Zero ADC + High ADC, simpan ke EEPROM |
| **Config** | `/config` | Noise tolerance, wait time, dll (simpan ke EEPROM) |
| **Hasil** | `/hasil` | Hasil pengujian terakhir |

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
{ "cmd": "calibrate_zero" }
{ "cmd": "calibrate_high" }
{ "cmd": "save_calibration" }
{ "cmd": "save_settings", "noise": 0.3, "wait_time": 3 }
{ "cmd": "get_result" }
```

## Rencana Implementasi Firmware

### Struktur Sketch

```
sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino
├── Sensor Reading (port dari V1.2, pin A5)
├── Serial Protocol Handler (SSP2000 - tetap sama)
├── WiFi AP Mode
├── Web Server (4 halaman)
├── WebSocket Server
└── EEPROM Storage (kalibrasi + settings)
```

### Alur Kalibrasi (mengikuti SSP2000)

1. User buka halaman `/kalibrasi`.
2. Posisi nol → tekan "Set Zero" → simpan Zero ADC.
3. Posisi tinggi → tekan "Set High" → simpan High ADC.
4. Tekan "SAVE" → hitung koefisien → simpan ke EEPROM dalam format `*KoefZZeroHHigh#`.
5. Semua halaman berikutnya menggunakan nilai kalibrasi dari EEPROM.

## Library yang Dibutuhkan

- `WiFiS3` (bawaan R4 WiFi)
- `EEPROM` (bawaan)
- `ArduinoJson` (recommended)

## Langkah Kerja Selanjutnya

1. Porting sensor + serial protocol dari V1.2 (uji dengan SSP2000).
2. Buat WiFi AP + WebSocket dasar + halaman utama.
3. Tambah halaman Kalibrasi, Config, dan Hasil.
4. Integrasi penyimpanan EEPROM untuk kalibrasi & settings.
5. Testing dual mode (Serial + WebSocket bersamaan).

---

**Status:** Detail desain sudah lengkap berdasarkan input user.  
Siap dilanjutkan ke tahap coding `sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino`.
