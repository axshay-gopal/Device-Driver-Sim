# Linux Device Driver Simulator

**Automotive Sensor Simulation using a Linux Character Device Driver**

A Linux kernel module written in C that simulates automotive sensors and exposes them through a character device (`/dev/mydevice`). User-space applications can communicate with the driver exactly as they would with real embedded hardware on an automotive Linux system.

---

## Overview

This project demonstrates the interaction between **kernel space** and **user space** by implementing a custom Linux character device driver.

The driver simulates common vehicle sensors:

* Temperature Sensor
* Vehicle Speed Sensor
* Battery Monitor
* Door Status Sensor

A Node.js dashboard continuously reads data from the device, creating a complete end-to-end simulation of an embedded automotive monitoring system.

---

## Features

* Custom Linux kernel module
* Character device driver implementation
* Dynamic sensor data generation
* User-space communication through `/dev/mydevice`
* Real-time monitoring dashboard using Node.js
* Demonstrates embedded Linux development concepts

---

## Project Structure

```text
DeviceDriverSim/
├── mydevice/
│   ├── mydevice.c       # Linux kernel driver
│   └── Makefile         # Kernel module build file
│
├── test/
│   └── dashboard.js     # Node.js monitoring dashboard
│
├── demo.sh              # Automated demo script
└── README.md
```

---

## Technologies Used

* C
* Linux Kernel Modules
* Character Device Drivers
* Node.js
* Embedded Linux Concepts
* Makefiles
* Bash

---

## Linux Concepts Demonstrated

### Kernel Module Development

* Module initialization and cleanup
* Loading and unloading drivers

### Character Device Drivers

* `register_chrdev()`
* Device creation under `/dev`

### File Operations Interface

* `open()`
* `read()`
* `write()`
* `release()`

### Kernel ↔ User Space Communication

* `copy_to_user()`
* `copy_from_user()`

### Kernel APIs

* `get_random_bytes()`
* Kernel logging using `printk()`

### Embedded Systems Design

* Simulated hardware sensors
* User-space monitoring applications

---

## Build Instructions

### 1. Build the Driver

```bash
cd mydevice
make
```

### 2. Load the Module

```bash
sudo insmod mydevice.ko
```

### 3. Verify Device Creation

```bash
ls /dev/mydevice
```

### 4. Grant Access

```bash
sudo chmod 666 /dev/mydevice
```

---

## Running the Demo

Execute the complete demonstration:

```bash
./demo.sh
```

---

## Manual Sensor Commands

### Temperature

```bash
echo "get_temperature" > /dev/mydevice
cat /dev/mydevice
```

### Speed

```bash
echo "get_speed" > /dev/mydevice
cat /dev/mydevice
```

### Battery

```bash
echo "get_battery" > /dev/mydevice
cat /dev/mydevice
```

### Door Status

```bash
echo "get_door" > /dev/mydevice
cat /dev/mydevice
```

### All Sensors

```bash
echo "get_all" > /dev/mydevice
cat /dev/mydevice
```

---

## Live Dashboard

Start the Node.js dashboard:

```bash
node test/dashboard.js
```

The dashboard continuously reads sensor values from the Linux device and displays them in real time.

---

## Sample Output

```text
Temperature: 36°C
Speed: 82 km/h
Battery: 89%
Door: CLOSED
```

---

## Unload the Driver

```bash
sudo rmmod mydevice
```

---

## Learning Outcomes

This project demonstrates practical knowledge of:

* Linux kernel development
* Device driver programming
* Embedded systems architecture
* Kernel-space and user-space interaction
* Linux character devices
* Node.js integration with low-level system components

---

## Future Improvements

* Support multiple sensor channels
* Implement IOCTL commands
* Add interrupt simulation
* Create a web-based dashboard
* Dockerized testing environment
* CAN bus message simulation

---

## Author

**Akshay Gopal**

Automotive Software | Embedded Linux | Linux Kernel Development
