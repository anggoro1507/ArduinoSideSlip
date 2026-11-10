/*
 * html_pengujian.h
 * Halaman Pengujian - style putih/biru
 * Responsive: kecil di smartphone, besar di PC
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
      justify-content: center;
      padding: 16px;
    }
    .value-box {
      text-align: center;
      background: #fff;
      padding: 24px;
      border-radius: 12px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
      width: 100%;
      max-width: 320px;
    }
    #value {
      font-size: 56px;
      font-weight: bold;
      padding: 10px 20px;
      background: #555;
      color: #fff;
      display: inline-block;
      min-width: 150px;
      border-radius: 8px;
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
      padding: 14px 32px;
      font-size: 28px;
      text-align: center;
      margin: 20px 0;
      border-radius: 10px;
      box-shadow: 0 2px 6px rgba(0,0,0,0.08);
    }
    .nav {
      display: flex;
      gap: 12px;
      flex-wrap: wrap;
      justify-content: center;
    }
    .nav a {
      color: #fff;
      text-decoration: none;
      background: #1a73e8;
      padding: 14px 22px;
      border-radius: 8px;
      font-size: 16px;
    }
    .nav a:hover { background: #1557b0; }

    /* PC / layar besar */
    @media (min-width: 800px) {
      .value-box {
        max-width: 480px;
        padding: 40px;
      }
      #value {
        font-size: 96px;
        min-width: 240px;
        padding: 16px 32px;
      }
      .unit { font-size: 22px; }
      .axle {
        font-size: 42px;
        padding: 18px 48px;
      }
      .nav a {
        font-size: 18px;
        padding: 16px 28px;
      }
    }
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
