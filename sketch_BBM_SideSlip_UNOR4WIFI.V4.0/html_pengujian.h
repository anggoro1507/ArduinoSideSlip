/*
 * html_pengujian.h
 * Measurement page - HTTP polling (lebih stabil di R4)
 * Axle locked at 1
 */

#ifndef HTML_PENGUJIAN_H
#define HTML_PENGUJIAN_H

const char HTML_PENGUJIAN[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0">
  <title>SideSlip - Measurement</title>
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
      padding-bottom: 80px;
    }
    .value-box {
      text-align: center;
      background: #fff;
      padding: 20px;
      border-radius: 12px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
      width: 100%;
      max-width: 320px;
      margin-top: 12px;
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
    .status {
      font-size: 12px;
      color: #888;
      margin-top: 8px;
    }
    .nav {
      position: fixed;
      bottom: 0;
      left: 0;
      right: 0;
      background: #f0f2f5;
      padding: 12px 16px;
      padding-bottom: max(16px, env(safe-area-inset-bottom));
      display: flex;
      gap: 10px;
      flex-wrap: wrap;
      justify-content: center;
      box-shadow: 0 -2px 8px rgba(0,0,0,0.06);
    }
    .nav a {
      color: #fff;
      text-decoration: none;
      background: #1a73e8;
      padding: 12px 18px;
      border-radius: 8px;
      font-size: 15px;
    }
    .nav a:hover { background: #1557b0; }

    @media (min-width: 800px) {
      body { padding-bottom: 16px; }
      .nav {
        position: static;
        margin-top: auto;
        box-shadow: none;
        padding-bottom: 16px;
      }
      .value-box {
        max-width: 480px;
        padding: 32px;
        margin-top: 24px;
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
    <div class="status" id="pollStatus">Polling...</div>
  </div>

  <div class="axle" id="axle">1</div>

  <div class="nav">
    <a href="/">Logout</a>
    <a href="/kalibrasi">Calibration</a>
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

    const statusEl = document.getElementById('pollStatus');
    let errorCount = 0;

    function pollData() {
      fetch('/data')
        .then(r => r.json())
        .then(data => {
          if (typeof data.value === 'number') {
            updateValue(data.value);
          }
          statusEl.textContent = 'Live';
          statusEl.style.color = '#0a0';
          errorCount = 0;
        })
        .catch(() => {
          errorCount++;
          if (errorCount > 3) {
            statusEl.textContent = 'No data';
            statusEl.style.color = '#c00';
          }
        });
    }

    // Poll setiap 200ms
    setInterval(pollData, 200);
    pollData();
  </script>
</body>
</html>
)rawliteral";

#endif
