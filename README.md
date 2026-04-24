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

## System Overview
### Components Used
  *	**STM32F103C8T6 Microcontroller:** The core processing unit handling data acquisition and signal processing. </br>
  *	**AD8232 ECG Sensor:** Measures electrical signals from the heart and provides analog output for real-time ECG monitoring.</br>
  *	**MAX30102 Heart Rate/Spo2 Sensor:** Tracks heart rate and SpO2 using Infra-Red (IR) as a part of technique called photoplethysmography (PPG). </br>
  *	**OLED Display (SSD1306):** Displays the ECG signal graph in real time.

### Features
* Real-time ECG graph visualization.
* Real-time Heart Rate and Oxygen Level Measure.
* Compact and portable design.
*	Cost-effective and easy to use.

## Pinout Diagram and Explanation
### Pinout Diagram 
![Picture1](https://github.com/user-attachments/assets/7e010855-c06b-47bf-b04d-ed42c032a4b6) \
*<p align="center">Figure 1: STM32F1 Pinout Diagram</p>*
![Picture2](https://github.com/user-attachments/assets/b32106bb-8288-48c0-9207-2085ced2355e) \
*<p align="center">Figure 2: Pinout View of Project Components</p>*
### Pin Connections
| Component  | STM32F1 Pin |  Description |
| ------------- | ------------- | ------------- |
| MAX30102(SCL)  | PB6  | I2C Clock  |
| MAX30102(SDA) | PB7  |  I2C Data  |
| MAX30102(INT)  | PA1  | Interrupt Handling (Optional)  |
| AD8232 (OUT)  | PA0  | Analog input to the ADC for ECG signals  |
| OLED Display (SCL)  | PB6  | I2C Clock  |
| OLED Display (SDA) | PB7  | I2C Data  |
| Power (3.3V)  | 3.3  | Constant 3.3 Volt Supply  |
| GND  | G  | Common Ground Point  |

### Explanation
The STM32F1 microcontroller interfaces with the AD8232 to acquire ECG signals via its ADC (PA0). The signals are processed and displayed as a real-time graph on the OLED display using I2C communication. And the MAX30102 interfaces with STM32F1 microcontroller to send Serial data specifically photoplethysmography data through SDA(PB7). The data is processed and displayed as a real-time Heart Rate/Oxygen Level on the OLED display using I2C communication. The OLED display is configured to provide a clear and intuitive visualization of the ECG waveform and Heart-Rate/Spo2.

### Project Diagram and Visual
### Block Diagram
![Picture3](https://github.com/user-attachments/assets/0d1d4ec0-c5f7-472a-8946-ac1eaa22aa62)
*<p align="center">Figure 3: Project Block Diagram</p>*
### Project Visualization
![Picture4](https://github.com/user-attachments/assets/acc53315-8f7e-497e-8e16-92e243f3af65)
*<p align="center">Figure 4: Project Image</p>*

### Methodology
### Hardware Configuration
* Connected AD8232 ECG sensor to the STM32F1 ADC (PA0).
* Connecter MAX30102 Heart-Rate/Spo2 to the STM32F1 SCL(PB6), SDA(PB7).
* Interfaced the OLED display using I2C SCL(PB6), SDA(PB7).
* Common 3.3v and Ground connection
* Used ST-Link V2 to Program STM32F1

### Software Development
* Used STM32CubeMX for Pinout View. (Optional)
* Developed firmware in Arduino IDE using STM32F1 MCU based Boards.
* Processed ECG signals, Heart-Rate/Spo2 in real time and displayed the waveform and Data on the OLED screen.
* Used C programming Language to communicate among STM32F1, Sensors
(MAX30102, AD8232) and OLED Display.

### Signal Processing
* Filtered and amplified ECG signals using the AD8232 module.
* Ensured real-time responsiveness for smooth waveform visualization.
* Measured Infrared and Red-Light value from finger using MAX30102.
* Ensured real-time responsiveness for approximate Heart Rate and Oxygen Level value.

### Results Analysis and Discussion
## Result Analysis
* **ECG Data:** Successfully visualized real-time ECG signals on the OLED display.
* **Challenges:** 
  - Initial noise in ECG signals was mitigated by using appropriate filters.
  - Ensuring smooth rendering of the ECG graph required careful tuning of data acquisition and display update rates.
* **Heart-Rate/Spo2:** Successfully achieved real-time Heart Rate and Oxygen Level Value.
* **Challenges:**
  - Hardware internal voltage and noise from the achieved infrared and red-light was making it hard to measure the actual heart rate out of it.
  - Ensuring real time appropriate Heart Rate value required smoothing the threshold value with proper algorithm and adjusting peak voltage.

## Discussion
The design and implementation of the Portable Cardiac Monitoring System demonstrated a practical and cost-effective solution for real-time monitoring of ECG, heart rate, and oxygen saturation levels. By leveraging the **STM32F103C8 microcontroller** and key sensors such as the **AD8232** and **MAX30102**, the system achieved reliable data acquisition and visualization on an **OLED display**. Challenges encountered during the development process included noise interference in ECG signals and inaccuracies in heart rate and SpO2 measurements caused by ambient light and motion artifacts. These issues were mitigated using digital filtering techniques, calibration adjustments, and iterative tuning of the signal acquisition process. While these solutions significantly improved performance, further optimization is required to enhance consistency in dynamic and real-world environments. Another challenge was ensuring seamless synchronization between data acquisition and rendering. This required iterative refinement of software algorithms to enable smooth, real-time visualization. Despite these limitations, the project effectively highlights the viability of portable systems for continuous cardiac monitoring. The results emphasize the importance of further development in signal processing, hardware integration, and system reliability to enhance overall performance.

### Conclusion
This project successfully demonstrates the feasibility of a Portable Cardiac Monitoring System capable of real-time ECG visualization, heart rate monitoring, and oxygen saturation measurement. By addressing the limitations of traditional hospital-based monitoring systems such as their bulkiness, high costs, and limited accessibility this portable solution offers an affordable and user-friendly alternative suitable for both personal and clinical use. The implementation underscores the importance of accessible health monitoring systems in empowering individuals to manage their heart health proactively. This is particularly critical in resource-limited settings, where continuous monitoring can help reduce the risk of severe cardiovascular events and alleviate the burden on healthcare systems. Future enhancements could include integrating the ESP8266 Wi-Fi module to enable wireless connectivity and mobile application integration. This would allow users to monitor their health metrics remotely, generate detailed reports, and receive real-time alerts, significantly expanding the system's functionality. Additionally, advancements such as refining signal processing algorithms, incorporating machine learning for anomaly detection, and exploring wearability can further improve its diagnostic and preventive capabilities. These developments position this system as a valuable tool in modern healthcare, paving the way for accessible and efficient continuous cardiac monitoring.
