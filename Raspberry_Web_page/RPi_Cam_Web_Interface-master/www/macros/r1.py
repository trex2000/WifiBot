#!/usr/bin/env python
import pigpio

pi = pigpio.pi()
pi.set_mode(22, pigpio.OUTPUT)
pi.write(22,1)
