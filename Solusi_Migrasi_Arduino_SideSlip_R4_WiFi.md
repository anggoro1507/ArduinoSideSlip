# Solusi Migrasi Arduino SideSlip ke Uno R4 WiFi (V4.0)

## Tujuan
Migrasi dari Arduino Uno R3 ke **Arduino Uno R4 WiFi**.

**Persyaratan:**
- Tetap membaca sensor data mentah (A5)
- **Kompatibel dengan software SSP2000** (protokol serial)
- Web UI dengan struktur mirip SSP2000
- Semua include di board R4 WiFi

## Keputusan Desain

| Aspek | Keputusan |
|-------|-----------|
| **WiFi** | AP Mode, SSID: `SideSlipBBM`, Password: `12345678` |
| **ADC** | 14-bit → mapping 0–9999 |
| **Kalibrasi** | Simpan koefisien (Zero + High), High = 10 mm/m |
| **Penyimpanan** | EEPROM |
| **Serial** | Serial (USB) + Serial1, 9600 baud |
| **Noise Tolerance** | % dari 9999 |
| **Magic Bytes** | `0xB2C0` |
| **Data Rate** | Baca 37ms, Measure 70ms |

## Status Implementasi

### Sudah Selesai

| Fitur | Status | Keterangan |
|-------|--------|------------|
| **EEPROM** | ✅ | Load/save kalibrasi, settings, akun. Address map fixed untuk R4 (int 4-byte) |
| **Web Server** | ✅ | Port 80, routing 4 halaman |
| **HTTP Polling** | ✅ | `/data` setiap 200ms (WebSocket tidak stabil di R4) |
| **Login UI** | ✅ | Halaman login (belum validasi ke Arduino) |
| **Kalibrasi** | ✅ | UI + save/load EEPROM, auto-zero, tombol ZERO |
| **Config** | ✅ | UI + save/load settings |
| **Logika Pengukuran** | ✅ | Zero dinamis, peak plus/minus, stabil detection |
| **Status Display** | ✅ | ZEROED / MEASURING / RESULT |
| **Serial SSP2000** | ✅ | Protokol `*XXXX#` setelah terima 'R' |

### Belum Selesai

| No | Fitur | Keterangan |
|----|-------|------------|
| 1 | **Login System** | Validasi ID/PW ke Arduino |
| 2 | **Ubah Akun** | ID1 bisa ganti ID1/ID2 |
| 3 | **Session/Timeout** | Auto-logout setelah idle |
| 4 | **Testing Dual Mode** | Uji Serial + Web bersamaan |

## File Struktur

```
sketch_BBM_SideSlip_UNOR4WIFI.V4.0/
├── sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino  ← Main: sensor, measure, EEPROM
├── web_server.cpp / .h                      ← HTTP server + endpoints
├── html_index.h                             ← Login page
├── html_pengujian.h                         ← Measurement page
├── html_kalibrasi.h                         ← Calibration page
├── html_config.h                            ← Config page
└── web_common.h                             ← CSS
```

## Endpoint API

| Endpoint | Method | Fungsi |
|----------|--------|--------|
| `/data` | GET | Data realtime (adc, value, max, result, zeroed) |
| `/cal/get` | GET | Load kalibrasi (zero, high, koef) |
| `/cal/save?zero=X&high=Y` | GET | Simpan kalibrasi |
| `/settings/get` | GET | Load settings (noise, wait) |
| `/settings/save?noise=X&wait=Y` | GET | Simpan settings |
| `/reset` | GET | Manual zero |

## Logika Pengukuran (Match VB)

1. **Zero dinamis** — ADC stabil saat ini = zero
2. **Deteksi pergerakan** — jika > noise threshold
3. **Track peak** — High_ADC (plus) atau Low_ADC (minus)
4. **Stabil 2.5 detik** — mulai countdown
5. **Wait time** — jika stabil, hitung result dan auto-zero

## Cara Pengukuran

```
Zero awal = ADC stabil
Mobil lewat → track peak (plus/minus)
Stabil → tampilkan peak, zero baru = ADC stabil
Siap untuk mobil berikutnya
```

## Langkah Selanjutnya

1. **Login system** — validasi ID/PW
2. **Ubah akun** — ID1 ganti ID1/ID2
3. **Testing** — uji dengan mobil nyata

---

**Status:** Core measurement sudah jalan. Siap lanjut ke login system.
