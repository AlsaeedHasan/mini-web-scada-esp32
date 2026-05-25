# Web-Based SCADA System (ESP32 Node)

![ESP-IDF](https://img.shields.io/badge/ESP--IDF-v5.x-red.svg?style=flat-square&logo=espressif)
![FreeRTOS](https://img.shields.io/badge/FreeRTOS-Supported-red.svg?style=flat-square&logo=freertos)
![C](https://img.shields.io/badge/Language-C-blue.svg?style=flat-square&logo=c)

A lightweight, web-oriented SCADA (Supervisory Control and Data Acquisition) node built around the ESP32 microcontroller and the ESP-IDF framework. This project bridges hardware design with software engineering by reading physical inputs and controlling outputs via a web interface, orchestrated entirely by a non-blocking FreeRTOS architecture.

Currently, this repository features the core embedded application and custom bare-metal drivers, with a full web server implementation planned for future stages.

## 🌟 Features

- **Custom Bare-Metal Drivers**: Interfaces with shift registers using efficient bit-banging (`shift_registers.c`), bypassing slow abstracted APIs for higher performance and footprint reduction.
- **FreeRTOS Concurrency**: Entirely non-blocking architecture relying on FreeRTOS tasks. Features dedicated tasks for polling inputs (`sensors_task`) and driving outputs (`leds_task`).
- **Thread-safe Data Passing**: Communication between the distinct FreeRTOS tasks is properly managed using a FreeRTOS `Queue` (FIFO).
- **SoftAP Wi-Fi Provisioning**: Utilizes the ESP-IDF `network_provisioning` component and FreeRTOS `Event Groups` for synchronization. The system seamlessly receives network credentials via a mobile app while the offline SCADA hardware tasks continue running in a non-blocking manner.
- **Hardware Protection**: Robust power supply design mapping 3.3V logic to higher voltage output loads seamlessly.

## 🛠️ Hardware Requirements & Architecture

- **Microcontroller**: ESP32-WROOM-32
- **Input Expansion**: 74HC165 (Shift Register IN) for reading multiple digital signals (e.g., DIP switches) using minimal ESP32 GPIO pins.
- **Output Expansion**: 74HC595 (Shift Register OUT) for controlling multiple discrete outputs (e.g., LEDs via PN2222A low-side NPN transistors).
- **Power Supply**: Robust local power supply utilizing an LM7805 regulator for heavy output loads while maintaining 3.3V logic compatibility for the ESP32.

> Note: Complete hardware schematics are available in PDF format within this repository.

## 📂 Project Structure

This project enforces a clean separation of hardware initialization, bare-metal drivers, and RTOS task definitions:

```text
main/
├── CMakeLists.txt        # Build system configuration
├── main.c                # Application entry point and FreeRTOS task creation
├── pd_wifi.c             # Wi-Fi SoftAP provisioning and network event handling
├── pins.c                # Hardware pin definitions and low-level GPIO setup
├── shift_registers.c     # Bare-metal drivers for 74HC595 and 74HC165
├── tasks.c               # FreeRTOS task implementations (sensors_task, leds_task)
└── include/              # Header files (bitwise.h, pd_wifi.h, etc.)
```

## 🚀 Getting Started (How to Build)

Ensure you have the [ESP-IDF framework](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/) installed and set up on your machine.

1. **Clone the repository:**

   ```bash
   git clone https://github.com/AlsaeedHasan/mini-web-scada-esp32.git mini-web-scada-esp32
   cd mini-web-scada-esp32
   ```

2. **Set the target to ESP32:**

   ```bash
   idf.py set-target esp32
   ```

3. **Build the project:**

   ```bash
   idf.py build
   ```

4. **Flash and monitor:**
   ```bash
   idf.py flash monitor
   ```
   _(Use `Ctrl+]` to safely exit the monitor)_

## 🔮 Future Updates

The project is continually evolving. Upcoming milestones include:

- **HTTP Web Server**: Implementing a lightweight embedded web server exposing endpoints to securely monitor sensor states and remotely control outputs over the network.

---

_Developed by Alsaeed — Computer and Control Systems Engineering Student._
