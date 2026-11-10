/*
 * html_pengujian.h
 * Measurement page with ZERO button
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
    .status {
      font-size: 14px;
      margin-top: 10px;
      padding: 6px 12px;
      border-radius: 6px;
      display: inline-block;
    }
    .status.zeroed { background: #e8f5e9; color: #2e7d32; }
    .status.measuring { background: #fff3e0; color: #e65100; }
    .status.result { background: #e3f2fd; color: #1565c0; }
    #btnZero {
      display: block;
      width: 100%;
      max-width: 320px;
      margin-top: 12px;
      background: #c00;
      color: #fff;
      border: none;
      padding: 14px;
      font-size: 16px;
      border-radius: 8px;
      cursor: pointer;
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
    .nav a.hidden { display: none; }

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
      #btnZero {
        max-width: 480px;
        font-size: 20px;
        padding: 18px;
      }
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
    <div class="status zeroed" id="status">ZEROED</div>
  </div>

  <button id="btnZero">ZERO</button>

  <div class="axle" id="axle">1</div>

  <div class="nav">
    <a href="/" onclick="logout()">Logout</a>
    <a href="/kalibrasi" id="linkCal" class="hidden">Calibration</a>
    <a href="/config" id="linkConfig" class="hidden">Config</a>
  </div>

  <script>
    const TIMEOUT_MS = 10 * 60 * 1000;
    let lastActivity = Date.now();

    function checkSession() {
      const session = localStorage.getItem('session');
      if (!session) {
        window.location.href = '/';
        return false;
      }
      const data = JSON.parse(session);
      if (Date.now() - data.time > TIMEOUT_MS) {
        localStorage.removeItem('session');
        window.location.href = '/';
        return false;
      }
      if (data.role === 'supervisor' || data.role === 'admin') {
        document.getElementById('linkCal').classList.remove('hidden');
        document.getElementById('linkConfig').classList.remove('hidden');
      }
      return true;
    }

    function updateActivity() {
      lastActivity = Date.now();
      const session = localStorage.getItem('session');
      if (session) {
        const data = JSON.parse(session);
        data.time = Date.now();
        localStorage.setItem('session', JSON.stringify(data));
      }
    }

    function logout() {
      localStorage.removeItem('session');
    }

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

    function updateStatus(zeroed, result) {
      const el = document.getElementById('status');
      el.className = 'status';
      if (result) {
        el.textContent = 'RESULT';
        el.classList.add('result');
      } else if (zeroed) {
        el.textContent = 'ZEROED';
        el.classList.add('zeroed');
      } else {
        el.textContent = 'MEASURING';
        el.classList.add('measuring');
      }
    }

    document.getElementById('btnZero').addEventListener('click', function() {
      fetch('/reset').catch(() => {});
    });

    let errorCount = 0;

    function pollData() {
      if (!checkSession()) return;
      updateActivity();
      
      fetch('/data')
        .then(r => r.json())
        .then(data => {
          updateValue(data.value || 0);
          updateStatus(data.zeroed, data.result);
          errorCount = 0;
        })
        .catch(() => {
          errorCount++;
          if (errorCount > 3) {
            document.getElementById('status').textContent = 'NO DATA';
            document.getElementById('status').className = 'status';
          }
        });
    }

    checkSession();
    setInterval(pollData, 200);
    pollData();
  </script>
</body>
</html>
)rawliteral";

#endif
