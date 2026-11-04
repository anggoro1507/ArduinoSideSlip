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
  - **Pengujian**
- Semua sudah include di board R4 WiFi (WiFi onboard).

## Keputusan Desain (Sudah Dikonfirmasi)

| Aspek | Keputusan |
|-------|-----------|
| **WiFi** | **AP Mode** dari Arduino R4 sendiri (connect langsung ke SSID Arduino) |
| **SSID** | `SideSlipBBM` |
| **Password WiFi** | `12345678` |
| **ADC Resolusi** | **14-bit** dibaca, lalu **dimapping ke 0–9999** agar tetap 4 digit |
| **Kalibrasi** | Simpan **koefisien** (dari Zero + High). High = **10 mm/m** |
| **Penyimpanan** | **EEPROM** (ganti file text di SSP2000) |
| **Serial** | Buka 2 Serial hardware di R4 (tidak perlu SoftwareSerial) |
| **Noise Tolerance** | Dihitung dari max **9999** (bukan 1023) |
| **Magic Bytes** | `0xB2C0` |

## Port & Serial yang Dipakai

| Port | Fungsi | Baudrate | Keterangan |
|------|--------|----------|------------|
| **Serial** (USB) | Komunikasi dengan software **SSP2000** | 9600 | Port utama (sama seperti R3) |
| **Serial1** | Cadangan / alternatif | 9600 | Hardware serial kedua di R4 |
| **WiFi AP** | Web UI + WebSocket | - | SSID: `SideSlipBBM` / Password: `12345678` |
| **Pin A5** | Sensor side slip (analog) | - | ADC 14-bit → mapping 0–9999 |

### Protokol Serial SSP2000

- Arduino **menunggu** karakter **'R'** dari software.
- Setelah terima **'R'**, Arduino kirim data ADC dalam format: `*XXXX#`
- Contoh: `*0512#`, `*5000#`, `*9999#`
- Data yang dikirim adalah nilai **setelah mapping** (0–9999) + filter.

## Sistem Login (Halaman Utama)

Login wajib di halaman utama sebelum mengakses fitur lain.

| Akun | ID (Default) | Password (Default) | Role | Akses | Penyimpanan |
|------|--------------|---------------------|------|-------|-------------|
| **Kepala** | `1` | `1` | Kepala | Semua akses + bisa ubah ID/PW sendiri dan ID:2 | EEPROM |
| **Penguji** | `2` | `2` | Penguji | **Bisa melakukan pengujian / realtime** | EEPROM |
| **Admin** | `sa` | `@abcd1234` | Admin | Full admin | **Hardcoded** (tidak disimpan di EEPROM) |

### Aturan Login
- **ID:2 (Penguji) bisa melakukan pengujian** (halaman utama / realtime).
- ID:1 (Kepala) bisa mengubah:
  - ID & password miliknya sendiri
  - ID & password ID:2 (Penguji)
- ID:sa tidak bisa diubah lewat UI dan tidak disimpan di EEPROM.
- Halaman **Kalibrasi** dan **Config** hanya bisa diakses oleh ID:1 atau sa.
- Setelah login, session disimpan di browser (localStorage / session).

## Cara Pengukuran / Pengujian (Alur yang Benar)

### Konsep Utama
- Yang diambil dari **kalibrasi** adalah **koefisien** (bukan zero tetap).
- **Zero bersifat dinamis** (otomatis diupdate saat pengujian).
- Selisih posisi plat saat berhenti dengan zero yang tersimpan **tidak masalah**.
- Nilai yang dicatat adalah **deviasi terbesar** dari zero (bisa **plus** atau **minus**).

### Alur Pengujian

1. **Alat aktif** → ambil nilai ADC saat ini sebagai **zero** (otomatis).
2. **Mobil lewat plat** → terjadi pergerakan (nilai ADC naik atau turun).
3. Sistem mencatat **deviasi terbesar** dari zero:
   - Jika plat geser ke **kanan** → nilai **plus** terbesar.
   - Jika plat geser ke **kiri** → nilai **minus** terbesar (paling kecil).
4. Setelah mobil lewat, nilai raw kembali ke posisi stabil.
5. Jika tidak ada perubahan selama **Wait Time Measure** (default 3 detik) dalam batas **Noise Tolerance** (default 0.3% dari 9999):
   - Nilai ADC yang stabil itu menjadi **zero baru**.
6. **Display menampilkan nilai deviasi terbesar** (dalam mm/m, dihitung dengan koefisien). Bisa positif (+) atau negatif (-).
7. Jika ada mobil berikutnya lewat, proses otomatis diulang.

### Contoh Sederhana

**Plat geser ke kanan (plus):**
```
Zero awal     = 5000
Mobil lewat   = nilai naik sampai 6500  → Deviasi terbesar = +1500
Stabil 3 detik di 5100 → Zero baru = 5100
Display       = +1.5 mm/m (contoh)
```

**Plat geser ke kiri (minus):**
```
Zero awal     = 5000
Mobil lewat   = nilai turun sampai 3500  → Deviasi terbesar = -1500
Stabil 3 detik di 4900 → Zero baru = 4900
Display       = -1.5 mm/m (contoh)
```

> **Penting:** Jangan hanya ambil nilai absolut (MAX). Harus mendukung nilai **minus** agar arah side slip benar.

## Arsitektur Solusi

### Dual Interface

```
[Sensors A5] → [Arduino Uno R4 WiFi]
                    ├── USB Serial (Serial) → SSP2000 (protokol lama)
                    ├── Serial1 (cadangan)
                    └── WiFi AP + WebSocket + Web Server → Browser
                          ├── /            → Halaman Utama (login)
                          ├── /pengujian   → Pengujian real-time (semua akun)
                          ├── /kalibrasi   → Kalibrasi (hanya ID1 / sa)
                          └── /config      → Config (hanya ID1 / sa)
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
- Yang dipakai saat pengujian adalah **Koef_ADC** (koefisien).
- Zero ADC dan High ADC hanya untuk menghitung koefisien.
- High = posisi **10 mm/m**.

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
| **Halaman Utama** | `/` | Login | Semua |
| **Pengujian** | `/pengujian` | Pengujian real-time (zero dinamis + deviasi plus/minus) | Semua akun |
| **Kalibrasi** | `/kalibrasi` | Set Zero + High (10 mm/m) untuk hitung koefisien, simpan ke EEPROM | ID:1 / sa |
| **Config** | `/config` | Noise tolerance, wait time, ubah akun | ID:1 / sa |

### Komunikasi WebSocket (JSON)

**Data dari Arduino → Browser:**
```json
{
  "type": "data",
  "raw": 512,
  "value": -1.25,
  "unit": "mm/m",
  "peak_value": -2.5,
  "status": "measuring"
}
```

**Command dari Browser → Arduino:**
```json
{ "cmd": "login", "id": "2", "pw": "2" }
{ "cmd": "calibrate_zero" }
{ "cmd": "calibrate_high" }
{ "cmd": "save_calibration" }
{ "cmd": "save_settings", "noise": 0.3, "wait_time": 3 }
{ "cmd": "change_account", "target": "2", "new_id": "penguji", "new_pw": "uji456" }
```

## Yang Sudah Ada di Kerangka Kode

File: `sketch_BBM_SideSlip_UNOR4WIFI.V4.0/sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino`

- Sensor reading + mapping 14-bit → 0–9999
- Filter eksponensial
- Serial protocol SSP2000 (Serial + Serial1)
- Struktur login & variabel kalibrasi/settings
- EEPROM load/save (magic `0xB2C0`)
- WiFi AP: `SideSlipBBM` / `12345678`

## Yang Masih Kurang Sebelum Coding Lengkap

| No | Bagian | Status | Keterangan |
|----|--------|--------|------------|
| 1 | **EEPROM** | Sudah | Load/save kalibrasi, settings, akun ID1 & ID2 |
| 2 | **Web Server** | Belum | 4 halaman HTML |
| 3 | **WebSocket** | Belum | Streaming data real-time + terima command |
| 4 | **Login System** | Belum | Validasi ID/PW + kontrol akses |
| 5 | **Halaman Kalibrasi** | Belum | Set Zero + High untuk hitung koefisien |
| 6 | **Halaman Config** | Belum | Ubah noise tolerance, wait time, ubah akun |
| 7 | **Logika Pengujian** | Belum | Zero dinamis + catat deviasi terbesar (plus/minus) + deteksi stabil 3 detik |
| 8 | **Perhitungan mm/m** | Belum | Hitung dari koefisien + nilai peak (bisa negatif) |
| 9 | **Testing Dual Mode** | Belum | Uji Serial SSP2000 + WebSocket bersamaan |

## Langkah Kerja Selanjutnya

1. Buat **WebSocket** + halaman utama dengan login.
2. Tambah logika **pengujian** (zero dinamis + peak plus/minus + stabil).
3. Tambah halaman **Kalibrasi** dan **Config**.
4. Testing dual mode (Serial + WebSocket).

---

**Status:** SSID WiFi dan magic bytes sudah disesuaikan dengan kode `.ino`.  
Siap dilanjutkan ke implementasi.
