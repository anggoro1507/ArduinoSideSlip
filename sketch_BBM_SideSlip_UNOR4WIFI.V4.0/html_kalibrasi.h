/*
 * html_kalibrasi.h
 * Halaman Kalibrasi - mengikuti tampilan software SSP2000
 * - Panel kiri: Status + Calibration (radio, ADC, progress, NEXT/SAVE/EXIT)
 * - Tengah: Max (nilai terjauh) + nilai saat ini + tombol ZERO (manual)
 * - Tidak auto-zero, zero hanya saat tombol ZERO ditekan
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
  <style>
    body {
      margin: 0;
      background: #000;
      color: #fff;
      font-family: Arial, Helvetica, sans-serif;
      overflow: hidden;
    }
    .panel {
      position: absolute;
      top: 10px;
      left: 10px;
      border: 1px solid #fff;
      background: #111;
      padding: 8px;
      font-size: 14px;
      min-width: 180px;
    }
    .panel select, .panel button {
      width: 100%;
      margin: 3px 0;
      padding: 4px;
    }
    .indicator {
      height: 16px;
      background: #00f;
      margin-top: 4px;
    }
    .cal-panel {
      position: absolute;
      top: 160px;
      left: 10px;
      border: 1px solid #fff;
      background: #111;
      padding: 10px;
      font-size: 13px;
      min-width: 180px;
    }
    .cal-panel label {
      display: block;
      margin: 4px 0;
    }
    .cal-panel button {
      width: 48%;
      margin: 4px 1%;
      padding: 6px;
    }
    .progress {
      height: 16px;
      background: #333;
      border: 1px solid #666;
      margin: 6px 0;
    }
    .progress-bar {
      height: 100%;
      background: #0a0;
      width: 0%;
    }
    .max-box {
      position: absolute;
      top: 30px;
      left: 50%;
      transform: translateX(-50%);
      text-align: center;
    }
    .max-label {
      font-size: 18px;
      margin-bottom: 4px;
    }
    #maxValue {
      font-size: 48px;
      font-weight: bold;
      background: #444;
      padding: 8px 24px;
      display: inline-block;
      min-width: 140px;
    }
    .current-box {
      position: absolute;
      bottom: 80px;
      left: 50%;
      transform: translateX(-50%);
      text-align: center;
    }
    #currentValue {
      font-size: 36px;
      font-weight: bold;
      background: #444;
      padding: 6px 20px;
      display: inline-block;
      min-width: 120px;
    }
    #btnZero {
      margin-top: 8px;
      background: #800;
      color: #fff;
      border: 1px solid #fff;
      padding: 8px 20px;
      font-size: 16px;
      cursor: pointer;
    }
    .unit {
      font-size: 14px;
      margin-top: 2px;
    }
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
  <!-- Status Panel -->
  <div class="panel">
    <div>Status</div>
    <select><option>WiFi AP</option></select>
    <button>DISCONNECT</button>
    <div class="indicator"></div>
  </div>

  <!-- Calibration Panel -->
  <div class="cal-panel">
    <div>Calibration</div>
    <label>
      <input type="radio" name="calPoint" id="radio0" checked> 0 mm/m
      <span id="zeroAdc" style="float:right;">-</span>
    </label>
    <label>
      <input type="radio" name="calPoint" id="radioHigh"> +10 mm/m
      <span id="highAdc" style="float:right;">-</span>
    </label>
    <div>DATA ADC <span id="adc" style="float:right;">0</span></div>
    <div id="realValue" style="text-align:right;">0.0 mm/m</div>
    <div class="progress"><div class="progress-bar" id="prog"></div></div>
    <button id="btnNext">NEXT</button>
    <button id="btnExit">EXIT</button>
  </div>

  <!-- Max Value (paling jauh) -->
  <div class="max-box">
    <div class="max-label">Max</div>
    <div id="maxValue">0.0</div>
    <div class="unit">mm / m</div>
  </div>

  <!-- Current Value + ZERO -->
  <div class="current-box">
    <div id="currentValue">0.0</div>
    <div class="unit">mm / m</div>
    <br>
    <button id="btnZero">ZERO</button>
  </div>

  <div class="nav">
    <a href="/pengujian">Kembali ke Pengujian</a>
  </div>

  <script>
    // State kalibrasi
    let zeroAdc = null;
    let highAdc = null;
    let calZero = 0;
    let maxValue = 0;
    let currentAdc = 0;
    let koef = 256; // default sementara

    function updateDisplay() {
      const real = (currentAdc - calZero) / koef * 10;
      document.getElementById('adc').textContent = currentAdc;
      document.getElementById('realValue').textContent = real.toFixed(1) + ' mm/m';
      document.getElementById('currentValue').textContent = real.toFixed(1);

      // Progress (0-9999)
      const pct = Math.min(100, (currentAdc / 9999) * 100);
      document.getElementById('prog').style.width = pct + '%';

      // Max = nilai dengan |absolut| terbesar
      if (Math.abs(real) > Math.abs(maxValue)) {
        maxValue = real;
      }
      document.getElementById('maxValue').textContent = maxValue.toFixed(1);
    }

    document.getElementById('btnZero').addEventListener('click', function() {
      calZero = currentAdc;
      maxValue = 0;
      updateDisplay();
    });

    document.getElementById('btnNext').addEventListener('click', function() {
      if (document.getElementById('radio0').checked) {
        zeroAdc = currentAdc;
        document.getElementById('zeroAdc').textContent = zeroAdc;
        document.getElementById('radio0').checked = false;
        document.getElementById('radioHigh').checked = true;
      } else if (document.getElementById('radioHigh').checked) {
        highAdc = currentAdc;
        document.getElementById('highAdc').textContent = highAdc;
        document.getElementById('btnNext').textContent = 'SAVE';
      }
    });

    document.getElementById('btnNext').addEventListener('click', function() {
      if (this.textContent === 'SAVE' && zeroAdc !== null && highAdc !== null) {
        const koefCalc = highAdc - zeroAdc;
        alert('Simpan kalibrasi:\nZero ADC: ' + zeroAdc + '\nHigh ADC: ' + highAdc + '\nKoef: ' + koefCalc);
        // TODO: kirim ke Arduino
      }
    });

    document.getElementById('btnExit').addEventListener('click', function() {
      window.location.href = '/pengujian';
    });

    // Demo data (nanti diganti WebSocket)
    currentAdc = 512;
    updateDisplay();
  </script>
</body>
</html>
)rawliteral";

#endif
