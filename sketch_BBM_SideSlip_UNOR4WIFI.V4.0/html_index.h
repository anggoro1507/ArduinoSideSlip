/*
 * html_index.h
 * Login page with Arduino validation + session
 */

#ifndef HTML_INDEX_H
#define HTML_INDEX_H

const char HTML_INDEX[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0">
  <title>SideSlip - Login</title>
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
    .card {
      background: #fff;
      border-radius: 12px;
      box-shadow: 0 2px 12px rgba(0,0,0,0.1);
      padding: 32px;
      width: 100%;
      max-width: 360px;
    }
    h1 {
      font-size: 24px;
      color: #1a73e8;
      text-align: center;
      margin-bottom: 24px;
    }
    .form-group { margin-bottom: 16px; }
    label {
      display: block;
      font-size: 14px;
      margin-bottom: 6px;
      color: #555;
    }
    input {
      width: 100%;
      padding: 12px;
      border: 1px solid #ddd;
      border-radius: 8px;
      font-size: 16px;
    }
    button {
      width: 100%;
      padding: 14px;
      background: #1a73e8;
      color: #fff;
      border: none;
      border-radius: 8px;
      font-size: 16px;
      cursor: pointer;
      margin-top: 8px;
    }
    button:hover { background: #1557b0; }
    .error {
      color: #c00;
      font-size: 14px;
      text-align: center;
      margin-top: 12px;
      display: none;
    }
  </style>
</head>
<body>
  <div class="card">
    <h1>SideSlip Login</h1>
    <div class="form-group">
      <label>ID</label>
      <input type="text" id="userid" maxlength="15" autocomplete="username">
    </div>
    <div class="form-group">
      <label>Password</label>
      <input type="password" id="password" maxlength="15" autocomplete="current-password">
    </div>
    <button type="button" id="btnLogin">Login</button>
    <div class="error" id="error">Invalid ID or Password</div>
  </div>

  <script>
    const TIMEOUT_MS = 10 * 60 * 1000;

    function checkSession() {
      const session = localStorage.getItem('session');
      if (session) {
        const data = JSON.parse(session);
        if (Date.now() - data.time < TIMEOUT_MS) {
          window.location.href = '/pengujian';
          return;
        }
        localStorage.removeItem('session');
      }
    }

    document.getElementById('btnLogin').addEventListener('click', function() {
      const id = document.getElementById('userid').value;
      const pw = document.getElementById('password').value;
      
      fetch('/login?id=' + encodeURIComponent(id) + '&pw=' + encodeURIComponent(pw))
        .then(r => r.json())
        .then(d => {
          if (d.ok) {
            localStorage.setItem('session', JSON.stringify({
              role: d.role,
              time: Date.now()
            }));
            window.location.href = '/pengujian';
          } else {
            document.getElementById('error').style.display = 'block';
          }
        })
        .catch(() => {
          document.getElementById('error').style.display = 'block';
        });
    });

    document.getElementById('password').addEventListener('keypress', function(e) {
      if (e.key === 'Enter') document.getElementById('btnLogin').click();
    });

    checkSession();
  </script>
</body>
</html>
)rawliteral";

#endif
