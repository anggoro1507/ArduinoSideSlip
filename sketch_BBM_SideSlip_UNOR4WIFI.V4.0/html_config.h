/*
 * html_config.h
 * Config page - English
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
    .form-group {
      margin-bottom: 14px;
    }
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
    }
    hr {
      border: none;
      border-top: 1px solid #eee;
      margin: 16px 0;
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
    <button type="button">Save Settings</button>
  </div>

  <div class="card">
    <h2>Change Account</h2>
    <p class="hint">ID & Password max 15 characters. No spaces.</p>
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
    <button type="button">Save Account</button>
  </div>

  <div class="nav">
    <a href="/pengujian">Back to Measurement</a>
  </div>
</body>
</html>
)rawliteral";

#endif
