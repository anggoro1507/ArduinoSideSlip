/*
 * web_server.cpp
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
  else if (path == "/cal/mode") {
    int m = query.indexOf("on=");
    if (m >= 0) {
      setCalibrationMode(query.substring(m + 3).toInt() == 1);
    }
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.print("OK");
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
