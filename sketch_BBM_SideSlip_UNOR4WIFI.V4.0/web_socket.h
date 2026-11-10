/*
 * web_socket.h
 * WebSocket server untuk data realtime
 * Library: WebSockets by Markus Sattler
 * Install via Library Manager: "WebSockets"
 */

#ifndef WEB_SOCKET_H
#define WEB_SOCKET_H

#include <WebSocketsServer.h>

extern WebSocketsServer webSocket;

void setupWebSocket();
void handleWebSocket();
void broadcastData(float value, int adc);

#endif
