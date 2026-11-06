/*
 * html_kalibrasi.h
 * Halaman Kalibrasi
 */

#ifndef HTML_KALIBRASI_H
#define HTML_KALIBRASI_H

const char HTML_KALIBRASI[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>SideSlip - Kalibrasi</title>
  <link rel="stylesheet" href="/style.css">
</head>
<body>
  <div class="container">
    <h1>Kalibrasi</h1>
    <p>Zero ADC: <span id="zero">-</span></p>
    <p>High ADC: <span id="high">-</span></p>
    <br>
    <button type="button">Set Zero</button>
    <button type="button">Set High (10 mm/m)</button>
    <button type="button">SAVE</button>
    <br><br>
    <a href="/pengujian"><button type="button">Kembali</button></a>
  </div>
</body>
</html>
)rawliteral";

#endif
