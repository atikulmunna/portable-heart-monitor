<!-- ## Abstract

A portable cardiac monitoring system is crucial in daily life due to the rising prevalence of cardiovascular diseases (CVDs), which are the leading cause of death worldwide. Early detection and consistent monitoring of cardiac health can significantly reduce the risk of severe complications such as heart attacks or strokes. Traditional hospital-based monitoring systems are often inaccessible in remote or underprivileged areas and inconvenient for continuous, long-term use. A portable system bridges this gap by offering real-time tracking of vital parameters like ECG, heart rate, and oxygen levels, empowering individuals to monitor their health from the comfort of their homes. This not only enhances patient outcomes through timely intervention but also reduces the burden on healthcare facilities. This project presents the design and implementation of a portable cardiac monitoring system using the STM32F103C8T6 microcontroller. The system integrates an AD8232 ECG sensor, MAX30102 Heart-Rate/Spo2 sensor to monitor and display real-time ECG signals, Heart-Rate/Oxygen Level on an OLED display. This project report discusses the hardware configuration, software development, and challenges encountered, providing a scalable and cost-effective solution for real-time cardiac monitoring. <br/>
**Keywords:** STM32F103C8T6, AD8232, MAX30102, OLED I2C

## Introduction
### Problem Statement
Cardiovascular diseases (CVDs) are the leading cause of death globally, responsible for millions of deaths each year. Early diagnosis and continuous monitoring are key to improving patient outcomes, reducing the risk of severe events such as heart attacks, strokes, and sudden cardiac arrests. However, current cardiac monitoring solutions, such as hospital-based equipment, are often expensive, bulky, and require specialized medical facilities and personnel. Furthermore, they are not easily accessible in rural or underserved areas where healthcare infrastructure may be limited. The lack of affordable, portable, and easy-to-use cardiac monitoring systems exacerbates this challenge, preventing individuals from effectively managing their heart health in everyday life. As a result, there is a growing need for an affordable, efficient, and portable solution that enables continuous, real-time monitoring of vital cardiac parameters outside of traditional medical settings. 

### Objective
The objective of this project is to design and implement a **Portable Cardiac Monitoring System** that can continuously monitor and display vital cardiac parameters, specifically the **ECG signals**, **heart rate**, and **SpO2 levels**, in real time. By integrating affordable, compact, and widely available components such as the **STM32F103C8 microcontroller**, **MAX30102 sensor**, and **AD8232 ECG module**, this system offers a scalable, cost-effective solution for personal health monitoring. The system will provide an intuitive user interface, displaying ECG graphs and vital signs on a mobile application, allowing individuals to track their cardiac health without the need for specialized medical equipment. 

### Scope 
This portable cardiac monitoring system is designed primarily for **home-based health monitoring** and **fitness tracking**, making it suitable for individuals looking to track their cardiovascular health in a non-clinical environment. It can be used by people with pre-existing heart conditions, fitness enthusiasts, or individuals at risk of cardiovascular disease, helping them stay proactive about their health. The system provides an accessible middle ground between **clinical-grade devices**, which are often costly and complex, and basic **consumer-grade health gadgets**, which may lack the necessary diagnostic accuracy. Moreover, this system offers a more affordable, real-time, and continuous solution for heart health management, addressing the limitations of traditional systems and expanding access to quality cardiac care.-->

# IoT-Based Portable Cardiac Monitoring System

## System Overview
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

*(Add your mobile app or web dashboard screenshot below)*

![Ubidots Mobile Dashboard Placeholder](Put_Your_Image_Link_Here) \
*<p align="center">Figure 1: Live Ubidots Health Monitoring Dashboard</p>*

---

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

## Results Analysis and Discussion

### Result Analysis
*   **ECG Data:** Successfully visualized real-time ECG waveforms locally on the OLED and remotely via the Ubidots web/mobile interface.
    *   *Challenges:* Initial noise in the analog signal was mitigated through appropriate hardware filtering, while rendering latency was solved by adjusting the refresh rate logic so as not to block the main loop.
*   **Heart-Rate / SpO2:** Achieved reliable, real-time pulse and oxygen saturation readings.
    *   *Challenges:* Dealing with ambient light interference and finger movement artifacts. This required establishing a dynamic threshold algorithm that constantly adjusts to the baseline IR light values before calculating the beats-per-minute.
*   **IoT Synchronization:** Successfully passed data reliably between two microcontrollers.
    *   *Challenges:* Preventing buffer overflows and serial desynchronization. Solved by standardizing the baud rate (115200), using strict JSON formatting, and implementing a non-blocking 3-second delay for cloud uploads.

### Discussion
The design and implementation of this system demonstrated a highly practical solution for continuous cardiac monitoring. By upgrading from a standalone device to a dual-MCU IoT architecture, the system overcomes the localized limitations of traditional monitors. The **STM32F103C8** proved highly capable of handling the strict timing requirements of the I2C sensors and OLED rendering, while offloading the heavy Wi-Fi protocols to the **ESP8266** ensured the system never froze or dropped frames while connecting to the internet. 

Challenges encountered during development included minimizing electrical noise across the sensors and compressing the required codebase to fit within the STM32's memory constraints. These were mitigated using advanced IDE optimization flags, digital filtering techniques, and careful management of global variables.

---

## Conclusion
This project successfully demonstrates the feasibility of an affordable, portable, and cloud-connected Cardiac Monitoring System. By addressing the limitations of traditional hospital-based equipment—such as bulkiness, high costs, and lack of remote accessibility—this solution empowers individuals to manage their heart health proactively. 

The integration of the ESP8266 Wi-Fi module elevates the project from a localized tool to a modern telemedicine device, allowing for real-time remote monitoring, data logging, and easy integration with mobile dashboards. This is particularly critical in resource-limited settings or for eldercare, where continuous, remote observation can help predict and reduce the risk of severe cardiovascular events.

**Future Enhancements:** Future iterations of this project could focus on hardware miniaturization by designing a custom Printed Circuit Board (PCB) to replace jumper wires, integrating a rechargeable Li-Po battery management system (BMS) for true portability, and implementing machine learning algorithms on the cloud side for automated anomaly detection in the ECG waveforms.
