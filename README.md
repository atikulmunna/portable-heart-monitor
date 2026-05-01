

# IoT-Based Portable Cardiac Monitoring System

This project is an advanced, IoT-enabled Portable Cardiac Monitoring System. Utilizing a dual-microcontroller architecture, it seamlessly captures, processes, and streams real-time biometric data. Users can interact with the system locally via a physical keypad and an OLED screen, or monitor their health remotely through a live cloud dashboard.

### Components Used
*   **STM32F103C8T6 Microcontroller:** The primary processing unit (the "Sensor Brain"). It handles high-speed data acquisition, filtering, and localized display rendering.
*   **ESP8266 (NodeMCU):** The Wi-Fi gateway (the "Network Brain"). It receives processed JSON data from the STM32 via serial communication and publishes it to the cloud using MQTT.
*   **4x4 Matrix Keypad:** Provides an interactive physical user interface, allowing patients or clinicians to instantly toggle between different diagnostic modes (Menu, ECG, BPM, SpO2).
*   **AD8232 ECG Sensor:** Measures the heart's electrical activity and provides an analog output for real-time ECG waveform monitoring.
*   **MAX30102 Pulse Oximeter:** Tracks heart rate and blood oxygen saturation (SpO2) using advanced photoplethysmography (PPG).
*   **OLED Display (SSD1306):** Renders the user menu, real-time ECG waveforms, and vital statistics directly on the device.

### Key Features
*   **Multi-Mode UI:** Dynamic physical keypad navigation for selecting specific health metrics on demand.
*   **Live Cloud Integration:** Real-time data streaming to a Ubidots IoT dashboard for remote patient monitoring.
*   **Real-time ECG Visualization:** Continuous rendering of electrical heart activity.
*   **Precise Vitals Tracking:** Live Heart Rate (BPM) and Oxygen Saturation (SpO2) calculations.
*   **Dual-MCU Efficiency:** Separates sensor processing (STM32) from network handling (ESP8266) to prevent lag and ensure smooth hardware performance.

---

## IoT Cloud Dashboard (Ubidots)
The system streams data over Wi-Fi directly to a Ubidots cloud dashboard, enabling remote monitoring from anywhere in the world. The dashboard features visual widgets including a live-scrolling ECG chart, a BPM gauge, and a SpO2 indicator.

<p align="center">
  <img src="https://github.com/user-attachments/assets/67aff9f8-1516-43fb-8f7d-83141ea76629" width="400" alt="Ubidots Mobile Dashboard Placeholder">
</p>
<p align="center"><i>Figure 1: Live Ubidots Health Monitoring Dashboard</i></p>


## Pinout Diagram and Explanation

### Pinout Diagrams
![STM32F1 Pinout Diagram](https://github.com/user-attachments/assets/7e010855-c06b-47bf-b04d-ed42c032a4b6) \
*<p align="center">Figure 2: STM32F1 Pinout Diagram</p>*

![Pinout View of Project Components](https://github.com/user-attachments/assets/b32106bb-8288-48c0-9207-2085ced2355e) \
*<p align="center">Figure 3: Pinout View of Project Components</p>*

### Pin Connections

| Component | STM32F1 Pin | Description |
| :--- | :--- | :--- |
| **MAX30102 (SCL)** | PB6 | I2C Clock |
| **MAX30102 (SDA)** | PB7 | I2C Data |
| **MAX30102 (INT)** | PA1 | Interrupt Handling (Optional) |
| **AD8232 (OUT)** | PA0 | Analog input to the ADC for ECG signals |
| **OLED Display (SCL)** | PB6 | I2C Clock (Shared Bus) |
| **OLED Display (SDA)** | PB7 | I2C Data (Shared Bus) |
| **Keypad (Rows)** | PA7, PA6, PA5, PA4 | Digital inputs for matrix keypad rows |
| **Keypad (Cols)** | PA3, PA2, PB0, PB1 | Digital inputs for matrix keypad columns |
| **ESP8266 (RX)** | PA9 (TX) | Serial communication to transmit JSON data to Wi-Fi chip |
| **Power (3.3V)** | 3.3V | Constant 3.3 Volt Supply (Isolated between MCUs) |
| **GND** | GND | Common Ground Point across all components |

### Explanation
The system operates by delegating tasks. The **STM32F1** acts as the master sensor controller. It continuously polls the **AD8232** via its ADC (PA0) and the **MAX30102** via the I2C bus (PB6/PB7). Based on the mode selected by the user via the **Keypad**, the STM32 processes the relevant sensor data and pushes a localized visual to the **OLED**. Concurrently, the STM32 packages all live biometric data into a formatted JSON string and transmits it over Serial (TX: PA9) to the **ESP8266**, which acts as a dedicated MQTT modem, pushing the payload to the Ubidots cloud.

---

## Project Diagram and Visual

### Block Diagram
![Project Block Diagram](https://github.com/user-attachments/assets/0d1d4ec0-c5f7-472a-8946-ac1eaa22aa62) \
*<p align="center">Figure 4: Project Block Diagram</p>*

### Project Visualization
![Project Image](https://github.com/user-attachments/assets/acc53315-8f7e-497e-8e16-92e243f3af65) \
*<p align="center">Figure 5: Hardware Assembly</p>*

---

## Methodology

### Hardware Configuration
*   **Sensor Integration:** Connected the AD8232 ECG sensor to the STM32 ADC (PA0) and the MAX30102 to the shared I2C bus (PB6, PB7).
*   **Display & UI:** Interfaced the SSD1306 OLED via I2C and wired a 4x4 matrix keypad to digital pins for menu navigation.
*   **IoT Bridge:** Established a Serial TX/RX connection between the STM32 (PA9) and the ESP8266 (RX) with a common ground to ensure safe data transfer.
*   **Power Management:** Ensured isolated 3.3V power routing to prevent voltage regulator conflicts between the STM32 and ESP8266.
*   **Programming:** Utilized ST-Link V2 to flash the STM32F1 and standard USB serial to flash the ESP8266.

### Software Development
*   **Firmware:** Developed in the Arduino IDE leveraging C/C++.
*   **Optimization:** Utilized Link Time Optimization (LTO) to successfully compress multiple heavy libraries (U8g2, Keypad, MAX30105) into the STM32's flash memory.
*   **Cloud Architecture:** Implemented `ArduinoJson` to serialize biometric data and `PubSubClient` on the ESP8266 to maintain a lightweight MQTT connection to the Ubidots API.

### Signal Processing
*   Filtered and amplified raw analog ECG signals for smooth, continuous line-chart rendering on both the OLED and the Ubidots dashboard.
*   Measured Infrared (IR) and Red-Light absorption from the MAX30102, passing the raw values through a smoothing algorithm to stabilize BPM and SpO2 calculations.

---

### Result Analysis
*   **ECG Data:** Successfully visualized real-time ECG waveforms locally on the OLED and remotely via the Ubidots web/mobile interface.
    *   *Challenges:* Initial noise in the analog signal was mitigated through appropriate hardware filtering, while rendering latency was solved by adjusting the refresh rate logic so as not to block the main loop.
*   **Heart-Rate / SpO2:** Achieved reliable, real-time pulse and oxygen saturation readings.
    *   *Challenges:* Dealing with ambient light interference and finger movement artifacts. This required establishing a dynamic threshold algorithm that constantly adjusts to the baseline IR light values before calculating the beats-per-minute.
*   **IoT Synchronization:** Successfully passed data reliably between two microcontrollers.
    *   *Challenges:* Preventing buffer overflows and serial desynchronization. Solved by standardizing the baud rate (115200), using strict JSON formatting, and implementing a non-blocking 3-second delay for cloud uploads.
