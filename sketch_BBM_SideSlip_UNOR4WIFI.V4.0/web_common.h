/*
 * web_common.h
 * CSS + JavaScript yang dipakai bersama oleh semua halaman
 */

#ifndef WEB_COMMON_H
#define WEB_COMMON_H

const char CSS_COMMON[] PROGMEM = R"rawliteral(
* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}
body {
  font-family: Arial, Helvetica, sans-serif;
  background: #f0f2f5;
  color: #333;
  min-height: 100vh;
}
.container {
  max-width: 400px;
  margin: 60px auto;
  background: #fff;
  padding: 30px;
  border-radius: 10px;
  box-shadow: 0 4px 12px rgba(0,0,0,0.1);
}
h1 {
  text-align: center;
  margin-bottom: 24px;
  color: #1a73e8;
}
.form-group {
  margin-bottom: 16px;
}
label {
  display: block;
  margin-bottom: 6px;
  font-weight: bold;
}
input[type="text"],
input[type="password"] {
  width: 100%;
  padding: 10px;
  border: 1px solid #ccc;
  border-radius: 6px;
  font-size: 16px;
}
button {
  width: 100%;
  padding: 12px;
  background: #1a73e8;
  color: #fff;
  border: none;
  border-radius: 6px;
  font-size: 16px;
  cursor: pointer;
}
button:hover {
  background: #1557b0;
}
.error {
  color: #d93025;
  text-align: center;
  margin-top: 12px;
  font-size: 14px;
}
)rawliteral";

const char JS_COMMON[] PROGMEM = R"rawliteral(
// JavaScript bersama (akan diisi nanti)
)rawliteral";

#endif
