/*
 * web_server.cpp
 * Login, account, session endpoints
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
    int idPos = query.indexOf("id=");
    int pwPos = query.indexOf("pw=");
    String role = "";
    if (idPos >= 0 && pwPos >= 0) {
      String id = query.substring(idPos + 3, query.indexOf('&', idPos) > 0 ? query.indexOf('&', idPos) : query.length());
      String pw = query.substring(pwPos + 3);
      
      if (id == ADMIN_ID && pw == ADMIN_PW) {
        role = "admin";
      } else if (id == account1_id && pw == account1_pw) {
        role = "supervisor";
      } else if (id == account2_id && pw == account2_pw) {
        role = "tester";
      }
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
    int t = query.indexOf("target=");
    int i = query.indexOf("id=");
    int p = query.indexOf("pw=");
    if (t >= 0 && i >= 0 && p >= 0) {
      int target = query.substring(t + 7, query.indexOf('&', t)).toInt();
      int idEnd = query.indexOf('&', i);
      String newId = query.substring(i + 3, idEnd > 0 ? idEnd : query.length());
      String newPw = query.substring(p + 3);
      
      if (newId.length() > 0 && newId.length() <= 15 && newPw.length() > 0 && newPw.length() <= 15) {
        if (target == 1) {
          account1_id = newId;
          account1_pw = newPw;
        } else if (target == 2) {
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
    } else {
      client.println("HTTP/1.1 400 Bad Request");
      client.println("Connection: close");
      client.println();
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
    int z = query.indexOf("zero=");
    int h = query.indexOf("high=");
    if (z >= 0 && h >= 0) {
      cal_zero_adc = query.substring(z + 5).toInt();
      int amp = query.indexOf('&', h);
      cal_high_adc = query.substring(h + 5, amp > 0 ? amp : query.length()).toInt();
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
    int n = query.indexOf("noise=");
    int w = query.indexOf("wait=");
    if (n >= 0 && w >= 0) {
      noise_tolerance = query.substring(n + 6).toFloat();
      int amp = query.indexOf('&', w);
      wait_time_measure = query.substring(w + 5, amp > 0 ? amp : query.length()).toInt();
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
