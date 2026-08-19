#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// === CHANGE THESE ===
const char* ssid = "Harish";
const char* password = "12345678";

ESP8266WebServer server(80);

void setupSerial() {
  Serial.begin(115200);
}

void sendToSTM32(int servoId, int angle) {
  Serial.print(servoId);
  Serial.print(":");
  Serial.println(angle);
}

String getHTML() {
  String html = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Advanced 4-Servo Controller</title>
  <style>
    :root {
      --primary: #0066ff;
      --bg: #f0f2f5;
      --card: #ffffff;
      --text: #1a1a1a;
      --track: #e0e0e0;
      --shadow: rgba(0,0,0,0.08);
    }
    .dark {
      --primary: #4d9fff;
      --bg: #0e0e0e;
      --card: #1a1a1a;
      --text: #f0f0f0;
      --track: #333333;
      --shadow: rgba(0,0,0,0.4);
    }
    body { 
      margin:0; padding:20px; background:var(--bg); color:var(--text); 
      font-family:'Segoe UI',Arial,sans-serif; transition:0.4s; 
    }
    header { 
      text-align:center; padding:20px; background:var(--primary); color:white; 
      border-radius:16px; margin-bottom:30px; position:relative; 
    }
    .toggle { position:absolute; right:20px; top:20px; font-size:28px; cursor:pointer; }
    h1 { margin:0; font-size:2.2em; }
    .container { 
      display:grid; grid-template-columns:repeat(auto-fit, minmax(320px,1fr)); gap:24px; 
    }
    .card { 
      background:var(--card); padding:24px; border-radius:20px; 
      box-shadow:0 12px 30px var(--shadow); text-align:center; transition:0.3s; 
    }
    .card:hover { transform:translateY(-8px); }
    h2 { margin:0 0 16px; color:var(--primary); }
    .angle { font-size:56px; font-weight:800; margin:10px 0; color:var(--primary); }
    input[type=range] { 
      -webkit-appearance:none; width:100%; height:14px; border-radius:10px; 
      background:var(--track); outline:none; margin:20px 0; 
    }
    input[type=range]::-webkit-slider-thumb {
      -webkit-appearance:none; height:34px; width:34px; border-radius:50%; 
      background:var(--primary); cursor:pointer; box-shadow:0 4px 12px rgba(0,0,0,0.3);
    }
    .buttons { margin:20px 0; display:flex; flex-wrap:wrap; gap:10px; justify-content:center; }
    button { 
      padding:12px 20px; background:var(--primary); color:white; border:none; 
      border-radius:12px; cursor:pointer; font-weight:600; transition:0.3s; 
    }
    button:hover { transform:scale(1.05); opacity:0.9; }
    .global { text-align:center; margin:40px 0; }
    footer { text-align:center; margin-top:50px; color:#888; font-size:0.9em; }
  </style>
</head>
<body>
  <header>
    <h1>Advanced 4-Servo Controller</h1>
    <div class="toggle" onclick="toggleDark()">🌙</div>
  </header>

  <div class="container">
    <!-- Servo 1 -->
    <div class="card">
      <h2>Servo 1</h2>
      <div class="angle" id="angle1">90</div> degrees
      <input type="range" id="servo1Slider" min="0" max="180" value="90" oninput="updateServo(1,this.value)">
      <div class="buttons">
        <button onclick="setServo(1,0)">0° Close</button>
        <button onclick="setServo(1,90)">90° Center</button>
        <button onclick="setServo(1,180)">180° Open</button>
      </div>
    </div>

    <!-- Servo 2 -->
    <div class="card">
      <h2>Servo 2</h2>
      <div class="angle" id="angle2">90</div> degrees
      <input type="range" id="servo2Slider" min="0" max="180" value="90" oninput="updateServo(2,this.value)">
      <div class="buttons">
        <button onclick="setServo(2,0)">0° Close</button>
        <button onclick="setServo(2,90)">90° Center</button>
        <button onclick="setServo(2,180)">180° Open</button>
      </div>
    </div>

    <!-- Servo 3 -->
    <div class="card">
      <h2>Servo 3</h2>
      <div class="angle" id="angle3">90</div> degrees
      <input type="range" id="servo3Slider" min="0" max="180" value="90" oninput="updateServo(3,this.value)">
      <div class="buttons">
        <button onclick="setServo(3,0)">0° Close</button>
        <button onclick="setServo(3,90)">90° Center</button>
        <button onclick="setServo(3,180)">180° Open</button>
      </div>
    </div>

    <!-- Servo 4 -->
    <div class="card">
      <h2>Servo 4</h2>
      <div class="angle" id="angle4">90</div> degrees
      <input type="range" id="servo4Slider" min="0" max="180" value="90" oninput="updateServo(4,this.value)">
      <div class="buttons">
        <button onclick="setServo(4,0)">0° Close</button>
        <button onclick="setServo(4,90)">90° Center</button>
        <button onclick="setServo(4,180)">180° Open</button>
      </div>
    </div>
  </div>

  <div class="global">
    <h2>Global Controls</h2>
    <button onclick="setAll(90)" style="padding:14px 30px;font-size:1.1em;">All Center (90°)</button>
    <button onclick="setAll(0)" style="padding:14px 30px;font-size:1.1em;">All Close (0°)</button>
    <button onclick="setAll(180)" style="padding:14px 30px;font-size:1.1em;">All Open (180°)</button>
    <button onclick="randomPose()" style="padding:14px 30px;font-size:1.1em;background:#ff5722;">Random Pose 🎲</button>
  </div>

  <footer>Powered by ESP8266 + STM32 • Instant control • No page reloads</footer>

  <script>
    function updateTrack(slider) {
      const val = slider.value;
      const percent = (val / 180) * 100;
      slider.style.background = linear-gradient(to right, var(--primary) ${percent}%, var(--track) ${percent}%);
    }

    function updateServo(id, value) {
      document.getElementById('angle'+id).innerText = value;
      const slider = document.getElementById('servo'+id+'Slider');
      slider.value = value;
      updateTrack(slider);
      const xhr = new XMLHttpRequest();
      xhr.open('GET', '/set?servo='+id+'&angle='+value, true);
      xhr.send();
    }

    function setServo(id, angle) {
      updateServo(id, angle);
    }

    function setAll(angle) {
      for(let i=1;i<=4;i++) updateServo(i, angle);
    }

    function randomPose() {
      for(let i=1;i<=4;i++) {
        const rand = Math.floor(Math.random()*181);
        updateServo(i, rand);
      }
    }

    let dark = false;
    function toggleDark() {
      dark = !dark;
      document.body.classList.toggle('dark');
      document.querySelector('.toggle').innerText = dark ? '☀' : '🌙';
      document.querySelectorAll('input[type=range]').forEach(updateTrack);
    }

    window.onload = () => {
      document.querySelectorAll('input[type=range]').forEach(slider => updateTrack(slider));
    };
  </script>
</body>
</html>
  )=====";
  return html;
}

// ----------------- Server handlers (unchanged) -----------------
void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void handleSet() {
  if (server.hasArg("servo") && server.hasArg("angle")) {
    int servoId = server.arg("servo").toInt();
    int angle = server.arg("angle").toInt();
    if (servoId >= 1 && servoId <= 4 && angle >= 0 && angle <= 180) {
      sendToSTM32(servoId, angle);
      server.send(200, "text/plain", "OK");
    } else server.send(400, "text/plain", "Invalid");
  } else server.send(400, "text/plain", "Missing");
}

// ----------------- Setup & Loop -----------------
void setup() {
  setupSerial();
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 100) { delay(100); Serial.print("."); }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP: http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed - restarting");
    ESP.restart();
  }

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("Web dashboard ready!");
}

void loop() {
  server.handleClient();
}