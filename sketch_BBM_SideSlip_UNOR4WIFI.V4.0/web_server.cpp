/*
 * web_server.cpp
 * Web Server + endpoint /data untuk polling
 */

#include "web_server.h"
#include "html_index.h"
#include "html_pengujian.h"
#include "html_kalibrasi.h"
#include "html_config.h"
#include "web_common.h"
#include <ArduinoJson.h>

WiFiServer server(80);

// External dari main sketch
extern float sensorFilter;
extern int cal_zero_adc;
extern int cal_high_adc;

float calculateSideSlip(int adc);

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
  if (q > 0) path = path.substring(0, q);

  if (path == "/data") {
    // JSON data untuk polling
    float val = calculateSideSlip((int)sensorFilter);
    StaticJsonDocument<128> doc;
    doc["value"] = val;
    doc["adc"] = (int)sensorFilter;

    String json;
    serializeJson(doc, json);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Connection: close");
    client.println();
    client.print(json);
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
