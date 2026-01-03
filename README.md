# Rust OS for ESP32

**Rust OS** is a simple C++-based operating system for ESP32 devices such as the **M5StickC Plus 2**. It provides a basic menu interface and several essential functions including WiFi management, IR remote control, system information, power management, and sleep mode. This project is designed as a base for creating custom ESP32 software.

---

## Features

### Main Menu

- **WiFi Tools**
  - **Scan:** Scan for nearby WiFi networks and display the top 6 results on the screen.
  - **AP Mode:** Toggle ESP32 as a WiFi Access Point. The device will broadcast as `M5StickC` when enabled.

- **IR Remote**
  - Send predefined NEC IR commands for simple control:
    - **Power**: Sends the IR power command.
    - **Vol +**: Sends the IR volume up command.
    - **Vol -**: Sends the IR volume down command.

- **System**
  - **Info:** Displays free heap memory and WiFi AP status.
  - **Battery:** Shows battery percentage, voltage, and charging status.
  - **Sleep:** Puts the device into light sleep mode to save power.
  - **Reset:** Clears all stored settings and restarts the ESP32.

---

## Hardware Requirements

- ESP32 device (tested on M5StickC Plus 2)
- Optional: IR LED connected to GPIO 19 for sending IR commands

---

## Software Requirements

- Arduino IDE
- M5StickC library installed
- IRremote library installed
- Preferences library (included with ESP32 core)

---

## Installation

1. **Open Arduino IDE.**  
2. **Copy the code** into a new sketch.  
3. **Select the board:** Tools → Board → M5StickC Plus  
4. **Connect your ESP32** device via USB.  
5. **Press Upload** to flash the code onto your ESP32.

---

## Menu Navigation

- **Button A:** Move the cursor down in the current menu.  
- **Button B:** Select the highlighted menu item or perform an action.  
- **Button C / Long press B:** Go back to the main menu.  

The display will turn off automatically after **10 seconds** of inactivity.

---

## WiFi Functions

- **Scan:** Shows nearby WiFi networks on screen.  
- **AP Mode:** Toggle access point mode. The state is saved in device memory, so it will persist after restart.

---

## IR Functions

- Send NEC IR commands using a connected IR LED.  
- Each menu item corresponds to a predefined command (Power, Volume +, Volume -).

---

## System Functions

- **Info:** Shows available heap and WiFi AP status.  
- **Battery:** Shows battery level, voltage, and charging state.  
- **Sleep:** Puts the display and ESP32 into low-power mode.  
- **Reset:** Clears all saved preferences and restarts the device.

---

## License

This project is licensed under the **MIT License**. See the LICENSE file for details.

---

## Notes

- This code is intended as a **base for customization**. You can add new menu items, IR commands, or WiFi features.  
- Ensure your IR LED is connected to GPIO 19 or change the `IR_PIN` in the code.  
- Compatible with **Arduino IDE**; no PlatformIO or other environments required, though it can be adapted.

---



