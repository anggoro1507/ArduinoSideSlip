/*
 * html_pengujian.h
 * Halaman Pengujian - style seperti login (putih/biru)
 * Tanpa status box
 * Responsive untuk smartphone
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
      font-family: Arial, Helvetica, sans-serif;
      background: #f0f2f5;
      color: #333;
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      padding: 16px;
    }
    .value-box {
      text-align: center;
      margin: 24px 0;
      background: #fff;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 2px 8px rgba(0,0,0,0.1);
      width: 100%;
      max-width: 320px;
    }
    #value {
      font-size: 56px;
      font-weight: bold;
      padding: 8px 16px;
      background: #555;
      color: #fff;
      display: inline-block;
      min-width: 140px;
      border-radius: 6px;
    }
    #value.ok { background: #0a0; }
    #value.over { background: #c00; }
    #value.zero { background: #555; }
    .unit {
      font-size: 16px;
      margin-top: 8px;
      color: #666;
    }
    .axle {
      background: #fff;
      border: 1px solid #ddd;
      padding: 12px 28px;
      font-size: 28px;
      text-align: center;
      margin: 16px 0;
      border-radius: 8px;
      box-shadow: 0 2px 6px rgba(0,0,0,0.08);
    }
    .nav {
      margin-top: 24px;
      display: flex;
      gap: 10px;
      flex-wrap: wrap;
      justify-content: center;
    }
    .nav a {
      color: #fff;
      text-decoration: none;
      background: #1a73e8;
      padding: 12px 18px;
      border-radius: 6px;
      font-size: 15px;
    }
    .nav a:hover { background: #1557b0; }
  </style>
</head>
<body>
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
