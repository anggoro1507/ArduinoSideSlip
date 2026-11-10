/*
 * sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino
 * Arduino Uno R4 WiFi - Side Slip Tester
 *
 * Fitur:
 * - Sensor A5, ADC 14-bit dimapping ke 0-9999
 * - Serial protocol kompatibel SSP2000 (*XXXX# saat terima 'R', kirim terus)
 * - WiFi AP + Web Server + WebSocket realtime
 * - Login: ID1 (Kepala), ID2 (Penguji), sa (Admin hardcoded)
 * - EEPROM untuk kalibrasi, settings, akun
 * - Dual Serial (Serial + Serial1)
 *
 * Library yang dibutuhkan:
 * - WebSockets by Markus Sattler (install via Library Manager)
 */

#include <WiFiS3.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "web_server.h"
#include "web_socket.h"

// ==================== KONFIGURASI ====================
#define SENSOR_PIN          A5
#define ADC_MAX_14BIT       16383
#define ADC_MAX_4DIGIT      9999
#define SERIAL_BAUD         9600
#define INTERVAL_MS         37
#define WS_INTERVAL_MS      100

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
float noise_tolerance       = 0.3f;
int wait_time_measure       = 3;

// Filter
float alpha                 = 0.8f;
float sensorFilter          = 0;

// Serial protocol state
int rcv                     = 0;
unsigned long per_37ms      = 0;
unsigned long lastWsSend    = 0;

// ==================== EEPROM MAP ====================
#define EE_MAGIC            0
#define EE_CAL_ZERO         2
#define EE_CAL_HIGH         4
#define EE_NOISE            6
#define EE_WAIT_TIME        10
#define EE_ACC1_ID          12
#define EE_ACC1_PW          28
#define EE_ACC2_ID          44
#define EE_ACC2_PW          60
#define EE_END              76

// ==================== SETUP ====================
void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(SERIAL_BAUD);

  analogReadResolution(14);

  EEPROM.begin();
  loadFromEEPROM();

  // WiFi AP
  WiFi.beginAP(AP_SSID, AP_PASSWORD);
  Serial.print("AP IP: ");
  Serial.println(WiFi.localIP());

  setupWebServer();
  setupWebSocket();
}

// ==================== LOOP ====================
void loop() {
  // Baca sensor
  int raw14 = analogRead(SENSOR_PIN);
  int adc4digit = map(raw14, 0, ADC_MAX_14BIT, 0, ADC_MAX_4DIGIT);
  sensorFilter = alpha * adc4digit + (1.0 - alpha) * sensorFilter;

  // Handle Serial protocol SSP2000
  handleSerialProtocol();

  // Handle Web Server
  handleWebServer();

  // Handle WebSocket
  handleWebSocket();

  // Broadcast data ke WebSocket setiap 100ms
  unsigned long now = millis();
  if (now - lastWsSend >= WS_INTERVAL_MS) {
    float val = calculateSideSlip((int)sensorFilter);
    broadcastData(val, (int)sensorFilter);
    lastWsSend = now;
  }
}

// ==================== SERIAL PROTOCOL (SSP2000) ====================
void handleSerialProtocol() {
  if (Serial.available()) {
    rcv = Serial.read();
  }
  if (Serial1.available()) {
    rcv = Serial1.read();
  }

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
  port.println(buf);
}

// ==================== EEPROM ====================
void loadFromEEPROM() {
  uint16_t magic;
  EEPROM.get(EE_MAGIC, magic);

  if (magic != 0xB2C0) {
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

  Serial.println("EEPROM saved");
}

// ==================== HELPER ====================
float calculateSideSlip(int adc) {
  if (cal_high_adc == cal_zero_adc) return 0;
  float koef = cal_high_adc - cal_zero_adc;
  return ((adc - cal_zero_adc) / koef) * 10.0f;
}
