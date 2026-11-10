/*
 * sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino
 * Arduino Uno R4 WiFi - Side Slip Tester
 */

#include <WiFiS3.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "web_server.h"

#define SENSOR_PIN          A5
#define ADC_MAX_14BIT       16383
#define ADC_MAX_4DIGIT      9999
#define SERIAL_BAUD         9600
#define INTERVAL_MS         37

const char* AP_SSID         = "SideSlipBBM";
const char* AP_PASSWORD     = "12345678";

const char* ADMIN_ID        = "sa";
const char* ADMIN_PW        = "@abcd1234";

String account1_id          = "1";
String account1_pw          = "1";
String account2_id          = "2";
String account2_pw          = "2";

int cal_zero_adc            = 0;
int cal_high_adc            = 0;
float noise_tolerance       = 0.3f;
int wait_time_measure       = 3;

float alpha                 = 0.8f;
float sensorFilter          = 0;

int rcv                     = 0;
unsigned long per_37ms      = 0;

// Measurement state
int zero_adc                = 0;
int high_adc                = 0;
int low_adc                 = 9999;
float peak_value            = 0;
bool zero_position_flag     = true;
bool stable_check_flag      = false;
bool show_result_flag       = false;
bool need_initial_zero      = true;
unsigned long t0            = 0;
unsigned long t1            = 0;
unsigned long t2            = 0;
int last_adc                = 0;

#define EE_MAGIC            0
#define EE_CAL_ZERO         4
#define EE_CAL_HIGH         8
#define EE_NOISE            12
#define EE_WAIT_TIME        16
#define EE_ACC1_ID          20
#define EE_ACC1_PW          36
#define EE_ACC2_ID          52
#define EE_ACC2_PW          68
#define EE_END              84

void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial1.begin(SERIAL_BAUD);
  analogReadResolution(14);
  EEPROM.begin();
  loadFromEEPROM();

  WiFi.beginAP(AP_SSID, AP_PASSWORD);
  Serial.print("AP IP: ");
  Serial.println(WiFi.localIP());

  setupWebServer();
}

void loop() {
  int raw14 = analogRead(SENSOR_PIN);
  int adc4digit = map(raw14, 0, ADC_MAX_14BIT, 0, ADC_MAX_4DIGIT);
  sensorFilter = alpha * adc4digit + (1.0 - alpha) * sensorFilter;

  // Initial zero on first stable reading
  if (need_initial_zero) {
    zero_adc = (int)sensorFilter;
    high_adc = zero_adc;
    low_adc = zero_adc;
    need_initial_zero = false;
    Serial.print("Initial zero: ");
    Serial.println(zero_adc);
  }

  handleSerialProtocol();
  handleWebServer();
  updateMeasurement((int)sensorFilter);
}

void updateMeasurement(int data) {
  float noiseThreshold = noise_tolerance * 9999.0f / 100.0f;

  if (zero_position_flag && abs(data - zero_adc) > noiseThreshold) {
    if (data > zero_adc) {
      high_adc = data;
    } else {
      low_adc = data;
    }
    zero_position_flag = false;
    stable_check_flag = false;
    show_result_flag = false;
    t0 = millis();
  }

  if (!zero_position_flag && !stable_check_flag) {
    if (data > zero_adc) {
      if (data > high_adc) {
        high_adc = data;
        t0 = millis();
      } else if (millis() - t0 > 2500) {
        stable_check_flag = true;
        t1 = millis();
      }
    } else {
      if (data < low_adc) {
        low_adc = data;
        t0 = millis();
      } else if (millis() - t0 > 2500) {
        stable_check_flag = true;
        t1 = millis();
      }
    }
  }

  if (stable_check_flag && millis() - t1 > (wait_time_measure * 1000UL - 3000)) {
    if (abs(data - last_adc) > noiseThreshold) {
      t2 = millis();
    } else if (millis() - t2 > 500) {
      int peakAdc = (abs(high_adc - zero_adc) > abs(low_adc - zero_adc)) ? high_adc : low_adc;
      peak_value = calculateSideSlip(peakAdc);
      show_result_flag = true;

      // Auto-zero after stable
      zero_adc = data;
      high_adc = data;
      low_adc = data;
      zero_position_flag = true;
      stable_check_flag = false;
    }
  } else {
    t2 = millis();
  }

  last_adc = data;
}

void resetMeasurement() {
  zero_adc = (int)sensorFilter;
  high_adc = zero_adc;
  low_adc = zero_adc;
  peak_value = 0;
  zero_position_flag = true;
  stable_check_flag = false;
  show_result_flag = false;
  Serial.print("Manual zero: ");
  Serial.println(zero_adc);
}

void handleSerialProtocol() {
  if (Serial.available()) rcv = Serial.read();
  if (Serial1.available()) rcv = Serial1.read();

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
  sprintf(buf, "*%04d#", (int)sensorFilter);
  port.println(buf);
}

void loadFromEEPROM() {
  uint16_t magic;
  EEPROM.get(EE_MAGIC, magic);

  if (magic != 0xB2C0) {
    Serial.println("EEPROM empty, init defaults");
    saveToEEPROM();
    return;
  }

  EEPROM.get(EE_CAL_ZERO, cal_zero_adc);
  EEPROM.get(EE_CAL_HIGH, cal_high_adc);
  EEPROM.get(EE_NOISE, noise_tolerance);
  EEPROM.get(EE_WAIT_TIME, wait_time_measure);

  char buf[17];
  EEPROM.get(EE_ACC1_ID, buf); account1_id = String(buf);
  EEPROM.get(EE_ACC1_PW, buf); account1_pw = String(buf);
  EEPROM.get(EE_ACC2_ID, buf); account2_id = String(buf);
  EEPROM.get(EE_ACC2_PW, buf); account2_pw = String(buf);

  Serial.print("EEPROM loaded: zero="); Serial.print(cal_zero_adc);
  Serial.print(" high="); Serial.println(cal_high_adc);
}

void saveToEEPROM() {
  uint16_t magic = 0xB2C0;
  EEPROM.put(EE_MAGIC, magic);
  EEPROM.put(EE_CAL_ZERO, cal_zero_adc);
  EEPROM.put(EE_CAL_HIGH, cal_high_adc);
  EEPROM.put(EE_NOISE, noise_tolerance);
  EEPROM.put(EE_WAIT_TIME, wait_time_measure);

  char buf[17];
  account1_id.toCharArray(buf, 16); EEPROM.put(EE_ACC1_ID, buf);
  account1_pw.toCharArray(buf, 16); EEPROM.put(EE_ACC1_PW, buf);
  account2_id.toCharArray(buf, 16); EEPROM.put(EE_ACC2_ID, buf);
  account2_pw.toCharArray(buf, 16); EEPROM.put(EE_ACC2_PW, buf);

  Serial.println("EEPROM saved");
}

float calculateSideSlip(int adc) {
  if (cal_high_adc == cal_zero_adc) return 0;
  float koef = cal_high_adc - cal_zero_adc;
  return ((adc - cal_zero_adc) / koef) * 10.0f;
}
