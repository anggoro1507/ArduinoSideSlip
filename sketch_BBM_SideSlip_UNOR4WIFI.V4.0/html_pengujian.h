/*
 * html_pengujian.h
 * Halaman Pengujian - mengikuti tampilan software SSP2000
 * Tidak menampilkan ADC mentah
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
  <style>
    body {
      margin: 0;
      background: #000;
      color: #fff;
      font-family: Arial, Helvetica, sans-serif;
      overflow: hidden;
    }
    .status-box {
      position: absolute;
      top: 10px;
      left: 10px;
      border: 1px solid #fff;
      padding: 8px;
      background: #111;
      font-size: 14px;
    }
    .status-box select, .status-box button {
      width: 140px;
      margin: 4px 0;
      padding: 4px;
    }
    .indicator {
      height: 18px;
      background: #00f;
      margin-top: 4px;
    }
    .value-box {
      position: absolute;
      top: 80px;
      left: 50%;
      transform: translateX(-50%);
      text-align: center;
    }
    #value {
      font-size: 72px;
      font-weight: bold;
      padding: 10px 30px;
      background: #444;
      display: inline-block;
      min-width: 180px;
    }
    #value.positive { background: #0a0; }
    #value.negative { background: #c00; }
    #value.zero { background: #555; }
    .unit {
      font-size: 20px;
      margin-top: 4px;
    }
    .axle {
      position: absolute;
      bottom: 120px;
      left: 50%;
      transform: translateX(-50%);
      background: #333;
      border: 1px solid #fff;
      padding: 8px 20px;
      font-size: 28px;
      min-width: 60px;
      text-align: center;
    }
    .wheels {
      position: absolute;
      bottom: 40px;
      width: 100%;
      height: 200px;
    }
    .wheel {
      position: absolute;
      width: 80px;
      height: 140px;
      background: linear-gradient(#888, #444);
      border-radius: 8px;
      bottom: 20px;
    }
    .wheel.left { left: 15%; }
    .wheel.right { right: 15%; }
    .line {
      position: absolute;
      width: 2px;
      height: 180px;
      background: #fff;
      bottom: 0;
    }
    .line.left { left: 22%; }
    .line.right { right: 22%; }
    .nav {
      position: absolute;
      bottom: 10px;
      left: 50%;
      transform: translateX(-50%);
    }
    .nav a {
      color: #fff;
      margin: 0 10px;
      text-decoration: none;
      border: 1px solid #fff;
      padding: 6px 12px;
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

  <div class="axle" id="axle">0</div>

  <div class="wheels">
    <div class="wheel left"></div>
    <div class="line left"></div>
    <div class="wheel right"></div>
    <div class="line right"></div>
  </div>

  <div class="nav">
    <a href="/">Logout</a>
    <a href="/kalibrasi">Kalibrasi</a>
    <a href="/config">Config</a>
  </div>

  <script>
    // Contoh update nilai (nanti diganti WebSocket)
    function updateValue(val) {
      const el = document.getElementById('value');
      el.textContent = val.toFixed(1);
      el.className = '';
      if (val > 0.05) el.classList.add('positive');
      else if (val < -0.05) el.classList.add('negative');
      else el.classList.add('zero');
    }
    // Demo
    updateValue(0);
  </script>
</body>
</html>
)rawliteral";

#endif
