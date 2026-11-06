/*
 * html_config.h
 * Halaman Config
 */

#ifndef HTML_CONFIG_H
#define HTML_CONFIG_H

const char HTML_CONFIG[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>SideSlip - Config</title>
  <link rel="stylesheet" href="/style.css">
</head>
<body>
  <div class="container">
    <h1>Config</h1>
    <div class="form-group">
      <label>Noise Tolerance (%)</label>
      <input type="text" id="noise" value="0.3">
    </div>
    <div class="form-group">
      <label>Wait Time (detik)</label>
      <input type="text" id="wait" value="3">
    </div>
    <button type="button">Simpan Settings</button>
    <hr>
    <h3>Ubah Akun</h3>
    <p style="font-size:13px;color:#666;">ID & Password maksimal 15 karakter. Hindari spasi.</p>
    <div class="form-group">
      <label>Target Akun</label>
      <select id="target">
        <option value="1">ID 1 (Kepala)</option>
        <option value="2">ID 2 (Penguji)</option>
      </select>
    </div>
    <div class="form-group">
      <label>ID Baru</label>
      <input type="text" id="newid" maxlength="15">
    </div>
    <div class="form-group">
      <label>Password Baru</label>
      <input type="password" id="newpw" maxlength="15">
    </div>
    <button type="button">Simpan Akun</button>
    <br><br>
    <a href="/pengujian"><button type="button">Kembali</button></a>
  </div>
</body>
</html>
)rawliteral";

#endif
