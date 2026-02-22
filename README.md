# Task 7 – Health Vitals Dashboard
---

## 📖 Description  

This project focuses on building a **real-time Health Vitals Monitoring Dashboard** using a microcontroller (ESP32) and a pulse oximeter sensor 

The system performs the following steps:

1. The pulse sensor measures:
   - Heart Rate (Beats Per Minute – BPM)
    
2. The ESP32 processes the sensor data.

3. A web server hosted on the ESP32 displays:
   - Real-time numerical values
   - Live updating graphs for BPM

4. The dashboard can be accessed from any device connected to the same Wi-Fi network.

This task integrates biomedical sensing with real-time web-based visualization.

---

##  Components Required  

- ESP32 Development Board  
- Pulse Oximeter Sensor (MAX30100 / MAX30102)  
- Jumper wires  
- Wi-Fi connection  
- Breadboard  

---

##  Working Principle  

1. The pulse sensor detects variations in blood flow using infrared light.  
2. The ESP32 reads raw sensor values via I2C communication.  
3. The data is processed to calculate:
   - Heart Rate (BPM)
   - Oxygen Saturation (SpO₂)  
4. The ESP32 hosts a web server.  
5. The web page fetches real-time data periodically using:
   - AJAX requests or  
   - WebSockets  
6. Graphs update dynamically without refreshing the page.

---

## System Flow  

**Pulse Sensor → ESP32 → Data Processing → Web Server → Live Dashboard (Graphs)**

---

##  Dashboard Features  

- Real-time BPM display  
- Real-time SpO₂ display  
- Line graph for heart rate trends  
- Line graph for oxygen level trends  
- Auto-refresh or live updating  
- Responsive design (mobile-friendly)

---

##  Graph Visualization  

The web dashboard typically uses:
- JavaScript charting libraries (e.g., Chart.js)  
- AJAX for periodic data fetching  
- JSON format for data transfer  

Graphs show:
- Time (X-axis)  
- Vital value (Y-axis) 
