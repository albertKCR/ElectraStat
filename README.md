# ⚡ Electrastat — Low-Cost Potentiostat / Source Measure Unit

**Electrastat** is an open-source, low-cost **potentiostat / source-measure unit (SMU)** designed for electrochemical measurements such as **cyclic voltammetry (CV)**, **linear sweep voltammetry (LSV)**, and **square wave voltammetry (SWV)**.  
It can apply potentials from **–0.5 V to +1 V** and measure currents up to **±70 µA** with a resolution of approximately **5 µA**, making it suitable for research, teaching, and prototyping of electrochemical sensors.

The C# interface can be found at https://github.com/albertKCR/Potentiostat. 
---

## 🔧 Key Features

- **Voltage range:** –0.5 V to +1 V  
- **Current range:** ±70 µA  
- **Current resolution:** ≈ 5 µA  
- **Supported techniques:**
  - Linear Sweep Voltammetry (LSV)
  - Cyclic Voltammetry (CV)
  - Square Wave Voltammetry (SWV)
- **DAC:** MCP4725 (12-bit I²C)
- **ADC:** ADS1115 (16-bit differential I²C)
- **Microcontroller:** ATmega328P (Arduino compatible)
- **Communication:** USB (CH340G bridge)
- **Supply rails:** ±8 V regulated from ±12 V input

---

## ⚙️ Hardware Overview

The analog front-end is composed of **CA3140** and **LMC662** operational amplifiers configured for:

- Potential control between **Working (WE)**, **Reference (RE)**, and **Counter (CE)** electrodes  
- **Transimpedance conversion** for current measurement  
- **Signal amplification** and filtering  

---

## 💻 Firmware

The firmware (`SMUNAP.cpp`) is implemented in **C++ for Arduino** and handles:

- Serial command parsing  
- Voltage output control using the MCP4725 DAC  
- Current measurement using the ADS1115 ADC  
- Real-time serial streaming of potential and current data  

## 🖥️ Graphical Interface (WPF C#)

The **Electrastat GUI** provides an intuitive interface for configuring experiments and viewing live electrochemical data.  
Built with **C# and WPF**, it communicates with the SMU via a serial connection.

### Features

- COM port and baud rate selection  
- Live plotting of:
  - **Current vs. Voltage**
  - **Voltage vs. Time**
- Export data as **CSV**
- Save plots as **PNG**
- Clear or reset charts

---

## 💻 GUI Application

1. Open the **Electrastat.exe** application.  
2. Select the correct **COM port** and **baud rate**.  
3. Click **Connect**.  
4. Choose your desired measurement mode and parameters.  
5. Press **Start** to begin acquisition.  

---
