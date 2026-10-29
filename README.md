# College ERP on ESP32

A simple College ERP demo website hosted directly by an ESP32 DevKit V1 using Arduino IDE. The ESP32 creates its own Wi-Fi access point and serves an embedded HTML/CSS/JavaScript interface.

## Features
- Admin and Student demo login
- Dashboard
- Student directory and branch details
- Attendance, marks and fee-status views
- Sample timetable
- Mobile-friendly layout

## Hardware and software
- ESP32 DevKit V1
- USB data cable
- Arduino IDE
- ESP32 board package installed in Arduino IDE

## Upload steps
1. Install Arduino IDE.
2. In **File → Preferences**, add this board manager URL if it is not already present: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. Open **Tools → Board → Boards Manager**, search for **esp32**, and install **esp32 by Espressif Systems**.
4. Open `CollegeERP_ESP32.ino`.
5. Select **Tools → Board → ESP32 Arduino → ESP32 Dev Module** (or the matching ESP32 board).
6. Select the correct COM port under **Tools → Port**.
7. Upload the sketch. If required, hold the BOOT button while the IDE says “Connecting”.
8. Open Serial Monitor at **115200 baud** to view status messages.

## Connect to the website
1. On a phone or computer, connect to Wi-Fi network `CollegeERP-ESP32`.
2. Password: `college123`
3. Open `http://192.168.4.1` in a browser.

## Demo logins
- Admin: username `admin`, password `admin123`
- Student: username `student`, password `student123`

## Important limitations
This is an educational prototype. Student data is sample data embedded in the firmware. Login checks run in the browser and are not secure authentication. Records are not saved persistently, and this should not be used to store real student information or exposed to the public internet.

## GitHub
Create a repository named `CollegeERP-ESP32`, then upload `CollegeERP_ESP32.ino`, `README.md`, `.gitignore`, and `LICENSE`.
