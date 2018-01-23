#!/bin/sh
# Set up GPIO 4 and set to output
echo "17" > /sys/class/gpio/export
echo "out" > /sys/class/gpio/gpio17/direction

# Write output
echo "1" > /sys/class/gpio/gpio17/value

# Clean up
echo "17" > /sys/class/gpio/unexport
