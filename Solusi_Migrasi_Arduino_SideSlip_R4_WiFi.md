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

## Arsitektur Solusi

### Dual Interface

```
[Sensors] → [Arduino Uno R4 WiFi]
                ├── USB Serial → SSP2000 (protokol lama, tetap jalan)
                └── WiFi WebSocket + Web Server → Browser
                      ├── /          → Halaman Utama
                      ├── /kalibrasi → Kalibrasi
                      ├── /config    → Config
                      └── /hasil     → Hasil
```

**Keuntungan:**
- SSP2000 tetap bisa dipakai seperti biasa (plug USB).
- Bisa dipakai standalone via browser dengan tampilan mirip SSP2000.
- Tidak perlu aplikasi tambahan.

## Struktur Web UI (Mirip SSP2000)

| Halaman       | Path         | Fungsi Utama |
|---------------|--------------|--------------|
| **Halaman Utama** | `/`         | Tampilan real-time nilai side slip, status sensor, grafik live |
| **Kalibrasi**     | `/kalibrasi`| Proses kalibrasi zero/offset, simpan nilai kalibrasi |
| **Config**        | `/config`   | Pengaturan WiFi, interval sampling, unit, threshold, dll |
| **Hasil**         | `/hasil`    | Riwayat pengujian, hasil terakhir, export/print sederhana |

### Komunikasi WebSocket

Semua halaman menggunakan **satu WebSocket connection**. Perintah dan data dikirim dalam format JSON.

#### Contoh Message dari Arduino → Browser

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

```json
{
  "type": "status",
  "wifi": "connected",
  "mode": "dual",
  "ip": "192.168.4.1"
}
```

#### Contoh Command dari Browser → Arduino

```json
{ "cmd": "calibrate_zero" }
{ "cmd": "save_config", "interval": 50, "unit": "mm/m" }
{ "cmd": "get_result" }
{ "cmd": "reset_result" }
```

## Protokol Serial yang Sudah Ada (V1.2)

Dari `sketch_BBM_SideSlip.V1.2.ino` dan `README.md` / `SSP2000/Protokol.txt`:

- Sensor di pin **A5**
- Filter eksponensial (`alpha = 0.8`)
- Saat menerima karakter **'R'** (ASCII 82) via Serial atau SoftwareSerial:
  - Kirim data dalam format: `*XXXX#` (4 digit ADC)
  - Contoh: `*0512#`, `*0102#`, `*0009#`
- Baudrate: 9600

**Ini harus tetap dipertahankan** di versi R4 WiFi agar kompatibel dengan SSP2000.

## Rencana Implementasi Firmware

### Struktur Sketch

```
sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino
├── Sensor Reading (port dari V1.2)
├── Serial Protocol Handler (SSP2000 - tetap sama)
├── WiFi Manager (AP / STA)
├── Web Server
│   ├── Halaman Utama
│   ├── Kalibrasi
│   ├── Config
│   └── Hasil
├── WebSocket Server (command + data stream)
└── Storage (EEPROM / Preferences untuk kalibrasi & config)
```

### Komponen Utama

1. **Sensor Reading**  
   Port langsung dari V1.2. Sampling non-blocking.

2. **Serial Protocol (SSP2000)**  
   Output Serial **identik** dengan V1.2 agar software SSP2000 tetap kompatibel.

3. **Web Server + WebSocket**
   - Serve halaman HTML/CSS/JS (bisa di-embed sebagai string atau SPIFFS jika memungkinkan).
   - WebSocket untuk real-time data dan command.
   - Routing sederhana:
     - `/` → index (Halaman Utama)
     - `/kalibrasi`
     - `/config`
     - `/hasil`

4. **Fitur per Halaman**

| Halaman | Fitur |
|---------|-------|
| **Halaman Utama** | Nilai real-time, indikator arah (toe-in/toe-out), grafik live, status koneksi |
| **Kalibrasi** | Tombol “Zero / Kalibrasi”, tampilkan nilai raw sebelum & sesudah, simpan offset |
| **Config** | Interval sampling, unit (mm/m atau m/km), WiFi SSID/Password, threshold warning |
| **Hasil** | Nilai terakhir, waktu, status lulus/gagal (opsional), tombol reset |

## Library yang Dibutuhkan

- `WiFiS3` (bawaan R4 WiFi)
- `ArduinoJson` (recommended)
- WebSocket library ringan (atau implementasi sederhana dengan `WiFiClient`)

## Contoh Alur Kalibrasi

1. User buka halaman `/kalibrasi`.
2. Tekan tombol “Kalibrasi Zero”.
3. Browser kirim: `{"cmd": "calibrate_zero"}`.
4. Arduino baca sensor saat kondisi diam → simpan offset ke EEPROM.
5. Arduino kirim balik status sukses + nilai offset.
6. Semua halaman berikutnya menggunakan nilai yang sudah dikalibrasi.

## Langkah Kerja Selanjutnya

1. Porting sensor + serial protocol dari V1.2 (uji dengan SSP2000).
2. Buat WebSocket dasar + halaman utama.
3. Tambah halaman Kalibrasi, Config, dan Hasil.
4. Integrasi penyimpanan kalibrasi & config.
5. Testing dual mode (Serial + WebSocket bersamaan).

## Pertanyaan yang Perlu Dijawab Sebelum Coding

1. Preferensi WiFi: **AP Mode** (hotspot sendiri) atau **STA Mode**?
2. Apakah kalibrasi hanya zero offset, atau ada gain/multi-point?
3. Apakah hasil perlu disimpan permanen (EEPROM) atau cukup sesi saja?
4. Apakah perlu SoftwareSerial juga di R4 (seperti V1.2)?

---

**Rekomendasi:**  
Mulai dengan Dual Mode + struktur 4 halaman di atas.  
Serial tetap kompatibel dengan SSP2000, sementara Web UI mengikuti alur yang familiar (Utama → Kalibrasi → Config → Hasil).

Siap dilanjutkan ke tahap coding setelah konfirmasi detail.
