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
| **Noise Tolerance** | Dihitung dari max **9999** (bukan 1023) |

## Port & Serial yang Dipakai

| Port | Fungsi | Baudrate | Keterangan |
|------|--------|----------|------------|
| **Serial** (USB) | Komunikasi dengan software **SSP2000** | 9600 | Port utama (sama seperti R3) |
| **Serial1** | Cadangan / alternatif | 9600 | Hardware serial kedua di R4 |
| **WiFi AP** | Web UI + WebSocket | - | SSID: `SideSlip-R4` |
| **Pin A5** | Sensor side slip (analog) | - | ADC 14-bit → mapping 0–9999 |

### Protokol Serial SSP2000

- Arduino **menunggu** karakter **'R'** dari software.
- Setelah terima **'R'**, Arduino kirim data ADC dalam format: `*XXXX#`
- Contoh: `*0512#`, `*5000#`, `*9999#`
- Data yang dikirim adalah nilai **setelah mapping** (0–9999) + filter.

## Cara Pengukuran (Alur Lengkap)

### 1. Mode Serial (SSP2000)
```
Software SSP2000  --kirim 'R'-->  Arduino
Arduino           --balas *XXXX#-->  Software SSP2000
```
- Software yang hitung nilai mm/m berdasarkan kalibrasi yang tersimpan di file text.
- Arduino hanya bertugas mengirim data ADC mentah (yang sudah difilter).

### 2. Mode Web UI (Standalone)
```
Browser  --login-->  Arduino
Arduino  --kirim data JSON via WebSocket-->  Browser
Browser  --command (kalibrasi/config)-->  Arduino
```
- Semua perhitungan mm/m dilakukan di Arduino.
- Kalibrasi & settings disimpan di EEPROM.

### 3. Alur Kalibrasi (Web UI)
1. Login sebagai **ID:1** atau **sa**.
2. Buka halaman `/kalibrasi`.
3. Plat dalam posisi **nol** → tekan **"Set Zero"** → simpan Zero ADC.
4. Geser plat **10 mm/m** (10 mm jika plat panjang 1 m) → tekan **"Set High"** → simpan High ADC.
5. Tekan **"SAVE"** → hitung koefisien → simpan ke EEPROM.
6. Format simpan: `*KoefZZeroHHigh#`

### 4. Alur Pengukuran (Web UI)
1. Login (ID:1, ID:2, atau sa).
2. Halaman utama menampilkan nilai real-time.
3. Saat pengukuran:
   - Tunggu stabil sesuai **Wait Time Measure** (default 3 detik).
   - Abaikan perubahan kecil sesuai **Noise Tolerance** (default 0.3% dari 9999).
   - Ambil nilai maksimal dalam jendela waktu tersebut.
4. Hasil ditampilkan dalam **mm/m**.

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

## Arsitektur Solusi

### Dual Interface

```
[Sensors A5] → [Arduino Uno R4 WiFi]
                    ├── USB Serial (Serial) → SSP2000 (protokol lama)
                    ├── Serial1 (cadangan)
                    └── WiFi AP + WebSocket + Web Server → Browser
                          ├── /          → Halaman Utama (dengan Login)
                          ├── /kalibrasi → Kalibrasi (hanya ID1 / sa)
                          ├── /config    → Config (hanya ID1 / sa)
                          └── /hasil     → Hasil (hanya ID1 / sa)
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
- **High ADC**: nilai saat plat digeser **10 mm/m**
- Koefisien dihitung dari selisih High - Zero

### Settings (disimpan di EEPROM)
Contoh dari `SideSlipSettings.txt`:
```
*0.3T3#
```
- Noise Zero Tolerance: `0.3` (%) → dihitung dari max **9999**
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

## Yang Sudah Ada di Kerangka Kode

File: `sketch_BBM_SideSlip_UNOR4WIFI.V4.0/sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino`

- Sensor reading + mapping 14-bit → 0–9999
- Filter eksponensial
- Serial protocol SSP2000 (Serial + Serial1)
- Struktur login & variabel kalibrasi/settings
- Placeholder EEPROM, Web Server, WebSocket

## Yang Masih Kurang Sebelum Coding Lengkap

| No | Bagian | Status | Keterangan |
|----|--------|--------|------------|
| 1 | **EEPROM** | Belum | Load/save kalibrasi, settings, akun ID1 & ID2 |
| 2 | **Web Server** | Belum | 4 halaman HTML (Utama, Kalibrasi, Config, Hasil) |
| 3 | **WebSocket** | Belum | Streaming data real-time + terima command |
| 4 | **Login System** | Belum | Validasi ID/PW, session, kontrol akses halaman |
| 5 | **Halaman Kalibrasi** | Belum | Set Zero, Set High (10 mm/m), SAVE ke EEPROM |
| 6 | **Halaman Config** | Belum | Ubah noise tolerance, wait time, ubah akun |
| 7 | **Halaman Hasil** | Belum | Tampilkan hasil pengujian terakhir |
| 8 | **Perhitungan mm/m** | Belum | Fungsi hitung side slip dari kalibrasi |
| 9 | **Noise & Wait Time** | Belum | Logika stabilitas pengukuran (seperti di VB) |
| 10 | **Testing Dual Mode** | Belum | Uji Serial SSP2000 + WebSocket bersamaan |

## Langkah Kerja Selanjutnya

1. Implementasi **EEPROM** (paling dasar).
2. Buat **WebSocket** + halaman utama dengan login.
3. Tambah halaman **Kalibrasi**, **Config**, dan **Hasil**.
4. Integrasi perhitungan mm/m + logika pengukuran.
5. Testing dual mode (Serial + WebSocket).

---

**Status:** Kerangka kode sudah ada. Penjelasan port serial dan alur pengukuran sudah dilengkapi.  
Siap dilanjutkan ke implementasi bagian yang masih kurang di atas.
