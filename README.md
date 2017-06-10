# Grbl Interface

##The aim of the project is to insert an arduino mega between the PC & Grbl device.

## Two goals:
- controlling the CNC/3D printer without a computer
- Allow custom display for Grbl intels 

We take advantage of the 2 serial ports on the Mega so to transmit commands and acqs from PC to Grbl, and injecting custom commands from buttons, sliders or joystick.

Constant status polling is send to Grbl and/or spied from the computer stream. This allow us to display these information on a LCD screen / status Leds