#!/usr/bin/env python
import pigpio

pi = pigpio.pi()
pi.set_mode(4, pigpio.OUTPUT)
pi.write(4,1)
