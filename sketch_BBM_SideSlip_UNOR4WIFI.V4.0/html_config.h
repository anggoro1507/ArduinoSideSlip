/*
 * html_config.h
 * Config page with account change
 */

#ifndef HTML_CONFIG_H
#define HTML_CONFIG_H

const char HTML_CONFIG[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0">
  <title>SideSlip - Config</title>
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
      padding: 20px;
      width: 100%;
      max-width: 360px;
      margin-bottom: 16px;
    }
    h2 {
      font-size: 18px;
      color: #1a73e8;
      margin-bottom: 16px;
      text-align: center;
    }
    .form-group { margin-bottom: 14px; }
    label {
      display: block;
      font-size: 14px;
      margin-bottom: 4px;
      color: #555;
    }
    input, select {
      width: 100%;
      padding: 10px;
      border: 1px solid #ddd;
      border-radius: 6px;
      font-size: 15px;
    }
    button {
      width: 100%;
      padding: 12px;
      background: #1a73e8;
      color: #fff;
      border: none;
      border-radius: 6px;
      font-size: 15px;
      cursor: pointer;
      margin-top: 8px;
    }
    button:hover { background: #1557b0; }
    .hint {
      font-size: 12px;
      color: #888;
      margin-bottom: 10px;
      line-height: 1.4;
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
    <h2>Settings</h2>
    <div class="form-group">
      <label>Noise Tolerance (%)</label>
      <input type="text" id="noise" value="0.3">
    </div>
    <div class="form-group">
      <label>Wait Time (seconds)</label>
      <input type="text" id="wait" value="3">
    </div>
    <button type="button" id="btnSaveSettings">Save Settings</button>
  </div>

  <div class="card">
    <h2>Change Account</h2>
    <p class="hint">
      Max 15 characters. Letters, numbers, symbols (@#$%&*+-_.) allowed.<br>
      No spaces. No control characters.
    </p>
    <div class="form-group">
      <label>Target Account</label>
      <select id="target">
        <option value="1">ID 1 (Supervisor)</option>
        <option value="2">ID 2 (Tester)</option>
      </select>
    </div>
    <div class="form-group">
      <label>New ID</label>
      <input type="text" id="newid" maxlength="15">
    </div>
    <div class="form-group">
      <label>New Password</label>
      <input type="password" id="newpw" maxlength="15">
    </div>
    <button type="button" id="btnSaveAccount">Save Account</button>
  </div>

  <div class="nav">
    <a href="/pengujian">Back to Measurement</a>
  </div>

  <script>
    fetch('/settings/get').then(r => r.json()).then(d => {
      if (d.noise !== undefined) document.getElementById('noise').value = d.noise;
      if (d.wait !== undefined) document.getElementById('wait').value = d.wait;
    }).catch(() => {});

    fetch('/account/get').then(r => r.json()).then(d => {
      if (d.id1) document.getElementById('target').options[0].text = 'ID 1: ' + d.id1;
      if (d.id2) document.getElementById('target').options[1].text = 'ID 2: ' + d.id2;
    }).catch(() => {});

    document.getElementById('btnSaveSettings').addEventListener('click', function() {
      const noise = document.getElementById('noise').value;
      const wait = document.getElementById('wait').value;
      fetch('/settings/save?noise=' + noise + '&wait=' + wait)
        .then(r => r.text())
        .then(t => {
          if (t === 'OK') alert('Settings saved');
          else alert('Failed');
        })
        .catch(() => alert('Error'));
    });

    document.getElementById('btnSaveAccount').addEventListener('click', function() {
      const target = document.getElementById('target').value;
      const newid = document.getElementById('newid').value;
      const newpw = document.getElementById('newpw').value;
      
      if (newid.length === 0 || newpw.length === 0) {
        alert('ID and Password required');
        return;
      }
      if (newid.indexOf(' ') >= 0 || newpw.indexOf(' ') >= 0) {
        alert('No spaces allowed');
        return;
      }
      
      fetch('/account/save?target=' + target + '&id=' + encodeURIComponent(newid) + '&pw=' + encodeURIComponent(newpw))
        .then(r => r.text())
        .then(t => {
          if (t === 'OK') {
            alert('Account saved');
            document.getElementById('newid').value = '';
            document.getElementById('newpw').value = '';
            location.reload();
          } else {
            alert('Failed: ' + t);
          }
        })
        .catch(() => alert('Error'));
    });
  </script>
</body>
</html>
)rawliteral";

#endif
