# from machine import UART, Pin
# import time
# 
# TXD_Pin = 'GP16'
# RXD_Pin = 'GP17'
# RST_Pin = 22
# RST = Pin(RST_Pin, mode=Pin.OUT)
# RST.value(0)
# uart = UART(2, baudrate=115200)
# RST.value(1)
# 
# uart.write("AT\r")
# uart.write("ATD+ +12063845010;\r")
# time.sleep(3)

from sim800 import SIM800
gsm = SIM800()
gsm.setUp()
gsm.makePhoneCall("+12063845010")
