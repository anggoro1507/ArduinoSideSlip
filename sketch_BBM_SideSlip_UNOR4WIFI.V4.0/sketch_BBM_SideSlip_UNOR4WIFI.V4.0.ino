/*
 * sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino
 * Arduino Uno R4 WiFi - Side Slip Tester
 *
 * Fitur:
 * - Sensor A5, ADC 14-bit dimapping ke 0-9999
 * - Serial protocol kompatibel SSP2000 (*XXXX# saat terima 'R', kirim terus)
 * - WiFi AP + WebSocket + Web UI
 * - Login: ID1 (Kepala), ID2 (Penguji), sa (Admin hardcoded)
 * - EEPROM untuk kalibrasi, settings, akun
 * - Dual Serial (Serial + Serial1)
 */

#include <WiFiS3.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

// ==================== KONFIGURASI ====================
#define SENSOR_PIN          A5
#define ADC_MAX_14BIT       16383
#define ADC_MAX_4DIGIT      9999
#define SERIAL_BAUD         9600
#define INTERVAL_MS         37

// WiFi AP
const char* AP_SSID         = "SideSlipBBM";
const char* AP_PASSWORD     = "12345678";

// Admin hardcoded
const char* ADMIN_ID        = "sa";
const char* ADMIN_PW        = "@abcd1234";

// Default akun
String account1_id          = "1";
String account1_pw          = "1";
String account2_id          = "2";
String account2_pw          = "2";

// Kalibrasi & Settings
int cal_zero_adc            = 0;
int cal_high_adc            = 0;
float noise_tolerance       = 0.3f;   // % dari max 9999
int wait_time_measure       = 3;      // detik

// Filter
float alpha                 = 0.8f;
float sensorFilter          = 0;

// Serial protocol state
int rcv                     = 0;
unsigned long per_37ms      = 0;

// ==================== EEPROM MAP ====================
// Alamat EEPROM (total ~100 byte)
#define EE_MAGIC            0     // 2 byte magic number (0xB2C0 = valid)
#define EE_CAL_ZERO         2     // 2 byte int
#define EE_CAL_HIGH         4     // 2 byte int
#define EE_NOISE            6     // 4 byte float
#define EE_WAIT_TIME        10    // 2 byte int
#define EE_ACC1_ID          12    // 16 byte string
#define EE_ACC1_PW          28    // 16 byte string
#define EE_ACC2_ID          44    // 16 byte string
#define EE_ACC2_PW          60    // 16 byte string
#define EE_END              76

// ==================== SETUP ====================
void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(SERIAL_BAUD);

  analogReadResolution(14);

  EEPROM.begin();          // R4 butuh begin()
  loadFromEEPROM();

  // WiFi AP
  WiFi.beginAP(AP_SSID, AP_PASSWORD);
  // TODO: Start Web Server + WebSocket
}

// ==================== LOOP ====================
void loop() {
  // Baca sensor
  int raw14 = analogRead(SENSOR_PIN);
  int adc4digit = map(raw14, 0, ADC_MAX_14BIT, 0, ADC_MAX_4DIGIT);
  sensorFilter = alpha * adc4digit + (1.0 - alpha) * sensorFilter;

  // Handle Serial protocol SSP2000
  handleSerialProtocol();

  // TODO: Handle WebSocket
  // TODO: Handle Web Server
}

// ==================== SERIAL PROTOCOL (SSP2000) ====================
void handleSerialProtocol() {
  // Baca karakter baru (jika ada)
  if (Serial.available()) {
    rcv = Serial.read();
  }
  if (Serial1.available()) {
    rcv = Serial1.read();
  }

  // Jika terakhir terima 'R' (82), kirim terus setiap 37ms
  if (rcv == 82) {
    unsigned long currentTime = millis();
    if (currentTime - per_37ms > INTERVAL_MS) {
      sendADCData(Serial);
      sendADCData(Serial1);
      per_37ms = currentTime;
    }
  }
}

void sendADCData(Stream &port) {
  char buf[10];
  int value = (int)sensorFilter;
  sprintf(buf, "*%04d#", value);
  port.println(buf);   // otomatis tambah \r\n
}

// ==================== EEPROM ====================
void loadFromEEPROM() {
  uint16_t magic;
  EEPROM.get(EE_MAGIC, magic);

  if (magic != 0xB2C0) {
    // Belum pernah disimpan → pakai default lalu simpan
    saveToEEPROM();
    return;
  }

  EEPROM.get(EE_CAL_ZERO, cal_zero_adc);
  EEPROM.get(EE_CAL_HIGH, cal_high_adc);
  EEPROM.get(EE_NOISE, noise_tolerance);
  EEPROM.get(EE_WAIT_TIME, wait_time_measure);

  char buf[17];

  EEPROM.get(EE_ACC1_ID, buf);
  account1_id = String(buf);
  EEPROM.get(EE_ACC1_PW, buf);
  account1_pw = String(buf);

  EEPROM.get(EE_ACC2_ID, buf);
  account2_id = String(buf);
  EEPROM.get(EE_ACC2_PW, buf);
  account2_pw = String(buf);

  Serial.println("EEPROM loaded");
}

void saveToEEPROM() {
  uint16_t magic = 0xB2C0;
  EEPROM.put(EE_MAGIC, magic);

  EEPROM.put(EE_CAL_ZERO, cal_zero_adc);
  EEPROM.put(EE_CAL_HIGH, cal_high_adc);
  EEPROM.put(EE_NOISE, noise_tolerance);
  EEPROM.put(EE_WAIT_TIME, wait_time_measure);

  char buf[17];

  account1_id.toCharArray(buf, 16);
  EEPROM.put(EE_ACC1_ID, buf);
  account1_pw.toCharArray(buf, 16);
  EEPROM.put(EE_ACC1_PW, buf);

  account2_id.toCharArray(buf, 16);
  EEPROM.put(EE_ACC2_ID, buf);
  account2_pw.toCharArray(buf, 16);
  EEPROM.put(EE_ACC2_PW, buf);

  EEPROM.commit();   // Penting di R4
  Serial.println("EEPROM saved");
}

// ==================== WEB / WEBSOCKET (TODO) ====================
// TODO:
// - Web Server dengan 4 halaman
// - WebSocket untuk data real-time + command
// - Login system
// - Halaman Kalibrasi (Zero + High 10mm/m)
// - Halaman Config (noise, wait time, ubah akun)
// - Halaman Pengujian

// ==================== HELPER ====================
float calculateSideSlip(int adc) {
  // Hitung mm/m berdasarkan koefisien
  // High = 10 mm/m
  if (cal_high_adc == cal_zero_adc) return 0;
  float koef = cal_high_adc - cal_zero_adc;
  return ((adc - cal_zero_adc) / koef) * 10.0f;
}
