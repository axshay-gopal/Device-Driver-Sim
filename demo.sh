#!/bin/bash

DRIVER_PATH="./mydevice/mydevice.ko"
DEVICE="/dev/mydevice"

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

clear

echo -e "${BLUE}"
echo "╔══════════════════════════════════════════╗"
echo "║     LINUX DEVICE DRIVER SIMULATOR        ║"
echo "║     Automotive Sensor Simulation         ║"
echo "║     Built by Akshay Gopal                ║"
echo "╚══════════════════════════════════════════╝"
echo -e "${NC}"

sleep 1

echo -e "${YELLOW}[1] Loading driver into Linux kernel...${NC}"
sudo insmod $DRIVER_PATH 2>/dev/null
if [ $? -eq 0 ]; then
    echo -e "    ${GREEN}✓ Driver loaded successfully${NC}"
else
    echo -e "    ${YELLOW}⚠ Driver may already be loaded${NC}"
fi

sudo chmod 666 $DEVICE
if [ -e $DEVICE ]; then
    echo -e "    ${GREEN}✓ /dev/mydevice created${NC}"
else
    echo -e "    ${RED}✗ Device not found${NC}"
    exit 1
fi

sleep 1

echo ""
echo -e "${YELLOW}[2] Testing individual sensor commands...${NC}"
sleep 0.5

echo "get_temperature" > $DEVICE
TEMP=$(cat $DEVICE)
echo -e "    ${GREEN}✓ get_temperature →${NC} $TEMP"
sleep 0.5

echo "get_speed" > $DEVICE
SPEED=$(cat $DEVICE)
echo -e "    ${GREEN}✓ get_speed       →${NC} $SPEED"
sleep 0.5

echo "get_battery" > $DEVICE
BATTERY=$(cat $DEVICE)
echo -e "    ${GREEN}✓ get_battery     →${NC} $BATTERY"
sleep 0.5

echo "get_door" > $DEVICE
DOOR=$(cat $DEVICE)
echo -e "    ${GREEN}✓ get_door        →${NC} $DOOR"
sleep 0.5

echo ""
echo -e "${YELLOW}[3] Reading all sensors at once...${NC}"
echo "get_all" > $DEVICE
echo ""
cat $DEVICE
sleep 1

echo ""
echo -e "${YELLOW}[4] Checking kernel logs...${NC}"
sudo dmesg | grep mydevice | tail -5 | while read line; do
    echo -e "    ${BLUE}$line${NC}"
done

sleep 1

echo ""
echo -e "${YELLOW}[5] Launching live Node.js dashboard...${NC}"
echo -e "    ${GREEN}✓ Starting in 2 seconds. Press Ctrl+C to stop.${NC}"
sleep 2

node ./test/dashboard.js

echo ""
echo -e "${YELLOW}[6] Unloading driver...${NC}"
sudo rmmod mydevice
echo -e "    ${GREEN}✓ Driver unloaded${NC}"
echo -e "    ${GREEN}✓ /dev/mydevice removed${NC}"

echo ""
echo -e "${BLUE}"
echo "╔══════════════════════════════════════════╗"
echo "║         Demo Complete!                   ║"
echo "║                                          ║"
echo "║  Concepts demonstrated:                  ║"
echo "║  ✓ Linux kernel module (C)               ║"
echo "║  ✓ Character device driver               ║"
echo "║  ✓ Kernel space / User space             ║"
echo "║  ✓ copy_to_user / copy_from_user         ║"
echo "║  ✓ file_operations struct                ║"
echo "║  ✓ Node.js reading from kernel device    ║"
echo "╚══════════════════════════════════════════╝"
echo -e "${NC}"
