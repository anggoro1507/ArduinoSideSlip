/*
 * web_socket.cpp
 * WebSocket server - kirim data realtime ke browser
 */

#include "web_socket.h"
#include <ArduinoJson.h>

WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      Serial.print("WS client connected: ");
      Serial.println(num);
      break;
    case WStype_DISCONNECTED:
      Serial.print("WS client disconnected: ");
      Serial.println(num);
      break;
    case WStype_TEXT:
      // Bisa terima perintah dari browser nanti
      break;
    default:
      break;
  }
}

void setupWebSocket() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket started on port 81");
}

void handleWebSocket() {
  webSocket.loop();
}

void broadcastData(float value, int adc) {
  StaticJsonDocument<128> doc;
  doc["value"] = value;
  doc["adc"] = adc;

  String json;
  serializeJson(doc, json);

  webSocket.broadcastTXT(json);
}
