/*
 * web_server.cpp
 * Fixed login parsing
 */

#include "web_server.h"
#include "html_index.h"
#include "html_pengujian.h"
#include "html_kalibrasi.h"
#include "html_config.h"
#include "web_common.h"
#include <ArduinoJson.h>

WiFiServer server(80);

extern int Int_ADC;
extern int cal_zero_adc;
extern int cal_high_adc;
extern float Koef_ADC;
extern float noise_tolerance;
extern int wait_time_measure;
extern float Value_result;
extern float Cal_Real_Zero;
extern float Cal_Max_Real_Value;
extern bool Show_Result_Flag;
extern bool Zero_Display_Flag;
extern bool Zero_Potition_Flag;
extern bool Calibration_Flag;

extern String account1_id;
extern String account1_pw;
extern String account2_id;
extern String account2_pw;
extern const char* ADMIN_ID;
extern const char* ADMIN_PW;

void saveToEEPROM();
void resetMeasurement();
void setCalibrationMode(bool on);

String getParam(String query, String key) {
  int start = query.indexOf(key + "=");
  if (start < 0) return "";
  start += key.length() + 1;
  int end = query.indexOf('&', start);
  if (end < 0) end = query.length();
  return query.substring(start, end);
}

void setupWebServer() {
  server.begin();
  Serial.println("Web server started on port 80");
}

void handleWebServer() {
  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  String path = "/";
  int start = request.indexOf(' ');
  int end = request.indexOf(' ', start + 1);
  if (start > 0 && end > start) {
    path = request.substring(start + 1, end);
  }

  int q = path.indexOf('?');
  String query = "";
  if (q > 0) {
    query = path.substring(q + 1);
    path = path.substring(0, q);
  }

  if (path == "/data") {
    float displayValue = 0;
    if (Calibration_Flag) {
      displayValue = (Int_ADC - Cal_Real_Zero) / Koef_ADC * 10.0f;
      if (abs(displayValue) > abs(Cal_Max_Real_Value)) {
        Cal_Max_Real_Value = displayValue;
      }
    } else {
      if (Show_Result_Flag) {
        displayValue = Value_result;
      } else if (Zero_Display_Flag) {
        displayValue = 0;
        Zero_Display_Flag = false;
      } else {
        displayValue = 0;
      }
    }

    StaticJsonDocument<192> doc;
    doc["adc"] = Int_ADC;
    doc["value"] = displayValue;
    doc["max"] = Cal_Max_Real_Value;
    doc["result"] = Show_Result_Flag;
    doc["zeroed"] = Zero_Potition_Flag;
    doc["cal_mode"] = Calibration_Flag;

    String json;
    serializeJson(doc, json);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Connection: close");
    client.println();
    client.print(json);
  }
  else if (path == "/login") {
    String id = getParam(query, "id");
    String pw = getParam(query, "pw");
    String role = "";
    
    Serial.print("Login: id="); Serial.print(id);
    Serial.print(" pw="); Serial.println(pw);
    Serial.print("Acc1: "); Serial.print(account1_id); Serial.print("/"); Serial.println(account1_pw);
    
    if (id == ADMIN_ID && pw == ADMIN_PW) {
      role = "admin";
    } else if (id == account1_id && pw == account1_pw) {
      role = "supervisor";
    } else if (id == account2_id && pw == account2_pw) {
      role = "tester";
    }
    
    StaticJsonDocument<64> doc;
    doc["ok"] = role.length() > 0;
    doc["role"] = role;
    
    String json;
    serializeJson(doc, json);
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print(json);
  }
  else if (path == "/account/get") {
    StaticJsonDocument<128> doc;
    doc["id1"] = account1_id;
    doc["id2"] = account2_id;
    
    String json;
    serializeJson(doc, json);
    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print(json);
  }
  else if (path == "/account/save") {
    String target = getParam(query, "target");
    String newId = getParam(query, "id");
    String newPw = getParam(query, "pw");
    
    if (newId.length() > 0 && newId.length() <= 15 && newPw.length() > 0 && newPw.length() <= 15) {
      if (target == "1") {
        account1_id = newId;
        account1_pw = newPw;
      } else if (target == "2") {
        account2_id = newId;
        account2_pw = newPw;
      }
      saveToEEPROM();
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.print("OK");
    } else {
      client.println("HTTP/1.1 400 Bad Request");
      client.println("Connection: close");
      client.println();
      client.print("Invalid");
    }
  }
  else if (path == "/cal/get") {
    StaticJsonDocument<64> doc;
    doc["zero"] = cal_zero_adc;
    doc["high"] = cal_high_adc;
    doc["koef"] = Koef_ADC;

    String json;
    serializeJson(doc, json);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print(json);
  }
  else if (path == "/settings/get") {
    StaticJsonDocument<64> doc;
    doc["noise"] = noise_tolerance;
    doc["wait"] = wait_time_measure;

    String json;
    serializeJson(doc, json);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.print(json);
  }
  else if (path == "/cal/save") {
    String zeroStr = getParam(query, "zero");
    String highStr = getParam(query, "high");
    if (zeroStr.length() > 0 && highStr.length() > 0) {
      cal_zero_adc = zeroStr.toInt();
      cal_high_adc = highStr.toInt();
      saveToEEPROM();
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.print("OK");
    } else {
      client.println("HTTP/1.1 400 Bad Request");
      client.println("Connection: close");
      client.println();
    }
  }
  else if (path == "/settings/save") {
    String noiseStr = getParam(query, "noise");
    String waitStr = getParam(query, "wait");
    if (noiseStr.length() > 0 && waitStr.length() > 0) {
      noise_tolerance = noiseStr.toFloat();
      wait_time_measure = waitStr.toInt();
      saveToEEPROM();
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println("Connection: close");
      client.println();
      client.print("OK");
    } else {
      client.println("HTTP/1.1 400 Bad Request");
      client.println("Connection: close");
      client.println();
    }
  }
  else if (path == "/reset") {
    resetMeasurement();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.print("OK");
  }
  else if (path == "/style.css") {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/css");
    client.println("Connection: close");
    client.println();
    client.print(CSS_COMMON);
  }
  else {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();

    if (path == "/" || path == "/index.html") {
      client.print(HTML_INDEX);
    }
    else if (path == "/pengujian") {
      setCalibrationMode(false);
      client.print(HTML_PENGUJIAN);
    }
    else if (path == "/kalibrasi") {
      setCalibrationMode(true);
      client.print(HTML_KALIBRASI);
    }
    else if (path == "/config") {
      client.print(HTML_CONFIG);
    }
    else {
      client.print("<h1>404 Not Found</h1><a href='/'>Back</a>");
    }
  }

  delay(1);
  client.stop();
}
