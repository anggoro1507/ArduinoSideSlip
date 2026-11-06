/*
 * html_index.h
 * Halaman Utama - Login
 */

#ifndef HTML_INDEX_H
#define HTML_INDEX_H

const char HTML_INDEX[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>SideSlip - Login</title>
  <link rel="stylesheet" href="/style.css">
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
        document.getElementById('error').textContent = 'ID dan Password harus diisi';
      }
    });
  </script>
</body>
</html>
)rawliteral";

#endif
