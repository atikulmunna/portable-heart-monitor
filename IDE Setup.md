# Arduino IDE Setup & Project Workflow

This project utilizes a **Dual-MCU Architecture** (STM32F103C8T6 and ESP8266). You will need to configure the Arduino IDE to support both boards and install the necessary libraries for hardware communication and cloud connectivity.

## 1. Install Arduino IDE
- Go to the [Arduino Software Page](https://www.arduino.cc/en/software)
- Click **Windows** (or your respective OS)
- Click **Just Download** (No need to enter information unless you wish to donate)
- Install the application and launch it.
- ***Done***

## 2. Configure Board Managers
By default, the Arduino IDE only knows about standard Arduino boards (like the Uno). We need to give it the URLs to understand the STM32 and ESP8266 chips.

1. Launch Arduino IDE. 
2. Go to **File > Preferences**.
3. Locate the field labeled **"Additional Boards Manager URLs"**.
4. Click the small window icon next to the text box and paste **both** of these links on separate lines:
> https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
> http://arduino.esp8266.com/stable/package_esp8266com_index.json

5. Click **OK**.

## 3. Install the Boards
1. Go to **Tools > Board > Boards Manager...**
2. **For the STM32:** Type `STM32` in the search bar. Look for **STM32 MCU based boards** by STMicroelectronics and click **Install**.
3. **For the ESP8266:** Type `esp8266` in the search bar. Look for **esp8266** by ESP8266 Community and click **Install**.
4. You can now close the Boards Manager.

## 4. Install Required Libraries
To install libraries, go to **Sketch > Include Library > Manage Libraries...** (or click the Library icon on the left sidebar). Search for and install the following exact libraries:

### For the STM32 (Hardware & Sensors)
*   **SparkFun MAX3010x Pulse and Proximity Sensor Library** (by SparkFun Electronics) - *For the SpO2/Heart Rate sensor.*
*   **U8g2** (by oliver) - *A highly efficient library for the OLED display.*
*   **Keypad** (by Mark Stanley, Alexander Brevig) - *For navigating the physical menu.*
*   *(Note: The AD8232 ECG sensor uses raw analog readings and does not require a specific library).*

### For the ESP8266 (Wi-Fi & Cloud)
*   **PubSubClient** (by Nick O'Leary) - *Handles the MQTT connection to Ubidots.*
*   **ArduinoJson** (by Benoit Blanchon) - *Packages the sensor data into a clean JSON format.*

---

## 5. IDE Settings & Upload Workflow
Because this project uses two separate microcontrollers, you must flash them individually with different settings. 

**⚠️ CRITICAL RULE:** Before uploading code to *either* board, you must unplug the jumper wires connecting **STM32 PA9 (TX)** to the **ESP8266 (RX)**. If these are connected during an upload, it will cause a timeout error.

### Step A: Flashing the ESP8266 (The Wi-Fi Gateway)
1. Plug the ESP8266 directly into your computer via Micro-USB.
2. In the Arduino IDE, open the `wifi_setup.ino` sketch.
3. Configure the IDE settings under the **Tools** menu:
   *   **Board:** NodeMCU 1.0 (ESP-12E Module)
   *   **Port:** Select the COM port that appears when you plug in the USB.
4. Click **Upload**. (You only need to do this once, or if you change your Wi-Fi password).

### Step B: Flashing the STM32 (The Sensor Brain)
Because the hardware code is large, we must enable Link Time Optimization (LTO) to compress the libraries and fit them into the STM32's flash memory.
1. Plug the ST-Link V2 into your computer, and wire it to the STM32's SWD pins (3.3V, GND, SWDIO, SWCLK).
2. In the Arduino IDE, open the `sensor_to_ubidots.ino` sketch.
3. Configure the IDE settings exactly as follows under the **Tools** menu:
   *   **Board:** Generic STM32F1 series
   *   **Board part number:** BluePill F103C8 (or your specific variant)
   *   **U(S)ART support:** Enabled (generic 'Serial')
   *   **Optimize:** Smallest (-Os) with LTO *(Crucial to prevent memory overflow)*
   *   **C Runtime Library:** Newlib Nano (default)
   *   **Upload Method:** STLink
4. Click **Upload**.

### Step C: Final Assembly
Once both boards say "Done Uploading":
1. Disconnect them from your computer.
2. Reconnect the **TX** and **RX** jumper wires between the two boards.
3. Connect power (ensure both boards share a common Ground).
4. The system is now live and will automatically connect to Wi-Fi and begin streaming data to Ubidots!
