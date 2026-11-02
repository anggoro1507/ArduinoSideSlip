/*
 * sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino
 * Arduino Uno R4 WiFi - Side Slip Tester
 *
 * Fitur:
 * - Sensor A5, ADC 14-bit dimapping ke 0-9999
 * - Serial protocol kompatibel SSP2000 (*XXXX# saat terima 'R')
 * - WiFi AP + WebSocket + Web UI (Halaman Utama, Kalibrasi, Config, Hasil)
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

// WiFi AP
const char* AP_SSID         = "SideSlip-R4";
const char* AP_PASSWORD     = "sideslip123";

// Admin hardcoded
const char* ADMIN_ID        = "sa";
const char* ADMIN_PW        = "@abcd1234";

// Default akun (akan disimpan di EEPROM)
String account1_id          = "1";
String account1_pw          = "1";
String account2_id          = "2";
String account2_pw          = "2";

// Kalibrasi & Settings (dari EEPROM)
int cal_zero_adc            = 0;
int cal_high_adc            = 0;
float noise_tolerance       = 0.3;   // % dari max 9999
int wait_time_measure       = 3;     // detik

// Filter
float alpha                 = 0.8;
float sensorFilter          = 0;

// ==================== SETUP ====================
void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(SERIAL_BAUD);   // Serial hardware kedua

  analogReadResolution(14);     // 14-bit

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
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'R') {
      sendADCData(Serial);
    }
  }
  if (Serial1.available()) {
    char c = Serial1.read();
    if (c == 'R') {
      sendADCData(Serial1);
    }
  }
}

void sendADCData(Stream &port) {
  char buf[10];
  int value = (int)sensorFilter;
  sprintf(buf, "*%04d#", value);
  port.println(buf);
}

// ==================== EEPROM ====================
void loadFromEEPROM() {
  // TODO: Load calibration, settings, accounts from EEPROM
  // Format kalibrasi: *KoefZZeroHHigh#
  // Format settings: *0.3T3#
}

void saveToEEPROM() {
  // TODO: Save calibration, settings, accounts to EEPROM
}

// ==================== WEB / WEBSOCKET (TODO) ====================
// TODO:
// - Web Server dengan 4 halaman
// - WebSocket untuk data real-time + command
// - Login system
// - Halaman Kalibrasi (Zero + High 10mm/m)
// - Halaman Config (noise, wait time, ubah akun)
// - Halaman Hasil

// ==================== HELPER ====================
float calculateSideSlip(int adc) {
  // TODO: Hitung mm/m berdasarkan kalibrasi
  // High = 10 mm/m
  if (cal_high_adc == cal_zero_adc) return 0;
  float koef = cal_high_adc - cal_zero_adc;
  return ((adc - cal_zero_adc) / koef) * 10.0;
}
