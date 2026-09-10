# wireless-rc-car
Arduino-based wireless RC car using nRF24L01 communication and dual-motor control.
# Wireless RC Car

An Arduino-based wireless RC car controlled using nRF24L01 radio communication and dual-motor control.

## Overview

This project is a wireless-controlled RC car built using Arduino. The system uses nRF24L01 modules for communication between the remote controller and the vehicle.

The receiver processes joystick X/Y input received wirelessly and converts it into motor commands for forward, backward, left, right, and diagonal movement.

## Features

- Wireless communication using nRF24L01
- Joystick-based directional control
- Forward and backward movement
- Left and right turning
- Diagonal movement
- PWM-based motor speed control
- Input dead zone for stable control
- Command validation using a predefined key
- Automatic motor stop when communication is lost
- Serial output for debugging and monitoring

## Hardware

- Arduino
- nRF24L01 wireless transceiver
- Motor driver
- DC motors
- RC car chassis
- Joystick/controller
- Battery/power supply

## Software

- Arduino C/C++
- SPI communication
- RF24 library

## Project Structure

```text
wireless-rc-car/
│
├── receiver/
│   └── receiver.ino
│
└── README.md
