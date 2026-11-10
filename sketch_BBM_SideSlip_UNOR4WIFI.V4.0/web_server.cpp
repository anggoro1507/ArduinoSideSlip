/*
 * web_server.cpp
 * Web Server + endpoint /data, /cal/save, /settings/save
 */

#include "web_server.h"
#include "html_index.h"
#include "html_pengujian.h"
#include "html_kalibrasi.h"
#include "html_config.h"
#include "web_common.h"
#include <ArduinoJson.h>

WiFiServer server(80);

extern float sensorFilter;
extern int cal_zero_adc;
extern int cal_high_adc;
extern float noise_tolerance;
extern int wait_time_measure;
extern float peak_value;
extern bool show_result_flag;

float calculateSideSlip(int adc);
void saveToEEPROM();
void resetMeasurement();

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
    float val = calculateSideSlip((int)sensorFilter);
    StaticJsonDocument<128> doc;
    doc["value"] = val;
    doc["adc"] = (int)sensorFilter;
    doc["peak"] = peak_value;
    doc["result"] = show_result_flag;

    String json;
    serializeJson(doc, json);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Connection: close");
    client.println();
    client.print(json);
  }
  else if (path == "/cal/save") {
    // Parse query: zero=123&high=456
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
    // Parse query: noise=0.3&wait=3
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
      client.print(HTML_PENGUJIAN);
    }
    else if (path == "/kalibrasi") {
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
