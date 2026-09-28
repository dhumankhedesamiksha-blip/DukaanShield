# 🛡️ DukaanShield

## Affordable Electrical Safety Assistant for Small Business

DukaanShield is an affordable electrical safety system designed to help
small businesses monitor electrical conditions and detect potential hazards
such as overheating and abnormal electrical parameters.

## 🚨 Problem Statement

Small shops often use multiple electrical appliances through shared
extension boards and ageing wiring. Overloading and overheating may go
unnoticed, which can lead to equipment damage, business losses and fire risks.

## 💡 Our Solution

DukaanShield continuously monitors important electrical parameters and
provides an early warning to the shopkeeper.

### Monitor → Detect → Alert → Protect → Affordable

## ⚙️ Main Features

- 🌡️ Temperature monitoring
- ⚡ Voltage monitoring
- 🔌 Current monitoring
- 📊 Power/energy monitoring
- 🖥️ OLED display
- 🔔 Buzzer alert
- 💡 LED indication
- 🔄 Relay-based protection
- 🧠 ESP32-based control system

## 🧩 Components Used

- ESP32
- OLED Display
- PZEM Energy Monitoring Module
- Temperature Sensor
- Relay Module
- Buzzer
- LED
- Current Transformer (CT)
- Logic Level Converter
- DC Power Supply

## 🔄 Working

1. Sensors collect electrical and temperature data.
2. ESP32 processes the sensor readings.
3. Important values are displayed on the OLED.
4. The system checks the readings against safety limits.
5. If an abnormal condition is detected, an alert is generated.
6. Relay control can be used as part of the protection mechanism.

## 🏗️ System Architecture

Sensors → ESP32 → Data Processing → OLED Display  
                           ↓  
                    Alert / Protection  
                           ↓  
                       Relay

## 👥 Team Mindforge

- Priti — Team Leader
- Samiksha — Team Member
- Himani — Team Member
- Sakshi — Team Member

## 🚀 Future Scope

- Mobile application
- Cloud monitoring
- Real-time notifications
- Data logging
- Automatic emergency shutdown
- Improved fault detection
- Compact commercial enclosure

## ⚠️ Safety

Mains AC connections can be dangerous. Always use proper electrical
protection, isolation and supervision while testing high-voltage circuits.

## 📜 License

MIT License
