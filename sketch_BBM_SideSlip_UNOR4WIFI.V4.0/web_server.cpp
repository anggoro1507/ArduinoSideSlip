/*
 * web_server.cpp
 * Implementasi Web Server untuk Side Slip R4
 * Melayani halaman HTML via WiFi AP
 */

#include "web_server.h"
#include "html_index.h"
#include "html_pengujian.h"
#include "html_kalibrasi.h"
#include "html_config.h"
#include "web_common.h"

WiFiServer server(80);

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
  else if (path == "/style.css") {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/css");
    client.println("Connection: close");
    client.println();
    client.print(CSS_COMMON);
  }
  else {
    client.print("<h1>404 Not Found</h1>");
  }

  delay(1);
  client.stop();
}
