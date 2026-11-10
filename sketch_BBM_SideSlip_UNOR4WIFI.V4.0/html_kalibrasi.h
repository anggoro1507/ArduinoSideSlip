/*
 * html_kalibrasi.h
 * Calibration page - role check
 */

#ifndef HTML_KALIBRASI_H
#define HTML_KALIBRASI_H

const char HTML_KALIBRASI[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0">
  <title>SideSlip - Calibration</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      font-family: Arial, Helvetica, sans-serif;
      background: #f0f2f5;
      color: #333;
      min-height: 100vh;
      padding: 16px;
      padding-bottom: 80px;
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    .card {
      background: #fff;
      border-radius: 10px;
      box-shadow: 0 2px 8px rgba(0,0,0,0.1);
      padding: 16px;
      width: 100%;
      max-width: 360px;
      margin-bottom: 16px;
    }
    h2 {
      font-size: 18px;
      color: #1a73e8;
      margin-bottom: 12px;
      text-align: center;
    }
    .max-box { text-align: center; margin-bottom: 8px; }
    .max-label { font-size: 14px; color: #666; margin-bottom: 4px; }
    #maxValue {
      font-size: 42px;
      font-weight: bold;
      background: #555;
      color: #fff;
      padding: 8px 16px;
      display: inline-block;
      min-width: 120px;
      border-radius: 6px;
    }
    .unit { font-size: 14px; color: #666; margin-top: 4px; }
    .current-box { text-align: center; margin: 12px 0; }
    #currentValue {
      font-size: 32px;
      font-weight: bold;
      background: #444;
      color: #fff;
      padding: 6px 14px;
      display: inline-block;
      min-width: 100px;
      border-radius: 6px;
    }
    #btnZero {
      display: block;
      width: 100%;
      margin-top: 10px;
      background: #c00;
      color: #fff;
      border: none;
      padding: 12px;
      font-size: 16px;
      border-radius: 6px;
      cursor: pointer;
    }
    .cal-row {
      display: flex;
      justify-content: space-between;
      align-items: center;
      padding: 8px 0;
      border-bottom: 1px solid #eee;
    }
    .cal-row:last-child { border-bottom: none; }
    .progress {
      height: 14px;
      background: #eee;
      border-radius: 7px;
      margin: 10px 0;
      overflow: hidden;
    }
    .progress-bar {
      height: 100%;
      background: #0a0;
      width: 0%;
    }
    .btn-row {
      display: flex;
      gap: 8px;
      margin-top: 12px;
    }
    .btn-row button {
      flex: 1;
      padding: 12px;
      border: none;
      border-radius: 6px;
      font-size: 15px;
      cursor: pointer;
    }
    #btnNext { background: #1a73e8; color: #fff; }
    #btnExit { background: #666; color: #fff; }
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
      justify-content: center;
      box-shadow: 0 -2px 8px rgba(0,0,0,0.06);
    }
    .nav a {
      color: #fff;
      text-decoration: none;
      background: #1a73e8;
      padding: 12px 20px;
      border-radius: 6px;
    }
    @media (min-width: 800px) {
      body { padding-bottom: 16px; }
      .nav { position: static; margin-top: auto; box-shadow: none; }
    }
  </style>
</head>
<body>
  <div class="card">
    <div class="max-box">
      <div class="max-label">Max</div>
      <div id="maxValue">0.0</div>
      <div class="unit">mm / m</div>
    </div>
  </div>

  <div class="card">
    <div class="current-box">
      <div id="currentValue">0.0</div>
      <div class="unit">mm / m</div>
      <button id="btnZero">ZERO</button>
    </div>
  </div>

  <div class="card">
    <h2>Calibration</h2>
    <div class="cal-row">
      <label><input type="radio" name="calPoint" id="radio0" checked> 0 mm/m</label>
      <span id="zeroAdc">-</span>
    </div>
    <div class="cal-row">
      <label><input type="radio" name="calPoint" id="radioHigh"> +10 mm/m</label>
      <span id="highAdc">-</span>
    </div>
    <div class="cal-row">
      <span>ADC Data</span>
      <span id="adc">0</span>
    </div>
    <div class="cal-row">
      <span>Value</span>
      <span id="realValue">0.0 mm/m</span>
    </div>
    <div class="progress"><div class="progress-bar" id="prog"></div></div>
    <div class="btn-row">
      <button id="btnNext">NEXT</button>
      <button id="btnExit">EXIT</button>
    </div>
  </div>

  <div class="nav">
    <a href="/pengujian">Back to Measurement</a>
  </div>

  <script>
    // Check role first
    (function() {
      const session = localStorage.getItem('session');
      if (!session) {
        window.location.href = '/';
        return;
      }
      const data = JSON.parse(session);
      if (data.role !== 'supervisor' && data.role !== 'admin') {
        window.location.href = '/pengujian';
        return;
      }
    })();

    let zeroAdc = null;
    let highAdc = null;

    fetch('/cal/get').then(r => r.json()).then(d => {
      if (d.zero) {
        zeroAdc = d.zero;
        document.getElementById('zeroAdc').textContent = zeroAdc;
      }
      if (d.high) {
        highAdc = d.high;
        document.getElementById('highAdc').textContent = highAdc;
      }
    }).catch(() => {});

    setInterval(() => {
      fetch('/data').then(r => r.json()).then(d => {
        document.getElementById('adc').textContent = d.adc || 0;
        document.getElementById('currentValue').textContent = (d.value || 0).toFixed(1);
        document.getElementById('realValue').textContent = (d.value || 0).toFixed(1) + ' mm/m';
        document.getElementById('maxValue').textContent = (d.max || 0).toFixed(1);
        const pct = Math.min(100, ((d.adc || 0) / 9999) * 100);
        document.getElementById('prog').style.width = pct + '%';
      }).catch(() => {});
    }, 200);

    document.getElementById('btnZero').addEventListener('click', function() {
      fetch('/reset').catch(() => {});
    });

    document.getElementById('btnNext').addEventListener('click', function() {
      if (this.textContent === 'SAVE' && zeroAdc !== null && highAdc !== null) {
        fetch('/cal/save?zero=' + zeroAdc + '&high=' + highAdc)
          .then(r => r.text())
          .then(t => {
            if (t === 'OK') alert('Calibration saved');
            else alert('Failed');
          })
          .catch(() => alert('Error'));
        return;
      }
      fetch('/data').then(r => r.json()).then(d => {
        if (document.getElementById('radio0').checked) {
          zeroAdc = d.adc;
          document.getElementById('zeroAdc').textContent = zeroAdc;
          document.getElementById('radio0').checked = false;
          document.getElementById('radioHigh').checked = true;
        } else if (document.getElementById('radioHigh').checked) {
          highAdc = d.adc;
          document.getElementById('highAdc').textContent = highAdc;
          this.textContent = 'SAVE';
        }
      });
    });

    document.getElementById('btnExit').addEventListener('click', function() {
      window.location.href = '/pengujian';
    });
  </script>
</body>
</html>
)rawliteral";

#endif
