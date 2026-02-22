#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);
MAX30105 particleSensor;

float beatsPerMinute;
float spo2 = 98;   // Dummy SpO2 (Replace with proper algorithm if using full library)

unsigned long lastBeat = 0;

void handleRoot() {
  String page = R"====(
<!DOCTYPE html>
<html>
<head>
  <title>Health Vitals Dashboard</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
<h2>Real-Time Health Vitals</h2>
<p>Heart Rate: <span id="bpm">0</span> BPM</p>
<p>SpO2: <span id="spo2">0</span> %</p>

<canvas id="bpmChart" width="400" height="200"></canvas>
<canvas id="spo2Chart" width="400" height="200"></canvas>

<script>
let bpmData = [];
let spo2Data = [];
let labels = [];

const bpmChart = new Chart(document.getElementById('bpmChart'), {
  type: 'line',
  data: { labels: labels,
    datasets: [{ label: 'BPM', data: bpmData, borderColor: 'red', fill: false }]
  }
});

const spo2Chart = new Chart(document.getElementById('spo2Chart'), {
  type: 'line',
  data: { labels: labels,
    datasets: [{ label: 'SpO2', data: spo2Data, borderColor: 'blue', fill: false }]
  }
});

setInterval(() => {
  fetch('/data')
  .then(response => response.json())
  .then(data => {
    document.getElementById("bpm").innerText = data.bpm;
    document.getElementById("spo2").innerText = data.spo2;

    labels.push('');
    bpmData.push(data.bpm);
    spo2Data.push(data.spo2);

    if(labels.length > 20){
      labels.shift();
      bpmData.shift();
      spo2Data.shift();
    }

    bpmChart.update();
    spo2Chart.update();
  });
}, 1000);
</script>
</body>
</html>
)====";

  server.send(200, "text/html", page);
}

void handleData() {
  String json = "{";
  json += "\"bpm\":" + String(beatsPerMinute) + ",";
  json += "\"spo2\":" + String(spo2);
  json += "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30102 not found!");
    while (1);
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeIR(0x0A);

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();

  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true) {
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);
  }

  if (beatsPerMinute < 40 || beatsPerMinute > 180) {
    beatsPerMinute = 0;
  }
}
