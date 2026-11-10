/*
 * html_pengujian.h
 * Halaman Pengujian - mengikuti tampilan software SSP2000
 * Menggunakan background image dari SSP2000/Resources/
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
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>SideSlip - Pengujian</title>
  <style>
    body {
      margin: 0;
      background: #000 url('/img/SS_Zero.png') center center no-repeat;
      background-size: cover;
      color: #fff;
      font-family: Arial, Helvetica, sans-serif;
      overflow: hidden;
      height: 100vh;
    }
    .status-box {
      position: absolute;
      top: 10px;
      left: 10px;
      border: 1px solid #fff;
      padding: 8px;
      background: #111;
      font-size: 14px;
      z-index: 10;
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
      top: 60px;
      left: 50%;
      transform: translateX(-50%);
      text-align: center;
      z-index: 10;
    }
    #value {
      font-size: 72px;
      font-weight: bold;
      padding: 10px 30px;
      background: #444;
      display: inline-block;
      min-width: 180px;
    }
    #value.ok { background: #0a0; }
    #value.over { background: #c00; }
    #value.zero { background: #555; }
    .unit {
      font-size: 20px;
      margin-top: 4px;
    }
    .axle {
      position: absolute;
      bottom: 140px;
      left: 50%;
      transform: translateX(-50%);
      background: #333;
      border: 1px solid #fff;
      padding: 8px 20px;
      font-size: 28px;
      min-width: 60px;
      text-align: center;
      z-index: 10;
    }
    .nav {
      position: absolute;
      bottom: 10px;
      left: 50%;
      transform: translateX(-50%);
      z-index: 10;
    }
    .nav a {
      color: #fff;
      margin: 0 10px;
      text-decoration: none;
      border: 1px solid #fff;
      padding: 6px 12px;
      background: #222;
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
        document.body.style.backgroundImage = "url('/img/SS_Zero.png')";
      } else if (abs > 5.0) {
        el.classList.add('over');
        document.body.style.backgroundImage = val > 0 ? "url('/img/SS_Positive.png')" : "url('/img/SS_Negative.png')";
      } else {
        el.classList.add('ok');
        document.body.style.backgroundImage = val >= 0 ? "url('/img/SS_Positive.png')" : "url('/img/SS_Negative.png')";
      }
      document.getElementById('axle').textContent = '1';
    }
    updateValue(0);
  </script>
</body>
</html>
)rawliteral";

#endif
