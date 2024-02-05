ESP32RET
=======

Reverse Engineering Tool running on RoboBoard X4. Supports USB and Wi-Fi connection.

RoboBoard X4 CAN bus documentation can be found in [RoboBoard -> CAN](https://docs.totemmaker.net/roboboard/api/can/).

## Setup

1. Download [ESP32RET-master.zip](https://github.com/totemmaker/ESP32RET/archive/refs/heads/master.zip) end extract it.
1. Rename `ESP32RET-master` to `ESP32RET`.
1. Open `ESP32RET.ino` in Arduino IDE.
1. Compile and upload code to RoboBoard X4.

## Connecting to SavvyCAN

Download [SavvyCAN](https://savvycan.com/).

### Over USB

1. Connect USB cable from RoboBoard X4 to PC.
1. In SavvyCAN click "Connection" -> "Open Connection Window".
1. Click "Add New Device Connection".
1. Select "Serial Connection (GVERT)" and Serial Port.
1. Click "Create New Connection" and close window.

### Over Wi-Fi

1. Connect PC to Wi-Fi access point "RoboBoardX4-GVRET".
1. In SavvyCAN click "Connection" -> "Open Connection Window".
1. Click "Add New Device Connection".
1. Select "Network Connection (GVERT)" and IP `192.168.4.1`.
1. Click "Create New Connection" and close window.

## Serial console

1. Connect USB cable from RoboBoard X4 to PC.
1. Open Serial Monitor with baud rate `1000000`.
1. Type `?` to print help menu.

## Credits

This project is a modified fork of [ESP32RET](https://github.com/collin80/ESP32RET).

