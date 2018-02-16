#!/usr/bin/env python
import pigpio

pi = pigpio.pi()
pi.set_mode(27, pigpio.OUTPUT)
pi.write(27,0)
