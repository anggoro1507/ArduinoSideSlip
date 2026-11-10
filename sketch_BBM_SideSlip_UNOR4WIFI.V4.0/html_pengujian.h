/*
 * html_pengujian.h
 * Halaman Pengujian - tampilan sederhana, responsive untuk smartphone
 * Tanpa background gambar
 * Warna: Merah jika |nilai| > 5 mm/m, Hijau jika normal, Abu jika nol
 * Axle di-lock di 1
 */

#ifndef HTML_PENGUJIAN_H
#define HTML_PENGUJIAN_H

const char HTML_PENGUJIAN[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0">
  <title>SideSlip - Pengujian</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      background: #000;
      color: #fff;
      font-family: Arial, Helvetica, sans-serif;
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 12px;
    }
    .status-box {
      align-self: flex-start;
      border: 1px solid #444;
      padding: 8px;
      background: #111;
      font-size: 13px;
      margin-bottom: 16px;
      width: 100%;
      max-width: 280px;
    }
    .status-box select, .status-box button {
      width: 100%;
      margin: 4px 0;
      padding: 6px;
    }
    .indicator {
      height: 14px;
      background: #00f;
      margin-top: 4px;
    }
    .value-box {
      text-align: center;
      margin: 20px 0;
    }
    #value {
      font-size: 64px;
      font-weight: bold;
      padding: 12px 24px;
      background: #444;
      display: inline-block;
      min-width: 160px;
      border-radius: 4px;
    }
    #value.ok { background: #0a0; }
    #value.over { background: #c00; }
    #value.zero { background: #555; }
    .unit {
      font-size: 18px;
      margin-top: 6px;
      color: #aaa;
    }
    .axle {
      background: #333;
      border: 1px solid #666;
      padding: 10px 24px;
      font-size: 32px;
      min-width: 70px;
      text-align: center;
      margin: 24px 0;
      border-radius: 4px;
    }
    .nav {
      margin-top: auto;
      padding: 16px 0;
      display: flex;
      gap: 8px;
      flex-wrap: wrap;
      justify-content: center;
    }
    .nav a {
      color: #fff;
      text-decoration: none;
      border: 1px solid #555;
      padding: 10px 16px;
      background: #222;
      border-radius: 4px;
      font-size: 14px;
    }
    @media (min-width: 600px) {
      #value { font-size: 80px; }
      .status-box { position: absolute; top: 12px; left: 12px; }
      .value-box { margin-top: 60px; }
    }
  </style>
</head>
<body>
  <div class="status-box">
    <div>Status</div>
    <select><option>WiFi AP</option></select>
    <button>DISCONNECT</button>
    <div class="indicator"></div>
  </div>

  <div class="value-box">
    <div id="value" class="zero">0.0</div>
    <div class="unit">mm / m</div>
  </div>

  <div class="axle" id="axle">1</div>

  <div class="nav">
    <a href="/">Logout</a>
    <a href="/kalibrasi">Kalibrasi</a>
    <a href="/config">Config</a>
  </div>

  <script>
    function updateValue(val) {
      const el = document.getElementById('value');
      el.textContent = val.toFixed(1);
      el.className = '';
      const abs = Math.abs(val);
      if (abs < 0.05) {
        el.classList.add('zero');
      } else if (abs > 5.0) {
        el.classList.add('over');
      } else {
        el.classList.add('ok');
      }
      document.getElementById('axle').textContent = '1';
    }
    updateValue(0);
  </script>
</body>
</html>
)rawliteral";

#endif
