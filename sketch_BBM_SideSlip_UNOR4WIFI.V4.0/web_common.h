/*
 * web_common.h
 * CSS + JavaScript yang dipakai bersama oleh semua halaman
 * (OPSIONAL - bisa digabung ke tiap halaman jika tidak diperlukan)
 */

#ifndef WEB_COMMON_H
#define WEB_COMMON_H

const char CSS_COMMON[] PROGMEM = R"rawliteral(
/* TODO: CSS bersama */
body { font-family: Arial, sans-serif; }
)rawliteral";

const char JS_COMMON[] PROGMEM = R"rawliteral(
// TODO: JavaScript bersama (WebSocket helper, dll)
)rawliteral";

#endif
