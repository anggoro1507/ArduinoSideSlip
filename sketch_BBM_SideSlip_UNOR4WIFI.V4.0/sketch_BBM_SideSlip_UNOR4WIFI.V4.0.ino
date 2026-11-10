/*
 * sketch_BBM_SideSlip_UNOR4WIFI.V4.0.ino
 * Reset EEPROM magic to reinitialize accounts
 */

#include <WiFiS3.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "web_server.h"

#define SENSOR_PIN          A5
#define ADC_MAX_14BIT       16383
#define ADC_MAX_4DIGIT      9999
#define SERIAL_BAUD         9600
#define READ_INTERVAL_MS    37
#define MEASURE_INTERVAL_MS 70

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
float Koef_ADC              = 256;
float noise_tolerance       = 0.3f;
int wait_time_measure       = 3;

float alpha                 = 0.5f;
float sensorFilter          = 0;

int rcv                     = 0;
unsigned long lastRead      = 0;
unsigned long lastMeasure   = 0;

int Int_ADC                 = 0;
int Zero_ADC                = 0;
int High_ADC                = 0;
int Low_ADC                 = 9999;
int Last_ADC                = 0;
float Value_result          = 0;
float Cal_Real_Zero         = 0;
float Cal_Max_Real_Value    = 0;

bool Get_Zero_ADC_Flag      = true;
bool Zero_Potition_Flag     = true;
bool High_ADC_flag          = true;
bool Stable_Check_Flag      = false;
bool Show_Result_Flag       = false;
bool Show_Value_Flag        = false;
bool Zero_Display_Flag      = true;
bool Calibration_Flag       = false;

unsigned long t0            = 0;
unsigned long t1            = 0;
unsigned long t2            = 0;

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

#define MAGIC_VALUE         0xB2C1

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
  unsigned long now = millis();

  if (now - lastRead >= READ_INTERVAL_MS) {
    int raw14 = analogRead(SENSOR_PIN);
    int adc4digit = map(raw14, 0, ADC_MAX_14BIT, 0, ADC_MAX_4DIGIT);
    sensorFilter = alpha * adc4digit + (1.0 - alpha) * sensorFilter;
    Int_ADC = (int)sensorFilter;
    lastRead = now;
  }

  handleSerialProtocol();
  handleWebServer();

  if (now - lastMeasure >= MEASURE_INTERVAL_MS) {
    if (!Calibration_Flag) {
      Measure(Int_ADC);
    }
    lastMeasure = now;
  }
}

void Measure(int Data) {
  float noiseThreshold = noise_tolerance * 9999.0f / 100.0f;

  if (Get_Zero_ADC_Flag) {
    Zero_ADC = Data;
    High_ADC = Data;
    Low_ADC = Data;
    Value_result = 0;
    Zero_Display_Flag = true;
    Zero_Potition_Flag = true;
    Show_Result_Flag = false;
    Show_Value_Flag = false;
    Stable_Check_Flag = false;
    Get_Zero_ADC_Flag = false;
  }

  if (Zero_Potition_Flag && abs(Data - Zero_ADC) > noiseThreshold) {
    Show_Result_Flag = false;
    if (Data > Zero_ADC) {
      High_ADC_flag = true;
      High_ADC = Data;
    } else {
      High_ADC_flag = false;
      Low_ADC = Data;
    }
    Zero_Potition_Flag = false;
    Stable_Check_Flag = false;
    t0 = millis();
  }

  if (!Zero_Potition_Flag && !Stable_Check_Flag) {
    if (High_ADC_flag) {
      if (High_ADC < Data) {
        High_ADC = Data;
        Show_Value_Flag = true;
        t0 = millis();
      } else if (millis() - t0 > 2500) {
        Stable_Check_Flag = true;
        t1 = millis();
      }
    } else {
      if (Low_ADC > Data) {
        Low_ADC = Data;
        Show_Value_Flag = true;
        t0 = millis();
      } else if (millis() - t0 > 2500) {
        Stable_Check_Flag = true;
        t1 = millis();
      }
    }
  }

  if (millis() - t1 > (wait_time_measure * 1000UL - 3000)) {
    if (Stable_Check_Flag) {
      if (abs(Data - Last_ADC) > noiseThreshold) {
        t2 = millis();
      } else if (millis() - t2 > 500) {
        if (High_ADC_flag) {
          Value_result = (High_ADC - Zero_ADC) / Koef_ADC * 10.0f;
        } else {
          Value_result = (Low_ADC - Zero_ADC) / Koef_ADC * 10.0f;
        }
        Show_Result_Flag = true;

        Zero_ADC = Data;
        High_ADC = Data;
        Low_ADC = Data;
        Zero_Potition_Flag = true;
        Stable_Check_Flag = false;
      }
    }
  } else {
    t2 = millis();
  }

  Last_ADC = Data;
}

void resetMeasurement() {
  Get_Zero_ADC_Flag = true;
  Cal_Real_Zero = Int_ADC;
  Cal_Max_Real_Value = 0;
}

void setCalibrationMode(bool on) {
  Calibration_Flag = on;
  if (on) {
    Cal_Real_Zero = Int_ADC;
    Cal_Max_Real_Value = 0;
  }
}

void handleSerialProtocol() {
  if (Serial.available()) rcv = Serial.read();
  if (Serial1.available()) rcv = Serial1.read();

  if (rcv == 82) {
    unsigned long currentTime = millis();
    if (currentTime - lastRead > READ_INTERVAL_MS) {
      sendADCData(Serial);
      sendADCData(Serial1);
    }
  }
}

void sendADCData(Stream &port) {
  char buf[10];
  sprintf(buf, "*%04d#", Int_ADC);
  port.println(buf);
}

void loadFromEEPROM() {
  uint16_t magic;
  EEPROM.get(EE_MAGIC, magic);

  if (magic != MAGIC_VALUE) {
    Serial.println("EEPROM reset, init defaults");
    saveToEEPROM();
    return;
  }

  EEPROM.get(EE_CAL_ZERO, cal_zero_adc);
  EEPROM.get(EE_CAL_HIGH, cal_high_adc);
  EEPROM.get(EE_NOISE, noise_tolerance);
  EEPROM.get(EE_WAIT_TIME, wait_time_measure);

  if (cal_high_adc != cal_zero_adc) {
    Koef_ADC = cal_high_adc - cal_zero_adc;
  }

  char buf[17];
  memset(buf, 0, sizeof(buf));
  EEPROM.get(EE_ACC1_ID, buf); account1_id = String(buf);
  memset(buf, 0, sizeof(buf));
  EEPROM.get(EE_ACC1_PW, buf); account1_pw = String(buf);
  memset(buf, 0, sizeof(buf));
  EEPROM.get(EE_ACC2_ID, buf); account2_id = String(buf);
  memset(buf, 0, sizeof(buf));
  EEPROM.get(EE_ACC2_PW, buf); account2_pw = String(buf);

  Serial.print("Loaded: Acc1="); Serial.print(account1_id);
  Serial.print(" Acc2="); Serial.println(account2_id);
}

void saveToEEPROM() {
  uint16_t magic = MAGIC_VALUE;
  EEPROM.put(EE_MAGIC, magic);
  EEPROM.put(EE_CAL_ZERO, cal_zero_adc);
  EEPROM.put(EE_CAL_HIGH, cal_high_adc);
  EEPROM.put(EE_NOISE, noise_tolerance);
  EEPROM.put(EE_WAIT_TIME, wait_time_measure);

  if (cal_high_adc != cal_zero_adc) {
    Koef_ADC = cal_high_adc - cal_zero_adc;
  }

  char buf[17];
  memset(buf, 0, sizeof(buf));
  account1_id.toCharArray(buf, 16); EEPROM.put(EE_ACC1_ID, buf);
  memset(buf, 0, sizeof(buf));
  account1_pw.toCharArray(buf, 16); EEPROM.put(EE_ACC1_PW, buf);
  memset(buf, 0, sizeof(buf));
  account2_id.toCharArray(buf, 16); EEPROM.put(EE_ACC2_ID, buf);
  memset(buf, 0, sizeof(buf));
  account2_pw.toCharArray(buf, 16); EEPROM.put(EE_ACC2_PW, buf);

  Serial.println("EEPROM saved with defaults");
}
