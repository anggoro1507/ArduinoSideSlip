/*
 * html_pengujian.h
 * Halaman Pengujian - Realtime
 */

#ifndef HTML_PENGUJIAN_H
#define HTML_PENGUJIAN_H

const char HTML_PENGUJIAN[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>SideSlip - Pengujian</title>
  <link rel="stylesheet" href="/style.css">
</head>
<body>
  <div class="container">
    <h1>Pengujian</h1>
    <p>Nilai Real-time:</p>
    <h2 id="value">0.0 mm/m</h2>
    <p>ADC: <span id="adc">0</span></p>
    <p>Peak: <span id="peak">0.0</span> mm/m</p>
    <br>
    <a href="/"><button type="button">Logout</button></a>
    <a href="/kalibrasi"><button type="button">Kalibrasi</button></a>
    <a href="/config"><button type="button">Config</button></a>
  </div>
</body>
</html>
)rawliteral";

#endif
