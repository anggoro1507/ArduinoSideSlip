/*
 * html_index.h
 * Login page - English
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
      align-items: center;
      justify-content: center;
      padding: 16px;
    }
    .container {
      background: #fff;
      border-radius: 12px;
      box-shadow: 0 2px 10px rgba(0,0,0,0.1);
      padding: 32px;
      width: 100%;
      max-width: 340px;
    }
    h1 {
      font-size: 22px;
      color: #1a73e8;
      text-align: center;
      margin-bottom: 24px;
    }
    .form-group {
      margin-bottom: 16px;
    }
    label {
      display: block;
      font-size: 14px;
      margin-bottom: 4px;
      color: #555;
    }
    input {
      width: 100%;
      padding: 12px;
      border: 1px solid #ddd;
      border-radius: 6px;
      font-size: 16px;
    }
    button {
      width: 100%;
      padding: 14px;
      background: #1a73e8;
      color: #fff;
      border: none;
      border-radius: 6px;
      font-size: 16px;
      cursor: pointer;
      margin-top: 8px;
    }
    button:hover { background: #1557b0; }
    .error {
      color: #c00;
      font-size: 13px;
      text-align: center;
      margin-top: 12px;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>SideSlip Login</h1>
    <form id="loginForm">
      <div class="form-group">
        <label for="id">ID</label>
        <input type="text" id="id" name="id" maxlength="15" required>
      </div>
      <div class="form-group">
        <label for="pw">Password</label>
        <input type="password" id="pw" name="pw" maxlength="15" required>
      </div>
      <button type="submit">Login</button>
    </form>
    <div id="error" class="error"></div>
  </div>

  <script>
    document.getElementById('loginForm').addEventListener('submit', function(e) {
      e.preventDefault();
      const id = document.getElementById('id').value.trim();
      const pw = document.getElementById('pw').value;

      if (id && pw) {
        localStorage.setItem('login_id', id);
        window.location.href = '/pengujian';
      } else {
        document.getElementById('error').textContent = 'ID and Password are required';
      }
    });
  </script>
</body>
</html>
)rawliteral";

#endif
