# Linux Device Driver Simulator
### Automotive Sensor Simulation | Akshay Gopal

## What This Is
A Linux character device driver written in C that simulates
automotive sensors — temperature, speed, battery, door status.
Any program can talk to it by reading/writing /dev/mydevice,
exactly how real embedded Linux hardware works.

## Project Structure
DeviceDriverSim/
├── mydevice/
│   ├── mydevice.c      ← kernel driver (C)
│   └── Makefile        ← kernel build recipe
├── test/
│   └── dashboard.js    ← Node.js live dashboard
├── demo.sh             ← full demo script
└── README.md

## Concepts Demonstrated
- Linux kernel module development in C
- Character device driver (register_chrdev)
- Kernel space vs User space
- copy_to_user / copy_from_user
- file_operations struct (open/read/write/release)
- Kernel memory (get_random_bytes)
- Node.js application reading from kernel device

## How to Run

### Build the driver
cd mydevice
make

### Run full demo
cd ..
./demo.sh

### Manual commands
sudo insmod mydevice/mydevice.ko
sudo chmod 666 /dev/mydevice

echo "get_temperature" > /dev/mydevice && cat /dev/mydevice
echo "get_speed"       > /dev/mydevice && cat /dev/mydevice
echo "get_battery"     > /dev/mydevice && cat /dev/mydevice
echo "get_door"        > /dev/mydevice && cat /dev/mydevice
echo "get_all"         > /dev/mydevice && cat /dev/mydevice

node test/dashboard.js

sudo rmmod mydevice
