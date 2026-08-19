# Smart Multi-Microcontroller Robotic Arm System 🤖🦾

An advanced industrial/laboratory robotic arm control system combining **STM32** hardware servo drive controllers, an **ESP32-CAM** module for real-time visual streaming, and an **ESP8266 NodeMCU** web server controller interface.

---

## 🌟 Architecture Overview

This project splits responsibility across specialized microcontroller subsystems:

1. **STM32 Motor Controller (`stm32_code/`)**: High-speed pulse/PWM timing for joint motor and servo articulation.
2. **ESP32-CAM Vision Module (`esp_cam_code/`)**: Live video stream server delivering high-resolution visual feedback of robotic arm workspace.
3. **ESP8266 NodeMCU Web Interface (`esp_nodemcu_code/`)**: Hosts a web dashboard for interactive manual control, telemetry, and web sockets interface.
4. **Nanochip Custom PCB Integration (`Nanochip project/`)**: Hardware schematic and circuit layouts.

---

## 📁 Repository Contents

```
.
├── stm32_code/            # STM32 F103 / STM32 low-level motor & kinematics controller
├── esp_cam_code/          # ESP32-CAM camera web server firmware
├── esp_nodemcu_code/      # ESP8266 NodeMCU WiFi & Web dashboard controller
├── Nanochip project/      # Custom microcontroller PCB files
├── Dashboard.jpg          # Web Dashboard UI preview
├── Pin configuration*.jpg # Detailed pinout schematics & wiring diagrams
└── stm connection.jpg     # STM32 hardware interconnect diagram
```

---

## 📌 Wiring & Connection Guides

Detailed pinout connections and hardware wiring diagrams are provided in the high-resolution image files:
- `Dashboard.jpg` - Web UI layout and manual control interface
- `Pin configuration.jpg` to `Pin configuration 5.jpg` - Servo motors to STM32 & ESP32 pinouts
- `stm connection.jpg` - STM32 communication bus connections

---

## 🚀 Setup & Flashing

1. **STM32**: Open `stm32_code/stm32_code.ino` in Arduino IDE or STM32CubeIDE. Select your target STM32 board and upload.
2. **ESP32-CAM**: Open `esp_cam_code/` and flash using an FTDI adapter with `Camera Web Server` configuration.
3. **NodeMCU**: Flash `esp_nodemcu_code/` after updating your local Wi-Fi SSID and password.

---

## 📄 License
Released under the [MIT License](LICENSE).
