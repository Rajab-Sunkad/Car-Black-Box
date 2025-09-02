# Car-Black-Box
Embedded system project that records and monitors vehicle data for accident analysis.

# 🚗 Car Black Box (Event Data Recorder)

## 📌 Overview
The **Car Black Box** is an embedded system project inspired by the airplane black box concept.  
It continuously records vehicle parameters such as **speed, engine status, and collision information**.  
In case of an accident, the stored data can be used for **accident investigation, and for safety analysis**.

---

## ⚡ Features
- Records critical driving parameters (speed, Time, gear status, etc.)
- Event logging in **EEPROM / external memory**
Accident detection simulated using a switch to represent collision occurrence (can be replaced by vibration/impact sensors in real-world systems).
- Stores **timestamped data**
- UART/Serial communication for data retrieval
- I2C for data storage in external EEPROM
- Error logging for debugging and analysis

---

## 🛠️ Technologies Used
- **Microcontroller:** PIC18F4580    
- **Protocols:** UART (data logging & communication) and I2C for data storage
- **Language:** Embedded C  
- **IDE/Compiler:** MPLAB X IDE, XC8 Compiler  

---

## 🚀 Applications
- Accident analysis
- Driver behavior monitoring (fleet & logistics)
- Automotive safety research & training
- Affordable event data recorder for cars and two-wheelers 

---
